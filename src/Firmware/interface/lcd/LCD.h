#pragma once

#include "../../board/Board.h"
#include "../buttons/Buttons.h"
#include "../pads/Pads.h"
#include "Config.h"
#include "DataTypes.h"
#include "Layout.h"
#include "Macros.h"
#include "strings/Strings.h"

extern U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

class LCD
{
    public:
    LCD();
    void init();
    bool update();

    void setDirectWriteState(bool state);

    void displayWelcomeMessage();

    void setupHomeScreen();
    void setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh = false);
    void setupCalibrationScreen(padCoordinate_t coordinate, bool scrollCalibration);

    void displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift);
    void displayPad(uint8_t pad = 255);
    void displayActivePadNotes(bool showNotes = true);
    void displayVelocity(uint8_t velocity = 255, int16_t rawPressure = 10000);
    void displayAftertouch(uint8_t aftertouch = 255);
    void displayXYvalue(padCoordinate_t type, midiMessageType_t messageType = midiMessageInvalidType, int16_t value1 = 10000, int16_t value2 = 10000);
    void displayMIDIchannel(uint8_t channel = 255);
    void clearPadPressData();

    void displayChangeResult(function_t function, int16_t value, settingType_t type);
    void displayError(function_t function, changeResult_t result);

    void clearAll();
    void clearLine(uint8_t row, bool writeToDisplay = true);

    void displayFirmwareUpdated();
    void displayFactoryResetConfirm();
    void displayFactoryResetStart();
    void displayFactoryResetEnd();

    void displayPressureCalibrationTime(uint8_t seconds, uint8_t pressureZone, bool done);
    void displayPressureCalibrationStatus(bool status);

    void setMessageTime(int32_t msgTime);
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
    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, uint8_t size);

    private:
    messageStatus_t getMessageStatus();
    void displayText(uint8_t row, const char *text, uint8_t startIndex);
    void displayMessage(uint8_t row, const char *message, uint8_t startIndex);
    void removeMessage();
    uint8_t getTextCenter(uint8_t textSize);

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

    screen_t activeScreen;
};

extern LCD display;
