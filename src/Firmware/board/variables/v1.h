#pragma once

#ifdef BOARD_R1

#include "../constants/Constants.h"

//leds
extern volatile int8_t     activeLEDcolumn;

//encoders
extern uint16_t             encoderData[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort1Array[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort2Array[NUMBER_OF_ENCODERS];
extern volatile uint8_t     encoderBuffer[NUMBER_OF_ENCODERS];

#endif
