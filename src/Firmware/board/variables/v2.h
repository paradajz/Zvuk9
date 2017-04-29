#pragma once

#include "../constants/Constants.h"

extern volatile int8_t      activeLEDcolumn;
extern uint8_t              encoderState[CONNECTED_ENCODERS];
extern int8_t               encPulses_x4[CONNECTED_ENCODERS];
extern volatile int8_t      encPulses[CONNECTED_ENCODERS];
extern volatile uint8_t     buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];
extern volatile uint8_t     inputBuffer[DIGITAL_BUFFER_SIZE][NUMBER_OF_BUTTON_COLUMNS];
extern uint8_t              inputBuffer_copy[NUMBER_OF_BUTTON_COLUMNS];
