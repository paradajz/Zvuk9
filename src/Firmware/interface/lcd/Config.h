#pragma once

#include "../pads/Config.h"

///
/// \brief Length of temporary (message) text on display in milliseconds.
///
#define LCD_MESSAGE_DURATION                1500

///
/// \brief Time in milliseconds after text on display is being refreshed.
///
#define LCD_REFRESH_TIME                    PAD_NOTE_SEND_DELAY

///
/// \brief Time in milliseconds after which scrolling text moves on display.
///
#define LCD_SCROLL_TIME                     1000

///
/// \brief Maximum amount of characters displayed in single LCD row.
///
#define LCD_WIDTH                           32

///
/// \brief Number of LCD rows.
///
#define LCD_HEIGHT                          4

///
/// \brief Value by which actual octave is being subtracted when showing octave on display.
///
#define OCTAVE_NORMALIZATION                -2

///
/// \brief Array holding remapped values of LCD rows.
/// LCD actually contains 8 rows, however, to increase readability every second row is used.
///
const uint8_t rowMap[LCD_HEIGHT] =
{
    0,
    2,
    4,
    6
};
