#include "../Board.h"

bool buttonsProcessed;

volatile uint8_t    buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];

bool Board::buttonDataAvailable()
{
    return true;
}

uint8_t Board::getButtonState(uint8_t buttonIndex)
{
    return buttonDebounceCounter[buttonIndex];
}