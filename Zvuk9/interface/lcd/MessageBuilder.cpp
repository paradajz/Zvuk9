#include "MessageBuilder.h"

#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15
#define CC_X_START                      0
#define CC_Y_START                      9

MessageBuilder::MessageBuilder()    {

    string_line.reserve(MAX_TEXT_LENGTH);

}

void MessageBuilder::updateDisplay(uint8_t row, bool overWrite, lcdTextType type)    {

    string_line.toCharArray(char_line, string_line.length()+1, 0);

    switch(type)    {

        case text:
        lcDisplay.displayText(row, char_line, string_line.length(), overWrite);
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

    updateDisplay(PROGRAM_PRESET_ROW_temp);

}

void MessageBuilder::displayNotes(uint8_t note[], uint8_t octave[], uint8_t numberOfNotes)    {

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[note[i]])));
        if (!i) string_line = nameBuffer;
        else string_line += nameBuffer;
        string_line += octave[i];
        string_line += " ";

    }

    updateDisplay(PAD_NOTE_ROW_temp);

}

void MessageBuilder::displayVelocity(uint8_t velocity)  {

    string_line = "v";
    string_line += velocity;
    if (velocity < 10)         string_line += "  ";
    else if (velocity < 100)   string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW_temp);

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

    updateDisplay(PAD_V_XY_AT_ROW_temp, false);

}

void MessageBuilder::displayAftertouch(uint8_t afterTouch)  {

    string_line = "at";
    string_line += afterTouch;
    if (afterTouch < 10)       string_line += "  ";
    else if (afterTouch < 100) string_line += " ";

    updateDisplay(PAD_V_XY_AT_ROW_temp, false);

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

    updateDisplay(XY_ROW_temp);

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

}

MessageBuilder messageBuilder;