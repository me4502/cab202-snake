#
# 	CAB202 Tutorial 9
#	Generic Makefile for compiling with floating point printf
#
#	B.Talbot, September 2015
#	Queensland University of Technology
#

# Modify these
SRC=snake
CAB202_LIB_DIR=cab202_teensy
DEBUG_LIB_DIR=usb_debug_only

# The rest should be all good as is
FLAGS=-mmcu=atmega32u4 -Os -DF_CPU=8000000UL -std=gnu99 -Wall -Werror
LIBS=-Wl,-u,vfprintf -lprintf_flt -lcab202_teensy -lm

DEBUG_FLAGS=-DUSB_DEBUG -I$(DEBUG_LIB_DIR) -L$(DEBUG_LIB_DIR)
DEBUG_LIBS=-lexample

# Default 'recipe'
all:
	avr-gcc $(SRC).c $(FLAGS) -I$(CAB202_LIB_DIR) -L$(CAB202_LIB_DIR) $(LIBS) -o $(SRC).o
	avr-objcopy -O ihex $(SRC).o $(SRC).hex

# Cleaning  (be wary of this in directories with lots of executables...)
clean:
	rm *.o
	rm *.hex

# Debug
debug:
	avr-gcc $(SRC).c $(FLAGS) $(DEBUG_FLAGS) -I$(CAB202_LIB_DIR) -L$(CAB202_LIB_DIR) $(LIBS) $(DEBUG_LIBS) -o $(SRC)-debug.o
	avr-objcopy -O ihex $(SRC)-debug.o $(SRC)-debug.hex