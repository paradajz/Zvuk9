#ifndef LCDSETTINGS_H_
#define LCDSETTINGS_H_

#include "../pads/Calibration.h"

#define LCD_MESSAGE_DURATION            1500

//sync refresh time with this setting to show x/y and notes at the same time
#define LCD_REFRESH_TIME                PAD_NOTE_SEND_DELAY

#define LCD_SCROLL_TIME                 500

#define LCD_PAD_DATA_CLEAR_DELAY        300

#define NUMBER_OF_LCD_COLUMNS           20
#define NUMBER_OF_LCD_ROWS              4

#define MAX_TEXT_SIZE                   (NUMBER_OF_LCD_COLUMNS+10)

#endif