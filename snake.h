#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

#include "linked_list.h"

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

// Game State
#define GAMESTATE_OPENING 0
#define GAMESTATE_PLAYING 1
#define GAMESTATE_GAMEOVER 2

unsigned char gamestate = GAMESTATE_OPENING;

// Snake
LinkedListEntry ** snake;

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
unsigned char string_length(char * str);