cmake_minimum_required(VERSION 3.5)
project(Assignment_2 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -D_XOPEN_SOURCE=500 -Wall -Werror -lncurses -lm")

set(SOURCE_FILES Makefile snake.c)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/cab202_teensy/)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cab202_teensy/)

add_executable(Assignment_2 snake.c)