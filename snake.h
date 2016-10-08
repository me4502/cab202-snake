#pragma once

/**
 * Updates the gamestate.
 */
void update();

/**
 * Renders the game to the LCD display.
 */
void render();

/**
 * Reset the variables, ready for a new game.
 */
void setup_game();

/**
 * Gets the character count of a string.
 *
 * @param str The input string
 * @return The character count
 */
unsigned char string_length(char * str);