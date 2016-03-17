#ifndef LCD_H_
#define LCD_H_

#include "Arduino.h"
#include "../../eeprom/EEPROMsettings.h"
#include "../../Types.h"
#include "../../midi/MIDI_parameters.h"
#include "../../hardware/lcd/lcd.h"
#include "LCDsettings.h"
#include "LCDstrings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "Macros.h"

#define MAX_TEXT_LENGTH                 (NUMBER_OF_LCD_COLUMNS*2)

class LCD   {

    public:
    LCD();
    void init();
    void update();
    void displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex, bool overwrite);
    void displayMessage(uint8_t row, const char *message, bool stayOn = false);

    void displayHelloMessage();
    void clearMessage(bool forceClear = false);

    private:
    void expandLine(uint8_t lineNumber, lcdLineType_t lineType);

    uint32_t messageDisplayTime;
    uint32_t lastScrollTime;
    bool displayMessage_var;
    bool messageActivated;
    int8_t scrollIndex;
    uint32_t lastRefreshTime;
    bool keepMessage;
    bool restoreMessage;

    uint32_t lastLCDupdateTime;

    String lcdLine[NUMBER_OF_LCD_ROWS];
    String lcdLineMessage[NUMBER_OF_LCD_ROWS];
    String lcdLineScroll[NUMBER_OF_LCD_ROWS];

    String lastLCDLine[NUMBER_OF_LCD_ROWS];
    String lastLCDmessage[NUMBER_OF_LCD_ROWS];

    bool lineChange[NUMBER_OF_LCD_ROWS];
    bool scrollEnabled[NUMBER_OF_LCD_ROWS];
    bool scrollDirection[NUMBER_OF_LCD_ROWS];

};

extern LCD lcDisplay;

#endif
