#include "../Board.h"

#if defined (BOARD_R2)

uint8_t Board::getEncoderPair(uint8_t buttonIndex)
{
    uint8_t row = buttonIndex/NUMBER_OF_BUTTON_COLUMNS;

    if (row%2)
        row -= 1;   //uneven row, get info from previous (even) row

    uint8_t column = buttonIndex % NUMBER_OF_BUTTON_COLUMNS;

    return (row*NUMBER_OF_BUTTON_COLUMNS)/2 + column;
}

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
#endif