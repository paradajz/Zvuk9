#ifndef MESSAGEBUILDER_H_
#define MESSAGEBUILDER_H_

//purpose of this class is to build strings which are going to be displayed on lcd

#include "LCD.h"
#include "LCDsettings.h"

#define MAX_TEXT_SIZE   (NUMBER_OF_LCD_COLUMNS*2)

class MessageBuilder {

    public:
    MessageBuilder();
    void displayProgramAndPreset(uint8_t program, uint8_t preset);

    private:
    String string_line;
    char nameBuffer[MAX_TEXT_SIZE];
    char char_line[NUMBER_OF_LCD_COLUMNS];

};

extern MessageBuilder messageBuilder;

#endif