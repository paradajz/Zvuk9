#include "Board.h"

Board::Board()
{
    //default constructor
}

void Board::init()
{
    initTimers();
    initEncoders();
    initButtons();
    initLEDs();
    initPads();
};

Board board;