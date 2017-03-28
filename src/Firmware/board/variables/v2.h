#pragma once

#ifdef BOARD_R2
#include "../constants/Constants.h"

extern volatile int8_t      activeLEDcolumn;
extern volatile uint8_t     activeButtonColumn;
extern volatile bool        _analogDataAvailable;
extern uint8_t              encoderState[CONNECTED_ENCODERS];
extern int8_t               encPulses_x4[CONNECTED_ENCODERS];
extern volatile int8_t      encPulses[CONNECTED_ENCODERS];
extern volatile uint8_t     buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];
extern uint8_t              inputBuffer[NUMBER_OF_BUTTON_COLUMNS];
#endif
