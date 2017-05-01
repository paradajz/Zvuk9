#pragma once

#define NUMBER_OF_PADS              9
#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3
#define NUMBER_OF_BUTTON_COLUMNS    8
#define NUMBER_OF_BUTTON_ROWS       8
#define MAX_NUMBER_OF_BUTTONS       (NUMBER_OF_BUTTON_COLUMNS*NUMBER_OF_BUTTON_ROWS)
#define MAX_NUMBER_OF_ENCODERS      (MAX_NUMBER_OF_BUTTONS/2)

#define MAX_NUMBER_OF_LEDS          (NUMBER_OF_LED_COLUMNS*NUMBER_OF_LED_ROWS)

//uncomment if leds use reverse logic for setting on/off state
#define LED_INVERT