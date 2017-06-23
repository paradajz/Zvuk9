#pragma once

#include "../../board/Board.h"
#include "../buttons/Buttons.h"
#include "../pads/Pads.h"
#include "Config.h"
#include "DataTypes.h"

#include "Macros.h"
#include "strings/Strings.h"

#define SPACE_CHAR          32

extern U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

class LCD
{
    public:
    LCD();
    void init();
    bool update();
    void setDirectWriteState(bool state);
    void displayWelcomeMessage();
    void displayProgramAndScale(uint8_t program, uint8_t scale);
    void clearLine(uint8_t row);
    void clearLines(uint8_t startIndex, uint8_t endIndex = 0);

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
    void displayOnOffChange(onOff_t type, uint8_t functionState);
    void displayCurveChange(padCoordinate_t coordinate);
    void displayCClimitChange(padCoordinate_t coordinate, ccLimitType_t type, uint8_t ccValue);
    void displayCCchange(padCoordinate_t type, uint8_t ccValue);
    void displayMIDIchannelChange();
    void displayActivePadNotes();
    void displayActiveOctave();
    void displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value);
    void displayEditModeNotAllowed(padEditModeResult_t errorType);
    void displayNoNotesError();
    void displayPadEditMode();
    void displayOutOfRange();
    void displayMaxNotesSet();
    void displayPadReleaseError(padReleaseError_t error);
    void displayPadEditChangeParametersError();
    void displayPad();
    void displayMIDIchannel();
    void displayFactoryResetConfirm();
    void displayFactoryResetStart();
    void displayFactoryResetEnd();
    void displayNoteShiftLevel(int8_t level);
    void displayNoteUpDown(bool state, int8_t shiftLevel = 0);

    void displayScrollCalibrationStatus(bool status);
    void displayPressureCalibrationTime(uint8_t seconds, uint8_t pressureZone, bool done);
    void displayPressureCalibrationStatus(bool status);

    void setMessageTime(int32_t msgTime);

    uint8_t getTextCenter(uint8_t textSize);

    //menu functions
    void displayDeviceInfo();

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

    protected:
    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size);

    private:
    messageStatus_t getMessageStatus();
    void displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite);
    void displayMessage(uint8_t row, const char *message);
    void displayPadAmount(uint8_t padNumber);
    void setupLCDlayout();
    void removeMessage();

    uint32_t messageDisplayTime;
    uint32_t lastLCDupdateTime;

    bool directWrite;

    bool displayMessage_var;
    bool messageActivated;

    char lcdLine[LCD_HEIGHT][MAX_TEXT_SIZE+1];
    char lcdLineMessage[LCD_HEIGHT][MAX_TEXT_SIZE+1];
    //warning! this assumes that max text length is max 32 characters
    uint32_t charChange[LCD_HEIGHT];

    int32_t messageTime;

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
