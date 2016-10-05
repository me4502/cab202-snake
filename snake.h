#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

typedef struct snakeSegment {
    int x;
    int y;
} SnakeSegment;

int length = 2;
int lives = 5;
int score = 0;

char * intro_text[] = {
        "By Madeline Miller",
        "n9342401"
};

// Game State
#define GAMESTATE_OPENING 0
#define GAMESTATE_PLAYING 1
#define GAMESTATE_GAMEOVER 2

unsigned char gamestate = GAMESTATE_OPENING;

// Method Declarations

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

// Utility Functions

/**
 * Gets the character count of a string.
 *
 * @param str The input string
 * @return The character count
 */
int string_length(char * str);