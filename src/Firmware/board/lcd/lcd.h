#pragma once

#ifdef BOARD_R1
#include "hd44780/lcd.h"
#elif defined (BOARD_R2)
#include "U8g2/src/U8x8lib.h"
#endif