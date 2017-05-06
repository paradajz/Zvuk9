#include "../Board.h"

volatile uint8_t    buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];

uint8_t Board::getButtonState(uint8_t buttonIndex)
{
    return buttonDebounceCounter[buttonIndex];
}