#pragma once

#include "constants/Constants.h"

//LEDs
extern volatile int8_t      activeLEDcolumn;
extern volatile uint8_t     ledState[NUMBER_OF_LEDS];
extern int16_t              transitionCounter[NUMBER_OF_LEDS];
extern volatile uint8_t     pwmSteps;

//encoders
extern uint16_t             encoderData[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort1Array[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort2Array[NUMBER_OF_ENCODERS];
extern volatile uint8_t     encoderBuffer[NUMBER_OF_ENCODERS];
