#pragma once

#define NUMBER_OF_ENCODERS          10
#define MAX_PADS                    9
#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3
#define MAX_NUMBER_OF_BUTTONS       32
#define MAX_NUMBER_OF_LEDS          (NUMBER_OF_LED_COLUMNS*NUMBER_OF_LED_ROWS)

//uncomment if leds use reverse logic for setting on/off state
#ifdef BOARD_R2
#define LED_INVERT
#endif
