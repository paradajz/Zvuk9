#include "LCD.h"

#ifdef LCD_H_
#include "../../Scales.h"
#include "../pads/Pads.h"

#define PROGRAM_PRESET_ROW  0
#define PAD_NOTE_ROW        1
#define PAD_V_XY_AT_ROW     2
#define XY_CC_PAD_ROW       3

#define X_COORDINATE_START  5
#define Y_COORDINATE_START  10
#define AFTERTOUCH_START    15
#define PAD_NUMBER_START    15
#define VELOCITY_START      0
#define CC_X_START          0
#define CC_Y_START          7

#define SPACE_CHAR          32

void LCD::displayHelloMessage() {

    strcpy_P(nameBuffer, welcome_string);

    uint8_t charIndex = 0;
    while (nameBuffer[charIndex] != '\0')   {

        //write directly to screen
        lcd_putc(nameBuffer[charIndex]);
        newDelay(75);
        charIndex++;

    }

    strcpy(lastLCDLine_char[0], nameBuffer);
    newDelay(250);

}


//messages

void LCD::displayOutOfRange()    {

    strcpy_P(nameBuffer, outOfRange_string);
    string_line = nameBuffer;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

void LCD::displayModifierEnabled()   {

    strcpy_P(nameBuffer, modifierEnabled_string);
    string_line = nameBuffer;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

void LCD::displayMIDIchannelChange(uint8_t channel) {

    strcpy_P(nameBuffer, midiChannel_string);
    string_line = nameBuffer;
    string_line += channel;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

void LCD::displayCCchange(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)   {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    string_line = nameBuffer;
    string_line += ccValue;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, (_splitState != splitOff) ? padAmountSingle_string : padAmountAll_string);
    string_line = nameBuffer;

    if (_splitState != splitOff)
    string_line += padNumber; //local change

    updateDisplay(2, message, 0, true);

}

void LCD::displayCurveChange(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t type, uint8_t padNumber)  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    string_line = nameBuffer;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveNameArray[type])));
    string_line += nameBuffer;

    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, (_splitState != splitOff) ? padAmountSingle_string : padAmountAll_string);
    string_line = nameBuffer;

    if (_splitState != splitOff)
    string_line += padNumber; //local change

    updateDisplay(2, message, 0, true);

}

void LCD::displayCClimitChange(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));

    string_line = nameBuffer;
    string_line += ccValue;

    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, (_splitState != splitOff) ? padAmountSingle_string : padAmountAll_string);
    string_line = nameBuffer;

    if (_splitState != splitOff)
        string_line += padNumber; //local change

    updateDisplay(2, message, 0, true);

}

void LCD::displayOnOff(functionsOnOff_t messageType, splitState_t _splitState, uint8_t functionState, uint8_t padNumber)  {

    switch(messageType) {

        case featureNotes:
        case featureAftertouch:
        case featureX:
        case featureY:
        if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offArray[messageType])));
        else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onArray[messageType])));
        string_line = nameBuffer;
        updateDisplay(1, message, 0, true);

        strcpy_P(nameBuffer, (_splitState != splitXYFunctions) ? padAmountAll_string : padAmountSingle_string);
        string_line = nameBuffer;

        if (_splitState == splitXYFunctions)
            string_line += padNumber; //local change

        updateDisplay(2, message, 0, true);
        break;

        case featureSplit:
        switch(_splitState) {

            case splitOff:
            case splitXY:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[_splitState])));
            string_line = nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, emptyLine_string);
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            case splitXYFunctions:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXY])));
            string_line = nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXYFunctions])));
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            default:
            break;

        }
        break;

        case featureAftertouchType:
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(aftertouchTypeArray[functionState])));
        string_line = nameBuffer;
        updateDisplay(1, message, 0, true);
        strcpy_P(nameBuffer, emptyLine_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;

        default:
        break;

    }

}

void LCD::displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value) {

    switch(type)    {

        case tonicChange:
        switch(result)  {

            case outputChanged:
            case noChange:
            strcpy_P(nameBuffer, tonic_string);
            string_line = nameBuffer;
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            string_line += nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, emptyLine_string);
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            case outOfRange:
            strcpy_P(nameBuffer, outOfRange_string);
            string_line = nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, emptyLine_string);
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            default:
            return;

        }
        break;

        case octaveChange:
        //always display active octave, ignore out of range here
        strcpy_P(nameBuffer, octave_string);
        string_line = nameBuffer;
        string_line += value;
        updateDisplay(1, message, 0, true);
        strcpy_P(nameBuffer, emptyLine_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;

        case noteShift:
        switch (result) {

            case outputChanged:
            strcpy_P(nameBuffer, value ? noteUp_string : noteDown_string);
            string_line = nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, emptyLine_string);
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            case outOfRange:
            strcpy_P(nameBuffer, outOfRange_string);
            string_line = nameBuffer;
            updateDisplay(1, message, 0, true);
            strcpy_P(nameBuffer, emptyLine_string);
            string_line = nameBuffer;
            updateDisplay(2, message, 0, true);
            break;

            default:
            return;

        }
        break;

        default:
        break;

    }

}

void LCD::displayEditModeNotAllowed(padEditModeResult_t errorType)   {

    switch (errorType)  {

        case notUserPreset:
        strcpy_P(nameBuffer, editModeEnterError0_string);
        string_line = nameBuffer;
        updateDisplay(1, message, 0, true);

        strcpy_P(nameBuffer, editModeEnterError1_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;

        case padNotReleased:
        displayPadReleaseError(enterPadEditMode);
        break;

        default:
        return;

    }

}

void LCD::displayTransportControl(transportControl_t type, bool state)  {

    switch(type)    {

        case transportRecord:
        if (state) strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[2])));
        else strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[3])));
        string_line = nameBuffer;
        break;

        case transportPlay:
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[0])));
        string_line = nameBuffer;
        break;

        case transportStop:
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[1])));
        string_line = nameBuffer;
        break;

    }

    updateDisplay(1, message, 0, true);
    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

void LCD::displayMaxNotesSet()   {

    strcpy_P(nameBuffer, maxNotesSet0_string);
    string_line = nameBuffer;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, maxNotesSet1_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

void LCD::displayPadReleaseError(padReleaseError_t error)    {

    strcpy_P(nameBuffer, relasePad_string);
    string_line = nameBuffer;
    updateDisplay(1, message, 0, true);

    switch(error)   {

        case changeParameters:
        strcpy_P(nameBuffer, changeParameters_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;

        case changeTonic:
        strcpy_P(nameBuffer, changeTonic_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;

        case enterPadEditMode:
        strcpy_P(nameBuffer, enterPadEditMode_string);
        string_line = nameBuffer;
        updateDisplay(2, message, 0, true);
        break;
        break;

        default:
        return;

    }

}

void LCD::displayPadEditChangeParametersError()  {

    strcpy_P(nameBuffer, exitPadMode_string);
    string_line = nameBuffer;
    updateDisplay(1, message, 0, true);

    strcpy_P(nameBuffer, changeParameters_string);
    string_line = nameBuffer;
    updateDisplay(2, message, 0, true);

}

//text

void LCD::displayProgramAndPreset(uint8_t program, uint8_t preset)   {

    //program and preset are displayed in single row
    strcpy_P(nameBuffer, program_string);
    string_line = nameBuffer;
    string_line += program;

    string_line += " ";

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        string_line += nameBuffer;

        }   else {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(presetNameArray[NUMBER_OF_PREDEFINED_SCALES])));
        string_line += nameBuffer;
        string_line += (preset - NUMBER_OF_PREDEFINED_SCALES + 1);

    }

    updateDisplay(PROGRAM_PRESET_ROW, text, 0, true);

}



void LCD::displayVelocity(uint8_t velocity)  {

    strcpy_P(nameBuffer, velocity_string);
    string_line = nameBuffer;
    string_line += velocity;
    if (velocity < 10)         string_line += "  ";
    else if (velocity < 100)   string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW, text, 0, false);

}

void LCD::displayAftertouch(uint8_t afterTouch)  {

    strcpy_P(nameBuffer, aftertouch_string);
    string_line = nameBuffer;
    string_line += afterTouch;
    if (afterTouch < 10)       string_line += "  ";
    else if (afterTouch < 100) string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW, text, AFTERTOUCH_START, false);

}

void LCD::displayXYposition(uint8_t position, ccType_t type)   {

    uint8_t lcdCoordinate = 0;

    switch(type)    {

        case ccTypeX:
        strcpy_P(nameBuffer, x_string);
        lcdCoordinate = X_COORDINATE_START;
        break;

        case ccTypeY:
        strcpy_P(nameBuffer, y_string);
        lcdCoordinate = Y_COORDINATE_START;
        break;

    }

    string_line = nameBuffer;
    string_line += position;

    if (position < 10)         string_line += "  ";
    else if (position < 100)   string_line += " ";

    string_line += " ";


    updateDisplay(PAD_V_XY_AT_ROW, text, lcdCoordinate, false);

}

void LCD::displayXYcc(uint8_t ccXY, ccType_t type)   {

    uint8_t lcdCoordinate = 0;

    switch(type)    {

        case ccTypeX:
        strcpy_P(nameBuffer, xCCid_string);
        lcdCoordinate = CC_X_START;
        break;

        case ccTypeY:
        strcpy_P(nameBuffer, yCCid_string);
        lcdCoordinate = CC_Y_START;
        break;

    }

    string_line = nameBuffer;
    string_line += ccXY;

    if (ccXY < 10)         string_line += "  ";
    else if (ccXY < 100)   string_line += " ";

    string_line += " ";

    updateDisplay(XY_CC_PAD_ROW, text, lcdCoordinate, false);

}

void LCD::displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes)  {

    if (!numberOfNotes) {

        strcpy_P(nameBuffer, emptyLine_string);
        string_line = nameBuffer;
        updateDisplay(1, text, 0, true);
        //we should also clear velocity if we aren't in pad edit mode
        if (!pads.editModeActive()) {

            strcpy_P(nameBuffer, velocityClear_string);
            string_line = nameBuffer;
            updateDisplay(2, text, 0, false);

        }

    }   else

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octaves[i];
        string_line += " ";
        updateDisplay(1, text, 0, true);
    }

}


void LCD::clearVelocity()   {

    strcpy_P(nameBuffer, velocityClear_string);
    string_line = nameBuffer;

    updateDisplay(PAD_V_XY_AT_ROW, text, VELOCITY_START, false);

}

void LCD::clearAftertouch() {

    strcpy_P(nameBuffer, aftertouchClear_string);
    string_line = nameBuffer;

    updateDisplay(PAD_V_XY_AT_ROW, text, AFTERTOUCH_START, false);

}

void LCD::clearXYposition(ccType_t type)    {

    strcpy_P(nameBuffer, xyPositionClear_string);
    string_line = nameBuffer;

    switch(type)    {

        case ccTypeX:
        updateDisplay(PAD_V_XY_AT_ROW, text, X_COORDINATE_START, false);
        break;

        case ccTypeY:
        updateDisplay(PAD_V_XY_AT_ROW, text, Y_COORDINATE_START, false);
        break;

    }

}

void LCD::clearXYcc(ccType_t type)  {

    strcpy_P(nameBuffer, xyCCclear_string);
    string_line = nameBuffer;

    switch(type)    {

        case ccTypeX:
        updateDisplay(XY_CC_PAD_ROW, text, CC_X_START, false);
        break;

        case ccTypeY:
        updateDisplay(XY_CC_PAD_ROW, text, CC_Y_START, false);
        break;

    }

}

void LCD::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    strcpy_P(nameBuffer, activeOctave_string);
    string_line = nameBuffer;
    string_line += octave;

    updateDisplay(2, text, 0, true);

}

void LCD::displayPadEditMode(uint8_t padNumber)  {

    strcpy_P(nameBuffer, editingPad_string);
    string_line = nameBuffer;
    string_line += padNumber;
    updateDisplay(0, text, 0, true);
    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    updateDisplay(1, text, 0, true);
    updateDisplay(2, text, 0, true);
    updateDisplay(3, text, 0, true);

}

void LCD::displayPad(uint8_t pad)   {

    strcpy_P(nameBuffer, padAmountSingle_string);
    string_line = nameBuffer;
    string_line += pad;

    updateDisplay(XY_CC_PAD_ROW, text, PAD_NUMBER_START, false);

}

void LCD::clearPad()    {

    strcpy_P(nameBuffer, padClear_string);
    string_line = nameBuffer;

    updateDisplay(XY_CC_PAD_ROW, text, PAD_NUMBER_START, false);

}

void LCD::clearPadEditMode()    {

    #if MODE_SERIAL > 0
        Serial.println(F("Clearing pad edit mode from LCD"));
    #endif

    strcpy_P(nameBuffer, emptyLine_string);
    string_line = nameBuffer;
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true);

}

void LCD::displayServiceMenu()  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(menu_types[serviceMenu])));
    string_line = nameBuffer;
    updateDisplay(0, text, 0, true);

    for (int i=0; i<(int16_t)progmemArraySize(service_menu_options); i++)    {

        (!i) ? string_line = ">" : string_line = " ";
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
        string_line += nameBuffer;
        updateDisplay(i+1, text, 0, true);

    }

}

void LCD::changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption) {

    //we can display up to three options/suboptions at the time
    uint8_t markerOption = (option > 2) ? (NUMBER_OF_LCD_ROWS-1) : option;

    switch(type)    {

        case serviceMenu:
        for (int i=0; i<(int)progmemArraySize(service_menu_options); i++)    {

            if (i == markerOption)  {

                string_line = ">";
                strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
                string_line += nameBuffer;
                updateDisplay(i+1, text, 0, true);

                }   else {

                strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
                string_line = " ";
                string_line += nameBuffer;
                updateDisplay(i+1, text, 0, true);

            }

        }
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

}

void LCD::selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption)  {



}


//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite)    {

    uint8_t size = string_line.length();
    string_line.toCharArray(char_line, size+1, 0);
    char_line[size] = '\0';

    switch(type)    {

        case text:
        display.displayText(row, char_line, startIndex, overwrite);
        break;

        case message:
        display.displayMessage(row, char_line);
        break;

        default:
        break;

    }

}
#endif