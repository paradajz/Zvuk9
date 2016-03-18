#ifndef MESSAGEBUILDER_H_
#define MESSAGEBUILDER_H_

//purpose of this class is to build strings which are going to be displayed on lcd

#include "LCD.h"
#include "LCDsettings.h"

#define MAX_TEXT_SIZE       (NUMBER_OF_LCD_COLUMNS*2)

#define PROGRAM_PRESET_ROW  0
#define PAD_NOTE_ROW        1
#define PAD_V_XY_AT_ROW     2
#define XY_ROW              3

class MessageBuilder {

    public:
    MessageBuilder();
    void displayProgramAndPreset(uint8_t program, uint8_t preset);
    void displayNotes(uint8_t note[], uint8_t octave[], uint8_t numberOfNotes);
    void displayVelocity(uint8_t velocity);
    void displayXYposition(uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
    void displayAftertouch(uint8_t afterTouch);
    void displayXYcc(uint8_t ccX, uint8_t ccY);
    void displayTransportControl(transportControl_t type, bool state);
    void displayOnOff(functionsOnOff_t messageType, splitState_t _splitState, bool functionState, uint8_t padNumber);
    void displayCurveChange(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t type, uint8_t padNumber);
    void displayCClimitChange(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayCCchange(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber);
    void displayMIDIchannelChange(uint8_t channel);
    void displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes);
    void displayActiveOctave(int8_t octave);
    void displayNoteChange(changeOutput_t result, changeType_t type, int8_t value);
    void displayEditModeNotAllowed(padEditModeResult_t errorType);
    void displayPadEditMode(uint8_t padNumber);
    void displayOctaveChange(uint8_t octave);
    void clearPadEditMode();
    void displayServiceMenu();
    void changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption);
    void selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption);
    void clearPadData();

    private:
    void updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite);
    String string_line;
    char char_line[MAX_TEXT_SIZE];
    char nameBuffer[MAX_TEXT_SIZE];

};

extern MessageBuilder messageBuilder;

#endif