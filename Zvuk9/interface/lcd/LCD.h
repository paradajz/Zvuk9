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

#define PROGRAM_PRESET_ROW              0
#define PAD_NOTE_ROW                    1
#define PAD_V_XY_AT_ROW                 2
#define XY_ROW                          3

#define LCD_AFTERTOUCH_POSITION         12
#define LCD_AFTERTOUCH_LENGHT           8

class LCD   {

    public:
    LCD();
    void init();
    void update();
    void displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex = 0);
    void displayMessage(uint8_t row, const char *message, bool stayOn = false);

    void clearPadData();

    void displayHelloMessage();
    void displayEditModeNotAllowed(padEditError_t errorType);

    void displayOctaveChange(uint8_t octave);
    void displayPadEditMode(uint8_t padNumber);
    void clearPadEditMode();

    void clearMessage(bool forceClear = false);

    void displayServiceMenu();
    void changeMenuOption(menuType_t type, uint8_t option, uint8_t suboption);
    void selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption);

    private:
    void clearRow(uint8_t rowNumber);
    void expandLine(uint8_t lineNumber, lcdLineType_t lineType);
    bool checkClearScreen();

    char nameBuffer[MAX_TEXT_LENGTH];

    uint32_t messageDisplayTime;
    uint32_t lastScrollTime;
    bool displayMessage_var;
    bool messageActivated;
    int8_t scrollIndex;
    uint32_t lastRefreshTime;
    bool keepMessage;
    bool restoreMessage;

    bool _clearPadData;
    uint32_t lastPadDataClearTime;
    uint32_t lastLCDupdateTime;

    String lcdLine[NUMBER_OF_LCD_ROWS];
    String lcdLineMessage[NUMBER_OF_LCD_ROWS];
    String lcdLineScroll[NUMBER_OF_LCD_ROWS];

    String lastLCDLine[NUMBER_OF_LCD_ROWS];
    String lastLCDmessage[NUMBER_OF_LCD_ROWS];

    String  tempLine1,
            tempLine2;

    bool lineChange[NUMBER_OF_LCD_ROWS];
    bool scrollEnabled[NUMBER_OF_LCD_ROWS];
    bool scrollDirection[NUMBER_OF_LCD_ROWS];

};

extern LCD lcDisplay;

#endif
