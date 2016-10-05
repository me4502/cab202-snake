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
void update();

void render();

// Utility Functions
int string_length(char * str);