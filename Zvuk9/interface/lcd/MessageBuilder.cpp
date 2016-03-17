#include "MessageBuilder.h"

MessageBuilder::MessageBuilder()    {

    string_line.reserve(MAX_TEXT_LENGTH);

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

    string_line.toCharArray(char_line, NUMBER_OF_LCD_COLUMNS, 0);

    lcDisplay.displayText(0, char_line);

}

MessageBuilder messageBuilder;