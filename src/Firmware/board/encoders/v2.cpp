#include "../Board.h"

#if defined (BOARD_R2)
bool encodersProcessed;

uint8_t Board::getEncoderPair(uint8_t buttonIndex)
{
    uint8_t row = buttonIndex/NUMBER_OF_BUTTON_COLUMNS;

    if (row%2)
        row -= 1;   //uneven row, get info from previous (even) row

    uint8_t column = buttonIndex % NUMBER_OF_BUTTON_COLUMNS;

    return (row*NUMBER_OF_BUTTON_COLUMNS)/2 + column;
}

bool Board::encoderDataAvailable()
{
    checkInputMatrixBufferCopy();

    bool returnValue = true;
    bool _dmBufferCopied = dmBufferCopied;

    if (!_dmBufferCopied)
        returnValue = copyInputMatrixBuffer();  //buffer isn't copied

    encodersProcessed = true;

    return returnValue;
}

int8_t Board::getEncoderState(uint8_t encoderNumber)
{
    uint8_t column = encoderNumber % NUMBER_OF_BUTTON_COLUMNS;
    uint8_t row  = (encoderNumber/NUMBER_OF_BUTTON_COLUMNS)*2;
    uint8_t shiftAmount = ((NUMBER_OF_BUTTON_COLUMNS-1)*8) - column*8;
    uint8_t pairState = inputMatrixBufferCopy >> shiftAmount;
    pairState = ((pairState >> row) & 0x03);

    return readEncoder(encoderNumber, pairState);
}
#endif