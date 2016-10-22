#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "cpu_speed.h"
#include "graphics.h"
#include "lcd.h"

#include "snake.h"

#ifdef USB_DEBUG
#include "usb_debug_only.h"
#include "print.h"
#include "analog.h"
#endif

#define SCALED_WIDTH (LCD_X / 3)
#define SCALED_HEIGHT ((LCD_Y / 3) - 3)

typedef struct scaledCoordinate {
    char x;
    char y;
} ScaledCoordinate;

char lives = 5;
unsigned char score = 0;

char * intro_text[] = {
        "By Madeline Miller",
        "n9342401"
};

char * game_over_text[] = {
        "Game Over",
};

//const float timer0_overflow = 0.03264;
volatile int timer0_index = 0;
unsigned int time_since_press[4];

typedef enum {OPENING, PLAYING, GAMEOVER} GameState;
GameState gamestate = OPENING;

// Walls
unsigned char show_walls = 0;

typedef enum {HORIZONTAL, VERTICAL} WallDirection;

typedef struct wallSegment {
    ScaledCoordinate pos;
    WallDirection direction;
} WallSegment;

WallSegment walls[SCALED_WIDTH * 3];
unsigned char wall_length;

// Snake
ScaledCoordinate snake[200];
unsigned char snake_length;
char snake_dx = 0, snake_dy = 0;
unsigned char snake_speed = 6;

// Food
ScaledCoordinate food_point;

unsigned char disable_bit(unsigned char input, unsigned char bit) {
    return (unsigned char) (input & ~(1 << bit));
}

unsigned char get_bit(unsigned char input, unsigned char bit) {
    return (unsigned char) ((input >> bit) & 1);
}

inline unsigned char max(unsigned char a, unsigned char b) {
    return a > b ? a : b;
}

int main() {
    set_clock_speed(CPU_8MHz);

    lcd_init(LCD_DEFAULT_CONTRAST);

    // Initialize Buttons
    PORTD = disable_bit((unsigned char) PORTD, 1); //UP
    PORTB = disable_bit((unsigned char) PORTB, 1); //LEFT
    PORTD = disable_bit((unsigned char) PORTD, 0); //RIGHT
    PORTB = disable_bit((unsigned char) PORTB, 7); //DOWN

    PORTF = disable_bit((unsigned char) PORTF, 5); //SW2
    PORTF = disable_bit((unsigned char) PORTF, 6); //SW3

    // Initialize Timers

    // Timer 0
    // Set to normal mode
    TCCR0B &= ~((1<<WGM02));
    // Set prescaler to 1024
    TCCR0B |= 1 << CS00 | 1 << CS02;
    TCCR0B &= ~(1 << CS01);

    // Enable interrupts
    TIMSK0 |= 1 << TOIE0;
    sei();

    // Initialize potentiometers
    ADMUX = (1 << REFS0);
    // Prescaler of 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // If USB Debugging is enabled, connect the debugger.
#ifdef USB_DEBUG
    usb_init();
	while (!usb_configured());
#endif

    _delay_ms(50);

    timer0_index = 0;

    while (1) {
        update();
        render();
    }

    return 0;
}

void setup_game() {
    lives = 5;
    score = 0;

    reset_snake();

    wall_length = 0;

    // Setup walls. 3 of them.
    int wall_count = (rand() % 1) + 3;
    for (int i = 0; i <= wall_count; i++) {
        WallDirection direction = (rand() % 2) == 1 ? HORIZONTAL : VERTICAL;
        if (direction == HORIZONTAL) {
            unsigned char length = max(4, (unsigned char) (rand() % SCALED_WIDTH));

#ifdef USB_DEBUG
            print("length: ");
        phex16(length);
            print("\n");
#endif

            char y = (char) (rand() % SCALED_HEIGHT);
            for (char x = 0; x <= length; x++) {
                ScaledCoordinate coordinate;
                coordinate.x = x;
                coordinate.y = y;

                WallSegment segment;
                segment.direction = direction;
                segment.pos = coordinate;

                walls[wall_length ++] = segment;
            }
        } else {
            unsigned char length = max(4, (unsigned char) (rand() % SCALED_HEIGHT));

#ifdef USB_DEBUG
            print("length: ");
            phex16(length);
            print("\n");
#endif

            char x = (char) (rand() % SCALED_WIDTH);
            for (char y = 0; y <= length; y++) {
                ScaledCoordinate coordinate;
                coordinate.x = x;
                coordinate.y = y;

                WallSegment segment;
                segment.direction = direction;
                segment.pos = coordinate;

                walls[wall_length ++] = segment;
            }
        }
    }

    spawn_food();

    gamestate = PLAYING;
}

void reset_snake() {
    snake_dx = 0;
    snake_dy = 0;

    ScaledCoordinate head;
    head.x = (LCD_X / 2) / 3;
    head.y = (LCD_Y / 2) / 3;

    ScaledCoordinate next;
    next.x = ((LCD_X / 2) / 3) - 1;
    next.y = (LCD_Y / 2) / 3;

    snake[0] = head;
    snake[1] = next;

    for (int i = 2; i < 100; i++) {
        ScaledCoordinate dummy;
        dummy.x = 0;
        dummy.y = 0;
        snake[i] = dummy;
    }

    snake_length = 2;
}

void spawn_food() {
    while (1) {
        food_point.x = (char) (rand() % SCALED_WIDTH);
        food_point.y = (char) (rand() % SCALED_HEIGHT);

        char is_invalid = 0;
        if (show_walls == 1) {
            for (int i = 0; i < wall_length; i++) {
                if (walls[i].pos.x == food_point.x && walls[i].pos.y == food_point.y) {
                    is_invalid = 1;
                    break;
                }
            }
        }
        if (is_invalid == 0) {
            break;
        }
    }

#ifdef USB_DEBUG
    print("Food: ");
    phex16(food_point.x);
    print(" ");
    phex16(food_point.y);
    print("\n");
#endif
}

void move_snake_to(char x, char y) {
    ScaledCoordinate last_point = snake[snake_length - 1];
    last_point.x = x;
    last_point.y = y;
    if (last_point.x < 0)
        last_point.x = (char) (SCALED_WIDTH - 1);
    else if (last_point.x >= SCALED_WIDTH)
        last_point.x = 0;

    if (last_point.y < 0)
        last_point.y = (char) (SCALED_HEIGHT - 1);
    else if (last_point.y >= SCALED_HEIGHT)
        last_point.y = 0;

    if (last_point.x == food_point.x && last_point.y == food_point.y) {
        score += (show_walls == 1 ? 2 : 1);
        spawn_food();

        snake_length ++;
    }

    for (int i = snake_length - 1; i >= 0; i--) {
        if (snake[i].x == last_point.x && snake[i].y == last_point.y) {
            lives --;
            reset_snake();
            return;
        }
        snake[i+1] = snake[i];
    }

    if (show_walls == 1) {
        for (int i = 0; i < wall_length; i++) {
            WallSegment wall = walls[i];
            if (last_point.x == wall.pos.x && last_point.y == wall.pos.y) {
                lives --;
                reset_snake();
                return;
            }
        }
    }

    snake[0] = last_point;
}

void update() {
    if (gamestate == OPENING) {
        if (timer0_index >= 61) {
            setup_game();
        }
    } else if (gamestate == PLAYING) {
        if (timer0_index > snake_speed ) {
            if (snake_dx != 0 || snake_dy != 0) {
                move_snake_to(snake[0].x + snake_dx, snake[0].y + snake_dy);
                timer0_index = 0;
            }

            ADMUX = (ADMUX & 0xF8) | 1;
            ADCSRA |= (1 << ADSC);
            while(ADCSRA & (1 << ADSC));
            snake_speed = (unsigned char) (ADC / 102 + 2);
        }

        if (get_bit((unsigned char) PIND, 1) == 1 && time_since_press[0] > 15) {
            snake_dx = 0;
            snake_dy = -1;
            time_since_press[0] = 0;
        } else if (get_bit((unsigned char) PINB, 1) == 1 && time_since_press[1] > 15) {
            snake_dx = -1;
            snake_dy = 0;
            time_since_press[1] = 0;
        } else if (get_bit((unsigned char) PIND, 0) == 1 && time_since_press[2] > 15) {
            snake_dx = 1;
            snake_dy = 0;
            time_since_press[2] = 0;
        } else if (get_bit((unsigned char) PINB, 7) == 1 && time_since_press[3] > 15) {
            snake_dx = 0;
            snake_dy = 1;
            time_since_press[3] = 0;
        }

        for (int i = 0; i < 4; i++) {
            time_since_press[i] ++;
        }

        if (get_bit((unsigned char) PINF, 5) == 1) {
            show_walls = 0;
        } else if (get_bit((unsigned char) PINF, 6) == 1) {
            show_walls = 1;
        }

        if (lives < 0) {
            gamestate = GAMEOVER;
            timer0_index = 0;
        }
    } else if (gamestate == GAMEOVER) {
        if (timer0_index >= 61) {
            gamestate = OPENING;
            timer0_index = 0;
        }
    }
}

void render() {
    clear_screen();

    if (gamestate == OPENING) {
        for (int i = 0; i < sizeof(intro_text) / sizeof(intro_text[0]); i++) {
            char * text = intro_text[i];

            int x = LCD_X / 2 - (string_length(text) * 5) / 2; // Glyphs have a width of 5

            draw_string((unsigned char) x, (unsigned char) (9 * 2 + i * 9), text);
        }
    } else if (gamestate == PLAYING) {
        char info_text[16];
        sprintf(info_text, "Score:%d Lives:%d", score, lives);
        draw_string(0, 0, info_text);

        for (int i = 0; i < snake_length; i++) {
            ScaledCoordinate segment = snake[i];
            for (unsigned char x = 0; x < 3; x++) {
                for (unsigned char y = 0; y < 3; y++) {
                    set_pixel((unsigned char) (segment.x * 3 + x), (unsigned char) (segment.y * 3 + y + 8), 1);
                }
            }
        }

        // Draw food.
        for (unsigned char x = 0; x < 3; x++) {
            for (unsigned char y = 0; y < 3; y++) {
                set_pixel((unsigned char) (food_point.x * 3 + x), (unsigned char) (food_point.y * 3 + y + 8), 1);
            }
        }

        // Draw the walls
        if (show_walls == 1) {
            for (int i = 0; i < wall_length; i++) {
                WallSegment wall = walls[i];
                if (wall.direction == HORIZONTAL) {
                    for (unsigned char x = 0; x < 3; x++) {
                        set_pixel((unsigned char) (wall.pos.x * 3 + x), (unsigned char) (wall.pos.y * 3 + 1 + 8), 1);
                    }
                } else {
                    for (unsigned char y = 0; y < 3; y++) {
                        set_pixel((unsigned char) (wall.pos.x * 3 + 1), (unsigned char) (wall.pos.y * 3 + y + 8), 1);
                    }
                }
            }
        }
    } else if (gamestate == GAMEOVER) {
        for (int i = 0; i < sizeof(game_over_text) / sizeof(game_over_text[0]); i++) {
            char * text = game_over_text[i];

            int x = LCD_X / 2 - (string_length(text) * 5) / 2; // Glyphs have a width of 5

            draw_string((unsigned char) x, (unsigned char) (9 * 2 + i * 9), text);
        }
    }

    show_screen();
}

unsigned char string_length(char * str) {
    unsigned char length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Timer 0 Interrupt
ISR(TIMER0_OVF_vect){
    timer0_index ++;
}