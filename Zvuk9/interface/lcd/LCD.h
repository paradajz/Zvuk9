#ifndef LCD_H_
#define LCD_H_

#include "Arduino.h"
#include "../../Types.h"
#include "../../hardware/lcd/lcd.h"
#include "LCDsettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../Debug.h"
#include "Macros.h"
#include "strings/Strings.h"

class LCD   {

    public:
    LCD();
    void init();
    void update();
    void displayHelloMessage();
    void displayProgramAndPreset(uint8_t program, uint8_t preset);

    void displayVelocity(uint8_t velocity);
    void displayAftertouch(uint8_t afterTouch);
    void displayXYposition(uint8_t position, ccType_t type);
    void displayXYcc(uint8_t ccXY, ccType_t type);

    void clearVelocity();
    void clearAftertouch();
    void clearXYposition(ccType_t type);
    void clearXYcc(ccType_t type);

    void displayTransportControl(transportControl_t type, bool state);
    void displayOnOff(functionsOnOff_t messageType, splitState_t _splitState, uint8_t functionState, uint8_t padNumber);
    void displayCurveChange(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t type, uint8_t padNumber);
    void displayCClimitChange(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayCCchange(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayMIDIchannelChange(uint8_t channel);
    void displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes);
    void displayActiveOctave(int8_t octave);
    void displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value);
    void displayEditModeNotAllowed(padEditModeResult_t errorType);
    void displayPadEditMode(uint8_t padNumber);
    void clearPadEditMode();
    void displayServiceMenu();
    void changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption);
    void selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption);
    void displayModifierEnabled();
    void displayReset();
    void displayOutOfRange();
    void displayMaxNotesSet();
    void displayPadReleaseError(padReleaseError_t error);
    void displayPadEditChangeParametersError();
    void displayPad(uint8_t pad);
    void clearPad();
    private:
    messageStatus_t getMessageStatus();
    void displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite);
    void displayMessage(uint8_t row, const char *message);
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

    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite);
    String string_line;
    char char_line[MAX_TEXT_SIZE+1];
    char nameBuffer[MAX_TEXT_SIZE+1];

};

extern LCD display;

#endif
