#include "Board.h"

volatile int8_t     activeLEDcolumn;

volatile bool       blinkEnabled,
                    blinkState;

volatile uint8_t    pwmSteps,
                    ledState[MAX_NUMBER_OF_LEDS];

volatile uint16_t   ledBlinkTime;

volatile int8_t     transitionCounter[MAX_NUMBER_OF_LEDS];
volatile uint32_t   blinkTimerCounter;