#pragma once

#include "../pads/Calibration.h"

#define LCD_MESSAGE_DURATION            1500

#ifdef BOARD_R1
//sync refresh time with this setting to show x/y and notes at the same time
#define LCD_REFRESH_TIME                PAD_NOTE_SEND_DELAY
#elif defined BOARD_R2
#define LCD_REFRESH_TIME                100
#endif

#define LCD_SCROLL_TIME                 300

#define LCD_PAD_DATA_CLEAR_DELAY        300

#ifdef BOARD_R1
#define LCD_WIDTH                       20
#define LCD_HEIGHT                      4
#elif defined (BOARD_R2)
#define LCD_WIDTH                       35
#define LCD_HEIGHT                      4
#define ROW_SPACING                     15
#endif

#ifdef BOARD_R1
#define MAX_TEXT_SIZE                   (LCD_WIDTH+10)
#elif defined (BOARD_R2)
#define MAX_TEXT_SIZE                   LCD_WIDTH
#endif

#define INFINITE_MESSAGE_TIME           -1