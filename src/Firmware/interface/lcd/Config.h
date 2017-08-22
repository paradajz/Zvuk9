#pragma once

#include "../pads/Config.h"

#define LCD_MESSAGE_DURATION                1500

#define LCD_REFRESH_TIME                    PAD_NOTE_SEND_DELAY

#define LCD_SCROLL_TIME                     1000

#define LCD_PAD_DATA_CLEAR_DELAY            300

#define LCD_WIDTH                           32
#define LCD_HEIGHT                          4

const uint8_t rowMap[LCD_HEIGHT] =
{
    0,
    2,
    4,
    6
};
