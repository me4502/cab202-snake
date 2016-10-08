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
 * Spawns a piece of food somewhere in the playing field.
 */
void spawn_food();

/**
 * Moves the snack to the given position.
 *
 * <p> Note: The position may change if off screen. </p>
 *
 * @param x The x coordinate
 * @param y The y coordinate
 */
void move_snake_to(char x, char y);

/**
 * Gets the character count of a string.
 *
 * @param str The input string
 * @return The character count
 */
unsigned char string_length(char * str);