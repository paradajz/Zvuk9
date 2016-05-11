#include "../../Modules.h"

#ifdef MODULE_LCD

#ifndef LCD_H_
#define LCD_H_

#include "../../Types.h"
#include "../../hardware/lcd/lcd.h"
#include "LCDsettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../Debug.h"

#if MODE_SERIAL > 0
#include "../../vserial/Serial.h"
#endif

#include "Macros.h"
#include "strings/Strings.h"
#include <string.h>
#include <stdlib.h>

#define SPACE_CHAR          32

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
    void clearPadEditMode();
    void clearPad();
    void clearMIDIchannel();
    void clearPadData();

    void displayTransportControl(transportControl_t type, bool state);
    void displayOnOff(functionsOnOff_t messageType, bool _splitState, uint8_t functionState, uint8_t padNumber);
    void displayCurveChange(curveCoordinate_t coordinate, bool _splitState, curveType_t type, uint8_t padNumber);
    void displayCClimitChange(ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayCCchange(ccType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayMIDIchannelChange(uint8_t channel, bool _splitState, uint8_t padNumber);
    void displayActivePadNotes(uint8_t notes[], int8_t octaves[], uint8_t numberOfNotes, bool padEditMode);
    void displayActiveOctave(int8_t octave);
    void displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value);
    void displayEditModeNotAllowed(padEditModeResult_t errorType);
    void displayPadEditMode(uint8_t padNumber);

    void displayServiceMenu();
    void changeMenuOption(menuType_t type, int8_t *menuHierachy);
    void changeMenuScreen(menuType_t type, int8_t *menuHierachy);

    void displayModifierEnabled();
    void displayOutOfRange();
    void displayMaxNotesSet();
    void displayPadReleaseError(padReleaseError_t error);
    void displayPadEditChangeParametersError();
    void displayPad(uint8_t pad);
    void displayDFUmode();
    void displayMIDIchannel(uint8_t channel);
    private:
    messageStatus_t getMessageStatus();
    void displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite, bool endOfLine = false);
    void displayMessage(uint8_t row, const char *message);
    void checkScroll(uint8_t row);
    void displayPadAmount(bool singlePad, uint8_t padNumber);
    void setupLCDlayout();

    uint32_t messageDisplayTime;
    uint32_t lastScrollTime;
    uint32_t lastLCDupdateTime;

    bool displayMessage_var;
    bool lineChange[NUMBER_OF_LCD_ROWS];
    bool scrollEnabled[NUMBER_OF_LCD_ROWS];
    bool scrollDirection[NUMBER_OF_LCD_ROWS];
    bool messageActivated;

    int8_t scrollIndex[NUMBER_OF_LCD_ROWS];

    uint8_t scrollStartIndex[NUMBER_OF_LCD_ROWS],
            scrollEndIndex[NUMBER_OF_LCD_ROWS];

    char lcdLine_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineMessage_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lastLCDLine_char[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineScroll_char[MAX_TEXT_SIZE+1];

    typedef struct {

        uint8_t row;
        uint8_t startIndex;

    } lcdElement_t;

    typedef struct {

        lcdElement_t ccx;
        lcdElement_t ccy;
        lcdElement_t xpos;
        lcdElement_t ypos;
        lcdElement_t velocity;
        lcdElement_t aftertouch;
        lcdElement_t padNumber;
        lcdElement_t midiChannel;
        lcdElement_t programAndPreset;
        lcdElement_t notes;
        lcdElement_t activeOctave;
        lcdElement_t messageText1;
        lcdElement_t messageText2;

    } lcdElements_t;

    lcdElements_t lcdElements;

    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size, bool endOfLine = false);

};

extern LCD display;

#endif
#endif