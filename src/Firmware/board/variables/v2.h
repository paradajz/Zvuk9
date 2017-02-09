#pragma once

#ifdef BOARD_R2
#include "constants/Constants.h"

extern bool                 dmBufferCopied;
extern volatile uint8_t     activeLEDcolumn;
extern volatile uint64_t    inputBuffer[DIGITAL_BUFFER_SIZE];
extern volatile uint8_t     digital_buffer_head;
extern volatile uint8_t     digital_buffer_tail;
extern volatile uint8_t     activeButtonColumn;
extern volatile bool        _analogDataAvailable;
extern bool                 encodersProcessed;
extern uint64_t             inputMatrixBufferCopy;
extern bool                 buttonsProcessed;

#endif
