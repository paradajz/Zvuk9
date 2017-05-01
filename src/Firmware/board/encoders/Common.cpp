#include "../Board.h"

uint8_t             encoderState[MAX_NUMBER_OF_ENCODERS];
volatile int8_t     encPulses[MAX_NUMBER_OF_ENCODERS];
int8_t              encPulses_x4[MAX_NUMBER_OF_ENCODERS];