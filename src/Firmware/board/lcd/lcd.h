#pragma once

#ifdef BOARD_R1
#include "hd44780/lcd.h"
#elif defined (BOARD_R2)
#include "u8glib/src/u8g.h"
#endif