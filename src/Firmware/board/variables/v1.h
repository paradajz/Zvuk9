#pragma once

#ifdef BOARD_R1

#include "../constants/Constants.h"

//leds
extern volatile int8_t     activeLEDcolumn;

//encoders
extern uint16_t             encoderData[MAX_NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort1Array[MAX_NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort2Array[MAX_NUMBER_OF_ENCODERS];
extern volatile uint8_t     encoderBuffer[MAX_NUMBER_OF_ENCODERS];

#endif
