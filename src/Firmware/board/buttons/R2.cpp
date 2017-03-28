#include "../Board.h"

#ifdef BOARD_R2
bool buttonsProcessed;

volatile uint8_t    buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];
uint8_t             inputBuffer[NUMBER_OF_BUTTON_COLUMNS];

bool Board::buttonDataAvailable()
{
    return true;
}


uint8_t Board::getButtonState(uint8_t buttonIndex)
{
    return buttonDebounceCounter[buttonIndex];
}
#endif