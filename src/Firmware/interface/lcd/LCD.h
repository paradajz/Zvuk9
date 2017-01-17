#pragma once

#include "../../board/Board.h"
#include "../buttons/Buttons.h"
#include "../pads/Pads.h"
#include "LCDsettings.h"

#ifdef DEBUG
#include "../../vserial/Serial.h"
#endif

#include "Macros.h"
#include "strings/Strings.h"
#include <string.h>
#include <stdlib.h>

typedef enum
{
    noMessage,
    showMessage,
    messageDisplayed,
    clearMessage
} messageStatus_t;

typedef enum
{
    userMenu,
    serviceMenu,
    noMenu
} menuType_t;

typedef enum
{
    text,
    message
} lcdTextType;

#define SPACE_CHAR          32

class LCD
{
    public:
    LCD();
    void init();
    void update();
    void displayHelloMessage();
    void displayProgramAndScale(uint8_t program, uint8_t scale);
    void clearLine(uint8_t row);

    void setScrollStart(uint8_t row, uint8_t index);

    void displayVelocity(uint8_t velocity);
    void displayAftertouch(uint8_t afterTouch);
    void displayXYposition(uint8_t position, padCoordinate_t type);
    void displayXYcc(uint8_t ccXY, padCoordinate_t type);

    void clearVelocity();
    void clearAftertouch();
    void clearXYposition(padCoordinate_t type);
    void clearXYcc(padCoordinate_t type);
    void clearPadEditMode();
    void clearPad();
    void clearMIDIchannel();
    void clearPadData();

    void displayFirmwareUpdated();

    void displayTransportControl(transportControl_t type);
    void displayOnOff(onOff_t type, bool _splitState, uint8_t functionState, uint8_t padNumber);
    void displayCurveChange(padCoordinate_t coordinate, bool _splitState, int8_t curveValue, uint8_t padNumber);
    void displayCClimitChange(padCoordinate_t coordinate, ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayCCchange(padCoordinate_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayMIDIchannelChange(uint8_t channel, bool _splitState, uint8_t padNumber);
    void displayActivePadNotes(uint8_t notes[], int8_t octaves[], uint8_t numberOfNotes, bool padEditMode);
    void displayActiveOctave(int8_t octave);
    void displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value);
    void displayEditModeNotAllowed(padEditModeResult_t errorType);
    void displayNoNotesError();
    void displayPadEditMode(uint8_t padNumber);
    void displayOutOfRange();
    void displayMaxNotesSet();
    void displayPadReleaseError(padReleaseError_t error);
    void displayPadEditChangeParametersError();
    void displayPad(uint8_t pad);
    void displayMIDIchannel(uint8_t channel);
    void displayFactoryResetWarning();
    void displayNoteShiftLevel(int8_t level);
    void displayNoteUpDown(bool state, int8_t shiftLevel = 0);

    inline uint8_t getNumberOfDigits(int32_t number)
    {
        if (number < 10)            return 1;
        if (number < 100)           return 2;
        if (number < 1000)          return 3;
        if (number < 10000)         return 4;
        if (number < 100000)        return 5;
        if (number < 1000000)       return 6;
        if (number < 10000000)      return 7;
        if (number < 100000000)     return 8;
        if (number < 1000000000)    return 9;

        return 10; //max size
    }

    //menu functions
    void displayDeviceInfo();

    //string manipulation
    inline void addNumberToCharArray(int32_t number, uint8_t &stringSize)
    {
        char intToCharArray[7];
        itoa(number, intToCharArray, 10);
        stringSize += getNumberOfDigits(number);
        if (number < 0) stringSize++;
        strcat(stringBuffer, intToCharArray);
        stringBuffer[stringSize] = '\0';
    }

    inline void appendText(const char *text, uint8_t &stringSize)
    {
        stringSize += strlen(text);
        strcat(stringBuffer, text);
        stringBuffer[stringSize] = '\0';
    }

    inline void addSpaceToCharArray(uint8_t &stringSize, uint8_t numberOfSpaces)
    {
        for (int i=0; i<numberOfSpaces; i++)
            stringBuffer[stringSize+i] = SPACE_CHAR;

        stringSize += numberOfSpaces;
        stringBuffer[stringSize] = '\0';
    }

    protected:
    char stringBuffer[MAX_TEXT_SIZE+1];
    char tempBuffer[MAX_TEXT_SIZE+1];
    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size, bool endOfLine = false);

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

    uint8_t scrollStartIndex[NUMBER_OF_LCD_ROWS];

    char lcdLine[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineMessage[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lastLCDLine[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];
    char lcdLineScroll[NUMBER_OF_LCD_ROWS][MAX_TEXT_SIZE+1];

    typedef struct
    {
        uint8_t row;
        uint8_t startIndex;
    } lcdElement_t;

    typedef struct
    {
        lcdElement_t ccx;
        lcdElement_t ccy;
        lcdElement_t xpos;
        lcdElement_t ypos;
        lcdElement_t velocity;
        lcdElement_t aftertouch;
        lcdElement_t padNumber;
        lcdElement_t midiChannel;
        lcdElement_t programAndScale;
        lcdElement_t notes;
        lcdElement_t activeOctave;
        lcdElement_t messageText1;
        lcdElement_t messageText2;
        lcdElement_t padCalibration;
        lcdElement_t noteShiftLevel;
    } lcdElements_t;

    lcdElements_t lcdElements;
};

extern LCD display;
