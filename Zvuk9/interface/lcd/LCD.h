#ifndef LCD_H_
#define LCD_H_

#include "Arduino.h"
#include "../../eeprom/EEPROMsettings.h"
#include "../../Types.h"
#include "../../midi/MIDI_parameters.h"
#include "../../hardware/lcd/hd44780.h"
#include "LCDsettings.h"
#include "LCDstrings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"

#define NUMBER_OF_LCD_COLUMNS           20
#define NUMBER_OF_LCD_ROWS              4

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

    void setProgram(uint8_t preset);
    void setPreset(uint8_t program);
    void updateNote(uint8_t pad, uint8_t *note, uint8_t *octave, uint8_t numberOfNotes, uint8_t velocity);
    void clearPadData();
    void setXYData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
    void updateAfterTouch(uint8_t afterTouch);
    void setCCData(uint8_t pad, uint8_t x, uint8_t y);

    void displayOnOffMessage(functionsOnOff_t messageType, splitState_t _splitState, bool functionState, uint8_t padNumber);
    void displayCurveChangeMessage(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t curveValue, uint8_t padNumber);
    void displayCClimitChangeMessage(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayCCchangeMessage(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayTransportControlMessage(transportControl_t type, bool state);
    void displayHelloMessage();
    void displayEditModeNotAllowed(padEditError_t errorType);
    void displayPadEditResult(changeOutput_t result);

    void displayNoteChange(changeOutput_t result, changeType_t type, int8_t value);
    void displayOctaveChange(uint8_t octave);
    void displayPadEditMode(uint8_t padNumber);
    void clearPadEditMode();

    void displayMIDIchannelChange(uint8_t channel);
    void displayActiveOctave(uint8_t octave);
    void displayActivePadNotes(uint8_t *notes, uint8_t *octaves);
    void displayUserMessage(String message);
    void displayServiceMenu();

    private:
    void clearRow(uint8_t rowNumber);
    void expandLine(uint8_t lineNumber, lcdLineType_t lineType);
    void resetData();
    void setProgramAndPreset();
    bool checkClearScreen();

    char nameBuffer[MAX_TEXT_LENGTH];

    uint32_t messageDisplayTime;
    uint32_t lastScrollTime;
    bool displayMessage;
    bool messageActivated;
    bool lcdUpdating;
    int8_t scrollIndex;
    uint32_t lastRefreshTime;

    int8_t  program,
            preset;

    bool _clearPadData;
    uint32_t lastPadDataClearTime;
    uint32_t lastLCDupdateTime;

    uint8_t currentPad;

    uint8_t lastCharPosition[NUMBER_OF_LCD_ROWS];

    String lcdLine[NUMBER_OF_LCD_ROWS];
    String lcdLineMessage[NUMBER_OF_LCD_ROWS];
    String lcdLineScroll[NUMBER_OF_LCD_ROWS];

    String lastLcdLine[NUMBER_OF_LCD_ROWS];

    String  tempLine1,
            tempLine2;

    bool lineChange[NUMBER_OF_LCD_ROWS];
    bool lineChangeBuffer[NUMBER_OF_LCD_ROWS];
    bool scrollEnabled[NUMBER_OF_LCD_ROWS];
    bool scrollDirection[NUMBER_OF_LCD_ROWS];

    int8_t ccX, ccY;
    int8_t lastDisplayedPad;

};

extern LCD lcDisplay;

#endif /* LCD_H_ */
