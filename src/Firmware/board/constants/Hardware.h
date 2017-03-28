#pragma once

#define MAX_PADS                    9
#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3
#ifdef BOARD_R2
#define NUMBER_OF_BUTTON_COLUMNS    8
#define NUMBER_OF_BUTTON_ROWS       8
#endif
#ifdef BOARD_R1
#define MAX_NUMBER_OF_BUTTONS       32
#elif defined (BOARD_R2)
#define MAX_NUMBER_OF_BUTTONS       (NUMBER_OF_BUTTON_COLUMNS*NUMBER_OF_BUTTON_ROWS)
#endif

#ifdef BOARD_R1
#define MAX_NUMBER_OF_ENCODERS      10
#elif defined (BOARD_R2)
#define MAX_NUMBER_OF_ENCODERS      (MAX_NUMBER_OF_BUTTONS/2)
#define CONNECTED_ENCODERS          10
#endif

#define MAX_NUMBER_OF_LEDS          (NUMBER_OF_LED_COLUMNS*NUMBER_OF_LED_ROWS)

//uncomment if leds use reverse logic for setting on/off state
#ifdef BOARD_R2
#define LED_INVERT
#endif
