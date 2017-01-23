#pragma once

#ifdef BOARD_R1
#include "v1/Board.h"
#else
#error Wrong board revision selected
#endif