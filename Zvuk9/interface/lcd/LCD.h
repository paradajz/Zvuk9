#ifndef LCD_H_
#define LCD_H_

#include "Arduino.h"
#include "../../Types.h"
#include "../../hardware/lcd/lcd.h"
#include "LCDsettings.h"
#include "strings/Strings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "Macros.h"
#include "../../Debug.h"

#define MAX_TEXT_LENGTH                 (NUMBER_OF_LCD_COLUMNS*2)

class LCD   {

    public:
    LCD();
    void init();
    void update();
    void displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex, bool overwrite);
    void displayMessage(uint8_t row, const char *message);

    private:
    void expandLine(uint8_t lineNumber, lcdLineType_t lineType);
    void checkMessage();
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

    String lcdLine[NUMBER_OF_LCD_ROWS];
    String lcdLineMessage[NUMBER_OF_LCD_ROWS];
    String lastLCDLine[NUMBER_OF_LCD_ROWS];
    String lastLCDmessage[NUMBER_OF_LCD_ROWS];
    String lcdLineScroll;

};

extern LCD lcDisplay;

#endif
