cmake_minimum_required(VERSION 3.5)
project(Assignment_2 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=atmega32u4 -Os -DF_CPU=8000000UL -std=gnu99 -Wall -Werror -Wl,-u,vfprintf -lprintf_flt -lcab202_teensy -lm -D__AVR_ATmega32U4__")

set(SOURCE_FILES Makefile snake.c linked_list.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cab202_teensy/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/usb_debug_only/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/avr/)

add_executable(Assignment_2 snake.c linked_list.c linked_list.h)