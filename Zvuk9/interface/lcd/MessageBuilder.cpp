#include "MessageBuilder.h"
#include "menu/MenuStrings.h"

#define XY_POSITION_START               5
#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15

#define SPACE_CHAR                      32

MessageBuilder::MessageBuilder()    {

    string_line.reserve(MAX_TEXT_LENGTH);

}

void MessageBuilder::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite)    {

    string_line.toCharArray(char_line, string_line.length()+1, 0);

    switch(type)    {

        case text:
        lcDisplay.displayText(row, char_line, string_line.length(), startIndex, overwrite);
        break;

        case message_std:
        lcDisplay.displayMessage(row, char_line);
        break;

        default:
        break;

    }

}

void MessageBuilder::displayProgramAndPreset(uint8_t program, uint8_t preset)   {

    //program and preset are displayed in single row
    char_line[0] = 'P';
    string_line = "P";
    string_line += program;
    char program_buffer[3];
    itoa(program, program_buffer, 10);

    char_line[1] = program_buffer[0];
    if (program >= 10) char_line[2] = program_buffer[1];

    string_line += " ";

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        string_line += nameBuffer;

    }   else {

        string_line += "User preset ";
        string_line += (preset - NUMBER_OF_PREDEFINED_SCALES + 1);

    }

    updateDisplay(PROGRAM_PRESET_ROW, text, 0, true);

}

void MessageBuilder::displayNotes(uint8_t note[], uint8_t octave[], uint8_t numberOfNotes)    {

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[note[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octave[i];
        string_line += " ";

    }

    updateDisplay(PAD_NOTE_ROW, text, 0, true);

}

void MessageBuilder::displayVelocity(uint8_t velocity)  {

    string_line = "v";
    string_line += velocity;
    if (velocity < 10)         string_line += "  ";
    else if (velocity < 100)   string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW, text, 0, false);

}

void MessageBuilder::displayXYposition(uint8_t xPosition, uint8_t yPosition, bool xAvailable, bool yAvailable)   {

    //velocity, x, y and aftertouch are all in same line
    //special care needs to be taken here

    if (xAvailable && yAvailable)   {

        string_line = "x";
        string_line += xPosition;
        if (xPosition < 10)         string_line += "  ";
        else if (xPosition < 100)   string_line += " ";
        string_line += " ";
        string_line += "y";
        string_line += yPosition;
        if (yPosition < 10)         string_line += "  ";
        else if (yPosition < 100)   string_line += " ";

    }   else if (!xAvailable && yAvailable) {

            for (int i=0; i<4; i++)
                string_line[X_COORDINATE_START+i] = emptyLine[i];

            string_line += "y";
            string_line += yPosition;
            if (yPosition < 10)         string_line += "  ";
            else if (yPosition < 100)   string_line += " ";

    }   else if (xAvailable && !yAvailable) {

            string_line = "x";
            string_line += xPosition;
            if (xPosition < 10)         string_line += "  ";
            else if (xPosition < 100)   string_line += " ";

            for (int i=0; i<4; i++)
                string_line[Y_COORDINATE_START+i] = emptyLine[i];

    }

    updateDisplay(PAD_V_XY_AT_ROW, text, XY_POSITION_START, false);

}

void MessageBuilder::displayAftertouch(uint8_t afterTouch)  {

    string_line = "at";
    string_line += afterTouch;
    if (afterTouch < 10)       string_line += "  ";
    else if (afterTouch < 100) string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW, text, AFTERTOUCH_START, false);

}

void MessageBuilder::displayXYcc(uint8_t ccX, uint8_t ccY)   {

    //ccx string
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccIDarray[ccTypeX])));
    string_line = nameBuffer;

    //ccx value
    string_line += ccX;
    if (ccX < 10)       string_line += "  ";
    else if (ccX < 100) string_line += " ";
    string_line += " ";

    //ccy string
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccIDarray[ccTypeY])));
    string_line += nameBuffer;
    string_line += ccY;

    updateDisplay(XY_ROW, text, 0, true);

}

void MessageBuilder::displayTransportControl(transportControl_t type, bool state)  {

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

    updateDisplay(1, message_std, 0, true);
    string_line = emptyLine;
    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayOnOff(functionsOnOff_t messageType, splitState_t _splitState, bool functionState, uint8_t padNumber)  {

    switch(messageType) {

        case featureNotes:
        case featureAftertouch:
        case featureX:
        case featureY:
        if (_splitState == splitXYFunctions) {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offLocalArray[messageType])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onLocalArray[messageType])));

            string_line = nameBuffer;
            updateDisplay(1, message_std, 0, true);

            string_line = "pad ";
            string_line += padNumber;
            updateDisplay(2, message_std, 0, true);

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[messageType])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[messageType])));

            string_line = nameBuffer;
            updateDisplay(1, message_std, 0, true);

            string_line = "all pads";
            updateDisplay(2, message_std, 0, true);

        }
        break;

        case featureSplit:
        switch(_splitState) {

            case splitOff:
            case splitXY:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[_splitState])));
            string_line = nameBuffer;
            updateDisplay(1, message_std, 0, true);
            string_line = emptyLine;
            updateDisplay(2, message_std, 0, true);
            break;

            case splitXYFunctions:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXY])));
            string_line = nameBuffer;
            updateDisplay(1, message_std, 0, true);
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXYFunctions])));
            string_line = nameBuffer;
            updateDisplay(2, message_std, 0, true);
            break;

            default:
            break;

        }

        default:
        break;

    }

}

void MessageBuilder::displayCurveChange(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t type, uint8_t padNumber)  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    string_line = nameBuffer;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveNameArray[type])));
    string_line += nameBuffer;

    updateDisplay(1, message_std, 0, true);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayCClimitChange(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));

    string_line = nameBuffer;
    string_line += ccValue;

    updateDisplay(1, message_std, 0, true);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayCCchange(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)   {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));

    string_line = nameBuffer;
    string_line += ccValue;

    updateDisplay(1, message_std, 0, true);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayMIDIchannelChange(uint8_t channel) {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[5])));
    string_line = nameBuffer;
    string_line += channel;

    updateDisplay(1, message_std, 0, true);
    string_line = emptyLine;
    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes)  {

    if (!numberOfNotes) string_line = emptyLine; else

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octaves[i];
        string_line += " ";

    }

    updateDisplay(1, text, 0, true);

}

void MessageBuilder::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    string_line = "Active octave: ";
    string_line += octave;

    updateDisplay(2, text, 0, true);

}

void MessageBuilder::displayNoteChange(changeOutput_t result, changeType_t type, int8_t value) {

    if (type == noteUpOrDown)  {

        if (result == outputChanged)    {

            string_line = value ? "One note up" : "One note down";
            updateDisplay(1, message_std, 0, true);
            string_line = emptyLine;
            updateDisplay(2, message_std, 0, true);

        } else if (result == outOfRange)  {

            string_line = "Out of range!";
            updateDisplay(1, message_std, 0, true);
            string_line = emptyLine;
            updateDisplay(2, message_std, 0, true);

        }   else if (result == notAllowed)  {

            string_line = "Switch to";
            updateDisplay(1, message_std, 0, true);

            string_line = "predefined preset";
            updateDisplay(2, message_std, 0, true);

        }

    }   else {  //octave/tonic change

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[type])));
        string_line = nameBuffer;

        if (type == octaveChange) string_line += value;
        else if (type == noteChange) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            string_line += nameBuffer;

        }

        updateDisplay(1, message_std, 0, true);
        string_line = emptyLine;
        updateDisplay(2, message_std, 0, true);

    }

}

void MessageBuilder::displayEditModeNotAllowed(padEditModeResult_t errorType)   {

    uint8_t errorIndex = 0;

    switch (errorType)  {

        case notUserPreset:
        errorIndex = 0;
        break;

        case padNotReleased:
        errorIndex = 2;
        break;

        default:
        return;

    }

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex])));
    string_line = nameBuffer;
    updateDisplay(1, message_std, 0, true);
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex+1])));
    string_line = nameBuffer;
    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::displayPadEditMode(uint8_t padNumber)  {

    string_line = "Editing pad ";
    string_line += padNumber;
    updateDisplay(0, text, 0, true);
    string_line = emptyLine;
    updateDisplay(1, text, 0, true);
    updateDisplay(2, text, 0, true);
    updateDisplay(3, text, 0, true);

}

void MessageBuilder::displayOctaveChange(uint8_t octave)   {

    string_line = "Octave set to " + octave;
    updateDisplay(1, message_std, 0, true);
    string_line = emptyLine;
    updateDisplay(2, message_std, 0, true);

}

void MessageBuilder::clearPadEditMode()    {

    string_line = emptyLine;
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true);

}


void MessageBuilder::displayServiceMenu()  {

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

void MessageBuilder::changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption) {

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

void MessageBuilder::selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption)  {



}

void MessageBuilder::clearPadData() {

    //clear all rows except first one
    string_line = emptyLine;

    for (int i=1; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true);

}


MessageBuilder messageBuilder;