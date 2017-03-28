#include "Board.h"

void Board::init()
{
    initPins();
    initTimers();
    #ifdef BOARD_R1
    initEncoders();
    initButtons();
    #endif
    initPads();
};