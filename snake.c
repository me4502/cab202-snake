#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"

#include "linked_list.h"

#include "snake.h"

#ifdef USB_DEBUG
#include "usb_debug_only.h"
#include "print.h"
#include "analog.h"
#endif

typedef struct snakeSegment {
    unsigned char x;
    unsigned char y;
} SnakeSegment;

unsigned char lives = 5;
unsigned char score = 0;

char * intro_text[] = {
        "By Madeline Miller",
        "n9342401"
};

//const float timer0_overflow = 0.03264;
volatile int timer0_index = 0;

typedef enum {OPENING, PLAYING, GAMEOVER} GameState;
GameState gamestate = OPENING;

// Snake
LinkedListEntry ** snake;


int main() {
    set_clock_speed(CPU_8MHz);

    lcd_init(LCD_DEFAULT_CONTRAST);

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

    // If USB Debugging is enabled, connect the debugger.
#ifdef USB_DEBUG
    usb_init();
	while (!usb_configured());

    _delay_ms(1000);
#endif

    timer0_index = 0;

    while (1) {
        update();

        render();

        _delay_ms(20);
    }

    return 0;
}

void setup_game() {
    lives = 5;
    score = 0;

    LinkedListEntry * head = malloc(sizeof(LinkedListEntry));
    head->value = malloc(sizeof(SnakeSegment));
    ((SnakeSegment *) head->value)->x = (LCD_X / 2) / 3;
    ((SnakeSegment *) head->value)->y = (LCD_Y / 2) / 3;
    head->next = NULL;

    SnakeSegment * next = malloc(sizeof(SnakeSegment));
    next->x = ((LCD_X / 2) / 3) - 1;
    next->y = (LCD_Y / 2) / 3;

    push_to_back(head, next);

    snake = &head;

    gamestate = PLAYING;
}

void update() {
    if (gamestate == OPENING) {
        if (timer0_index >= 61) {
            setup_game();
        }
    } else if (gamestate == PLAYING) {
    } else if (gamestate == GAMEOVER) {
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

        LinkedListEntry * snake_bit = *snake;

        while (snake_bit != NULL) {
            SnakeSegment segment = (*(SnakeSegment *) snake_bit->value);
            for (char x = -1; x < 2; x++) {
                for (char y = -1; y < 2; y++) {
                    set_pixel(segment.x + x, segment.y + y, 1);
                }
            }

            snake_bit = snake_bit->next;
        }
    } else if (gamestate == GAMEOVER) {
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