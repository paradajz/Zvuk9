#ifndef LCD_H_
#define LCD_H_

#include "Arduino.h"
#include "../../Types.h"
#include "../../hardware/lcd/lcd.h"
#include "LCDsettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../Debug.h"

class LCD   {

    public:
    LCD();
    void init();
    void update();
    void displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite);
    void displayMessage(uint8_t row, const char *message);

    private:
    messageStatus_t getMessageStatus();
    void checkScroll(uint8_t row);

    uint32_t messageDisplayTime;
    uint32_t lastScrollTime;
    uint32_t lastLCDupdateTime;

    bool displayMessage_var;
    bool lineChange[NUMBER_OF_LCD_ROWS];
    bool scrollEnabled[NUMBER_OF_LCD_ROWS];
    bool scrollDirection[NUMBER_OF_LCD_ROWS];
    bool messageActivated;

    int8_t scrollIndex[NUMBER_OF_LCD_ROWS];

    char lcdLine_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineMessage_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lastLCDLine_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineScroll_char[MAX_TEXT_SIZE+1];

};

extern LCD lcDisplay;

#endif
