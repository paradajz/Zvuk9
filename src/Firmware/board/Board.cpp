#include "Board.h"

Board::Board()
{
    //default constructor
}

void Board::init()
{
    initTimers();
    #ifdef BOARD_R1
    initEncoders();
    initButtons();
    #endif
    initLEDs();
    initPads();
};

Board board;