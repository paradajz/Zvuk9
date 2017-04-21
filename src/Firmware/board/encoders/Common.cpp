#include "../Board.h"

uint8_t             encoderState[CONNECTED_ENCODERS];
int8_t              encPulses_x4[CONNECTED_ENCODERS];
volatile int8_t     encPulses[CONNECTED_ENCODERS];
