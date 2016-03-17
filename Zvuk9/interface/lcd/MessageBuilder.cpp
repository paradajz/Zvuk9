#include "MessageBuilder.h"

#define XY_POSITION_START               5
#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15
#define CC_X_START                      0
#define CC_Y_START                      9

MessageBuilder::MessageBuilder()    {

    string_line.reserve(MAX_TEXT_LENGTH);

}

void MessageBuilder::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex)    {

    string_line.toCharArray(char_line, string_line.length()+1, 0);

    switch(type)    {

        case text:
        lcDisplay.displayText(row, char_line, string_line.length(), startIndex);
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
    string_line = "P";
    string_line += program;
    string_line += " ";

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        string_line += nameBuffer;

        }   else {

        string_line += "User preset ";
        string_line += (preset - NUMBER_OF_PREDEFINED_SCALES + 1);

    }

    updateDisplay(PROGRAM_PRESET_ROW_temp, text);

}

void MessageBuilder::displayNotes(uint8_t note[], uint8_t octave[], uint8_t numberOfNotes)    {

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[note[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octave[i];
        string_line += " ";

    }

    updateDisplay(PAD_NOTE_ROW_temp, text);

}

void MessageBuilder::displayVelocity(uint8_t velocity)  {

    string_line = "v";
    string_line += velocity;
    if (velocity < 10)         string_line += "  ";
    else if (velocity < 100)   string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW_temp, text);

}

void MessageBuilder::displayXYposition(uint8_t x, uint8_t y, bool xAvailable, bool yAvailable)   {

    //velocity, x, y and aftertouch are all in same line
    //special care needs to be taken here

    if (xAvailable && yAvailable)   {

        string_line = "x";
        string_line += x;
        if (x < 10)         string_line += "  ";
        else if (x < 100)   string_line += " ";

        string_line += "y";
        string_line += y;
        if (y < 10)         string_line += "  ";
        else if (y < 100)   string_line += " ";

    }   else if (!xAvailable && yAvailable) {

            for (int i=0; i<4; i++)
                string_line[X_COORDINATE_START+i] = emptyLine[i];

            string_line += "y";
            string_line += y;
            if (y < 10)         string_line += "  ";
            else if (y < 100)   string_line += " ";

    }   else if (xAvailable && !yAvailable) {

            string_line = "x";
            string_line += x;
            if (x < 10)         string_line += "  ";
            else if (x < 100)   string_line += " ";

            for (int i=0; i<4; i++)
                string_line[Y_COORDINATE_START+i] = emptyLine[i];

    }

    updateDisplay(PAD_V_XY_AT_ROW_temp, text, XY_POSITION_START);

}

void MessageBuilder::displayAftertouch(uint8_t afterTouch)  {

    string_line = "at";
    string_line += afterTouch;
    if (afterTouch < 10)       string_line += "  ";
    else if (afterTouch < 100) string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW_temp, text, AFTERTOUCH_START);

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

    updateDisplay(XY_ROW_temp, text);

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

    updateDisplay(1, message_std);
    string_line = emptyLine;
    updateDisplay(2, message_std);

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
            updateDisplay(1, message_std);

            string_line = "pad ";
            string_line += padNumber;
            updateDisplay(2, message_std);

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[messageType])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[messageType])));

            string_line = nameBuffer;
            updateDisplay(1, message_std);

            string_line = "all pads";
            updateDisplay(2, message_std);

        }
        break;

        case featureSplit:
        switch(_splitState) {

            case splitOff:
            case splitXY:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[_splitState])));
            string_line = nameBuffer;
            updateDisplay(1, message_std);
            string_line = emptyLine;
            updateDisplay(2, message_std);
            break;

            case splitXYFunctions:
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXY])));
            string_line = nameBuffer;
            updateDisplay(1, message_std);
            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[splitXYFunctions])));
            string_line = nameBuffer;
            updateDisplay(2, message_std);
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

    updateDisplay(1, message_std);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std);

}

void MessageBuilder::displayCClimitChange(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));

    string_line = nameBuffer;
    string_line += ccValue;

    updateDisplay(1, message_std);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std);

}

void MessageBuilder::displayCCchange(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)   {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));

    string_line = nameBuffer;
    string_line += ccValue;

    updateDisplay(1, message_std);

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    string_line = nameBuffer;

    if (!(_splitState == splitOff))
    string_line += padNumber; //local change

    updateDisplay(2, message_std);

}

void MessageBuilder::displayMIDIchannelChange(uint8_t channel) {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[5])));
    string_line = nameBuffer;
    string_line += channel;

    updateDisplay(1, message_std);
    string_line = emptyLine;
    updateDisplay(2, message_std);

}

void MessageBuilder::displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes)  {

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octaves[i];
        string_line += " ";

    }

    updateDisplay(1, text);

}

void MessageBuilder::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    string_line = "Active octave: ";
    string_line += octave;

    updateDisplay(2, text);

}

void MessageBuilder::displayNoteChange(changeOutput_t result, changeType_t type, int8_t value) {

    if (type == noteUpOrDown)  {

        if (result == outputChanged)    {

            string_line = value ? "One note up" : "One note down";
            updateDisplay(1, message_std);
            string_line = emptyLine;
            updateDisplay(2, message_std);

        } else if (result == outOfRange)  {

            string_line = "Out of range!";
            updateDisplay(1, message_std);
            string_line = emptyLine;
            updateDisplay(2, message_std);

        }   else if (result == notAllowed)  {

            string_line = "Switch to";
            updateDisplay(1, message_std);

            string_line = "predefined preset";
            updateDisplay(2, message_std);

        }

    }   else {  //octave/tonic change

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[type])));
        string_line = nameBuffer;

        if (type == octaveChange) string_line += value;
        else if (type == noteChange) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            string_line += nameBuffer;

        }

        updateDisplay(1, message_std);
        string_line = emptyLine;
        updateDisplay(2, message_std);

    }

}

MessageBuilder messageBuilder;