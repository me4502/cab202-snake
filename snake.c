#include "snake.h"

#define FPS_DELAY 1000/60

int main() {
    set_clock_speed(CPU_8MHz);

    lcd_init(LCD_DEFAULT_CONTRAST);

#ifdef USB_DEBUG
    usb_init();
	while (!usb_configured());

    _delay_ms(1000);
#endif

    while (1) {
        update();

        render();

        _delay_ms(FPS_DELAY);
    }

    return 0;
}

void setup_game() {
#ifdef USB_DEBUG
    print("Initializing Game\n");
#endif
    lives = 5;
    score = 0;

#ifdef USB_DEBUG
    print("Creating Snake\n");
#endif
    LinkedListEntry * head = malloc(sizeof(LinkedListEntry));
    head->value = malloc(sizeof(SnakeSegment));
    ((SnakeSegment *) head->value)->x = (LCD_X / 2) / 3;
    ((SnakeSegment *) head->value)->y = (LCD_Y / 2) / 3;

    head->next = malloc(sizeof(LinkedListEntry));
    head->next->value = malloc(sizeof(SnakeSegment));
    ((SnakeSegment *) head->next->value)->x = ((LCD_X / 2) / 3) - 1;
    ((SnakeSegment *) head->next->value)->y = (LCD_Y / 2) / 3;

    snake = &head;

#ifdef USB_DEBUG
    print("Setting Gamestate to Playing\n");
#endif
    gamestate = GAMESTATE_PLAYING;
}

void update() {

}

void render() {
    // Drawing
    clear_screen();

    if (gamestate == GAMESTATE_OPENING) {
        for (int i = 0; i < sizeof(intro_text) / sizeof(intro_text[0]); i++) {
            char * text = intro_text[i];

            int x = LCD_X / 2 - (string_length(text) * 5) / 2; // Glyphs have a width of 5

            draw_string((unsigned char) x, (unsigned char) (9 * 2 + i * 9), text);
        }
        show_screen();
        _delay_ms(2000); // Delay for 2 seconds. // TODO Make this a little neater. Timers etc

        setup_game();
    } else if (gamestate == GAMESTATE_PLAYING) {
        char * info_text = malloc(sizeof(char));
        sprintf(info_text, "Score:%d Lives:%d", score, lives);
        draw_string(0, 0, info_text);

        LinkedListEntry * snake_bit = *snake;

        while (snake_bit != NULL) {
            SnakeSegment segment = (*(SnakeSegment *) snake_bit->value);
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    set_pixel(segment.x, segment.y, 1);
                }
            }

            snake_bit = snake_bit->next;
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