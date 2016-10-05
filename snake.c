#include "snake.h"

int main() {
    set_clock_speed(CPU_8MHz);

    lcd_init(LCD_DEFAULT_CONTRAST);

    while (1) {
        update();

        render();

        _delay_ms(50);
    }

    return 0;
}

void setup_game() {
    length = 2;
    lives = 5;
    score = 0;

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
    }

    show_screen();
}

int string_length(char * str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}