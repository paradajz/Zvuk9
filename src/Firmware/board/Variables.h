#pragma once

#include "constants/Constants.h"

extern volatile int8_t      activeLEDcolumn;
extern uint8_t              encoderState[MAX_NUMBER_OF_ENCODERS];
extern volatile int8_t      encPulses[MAX_NUMBER_OF_ENCODERS];
extern int8_t               encPulses_x4[MAX_NUMBER_OF_ENCODERS];
extern volatile uint8_t     buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];
extern uint16_t             padPressed;
extern uint8_t              padReadingIndex;