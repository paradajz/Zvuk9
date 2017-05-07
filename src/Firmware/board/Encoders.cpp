#include "Board.h"

uint8_t             encoderState[MAX_NUMBER_OF_ENCODERS];
volatile int8_t     encPulses[MAX_NUMBER_OF_ENCODERS];
int8_t              encPulses_x4[MAX_NUMBER_OF_ENCODERS];

int8_t Board::getEncoderState(uint8_t encoderNumber)
{
    int8_t pulses;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        pulses = encPulses[encoderNumber];
        encPulses[encoderNumber] = 0;
    }

    if (encInverted[encoderNumber])
        return pulses*-1;

    return pulses;
}

bool Board::encoderEnabled(uint8_t encoderNumber)
{
    for (int i=0; i<(int)sizeof(encoderMap); i++)
    {
        if (encoderMap[i] == encoderNumber)
            return true;
    }

    return false;
}