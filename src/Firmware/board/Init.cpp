#include "Board.h"

void Board::init()
{
    initPins();
    initTimers();
    setUpADC();
    initPads();
};