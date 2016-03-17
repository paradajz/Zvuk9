#include "LCD.h"
#include "menu/MenuStrings.h"
#include <util/delay.h>
#include "Icons.h"

#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15
#define CC_X_START                      0
#define CC_Y_START                      9

LCD::LCD()  {

    displayMessage_var = false;
    messageDisplayTime = 0;
    _clearPadData = false;
    keepMessage = false;
    restoreMessage = false;

    lcd_init(LCD_DISP_ON);
    _delay_ms(100);

}

void LCD::init()    {

   lcd_clrscr();

   for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

       lcdLine[i].reserve(MAX_TEXT_LENGTH);
       lcdLine[i] = emptyLine;
       lcdLineMessage[i].reserve(MAX_TEXT_LENGTH);
       lcdLineMessage[i] = emptyLine;
       lineChange[i] = false;
       lastLCDLine[i].reserve(MAX_TEXT_LENGTH);
       lastLCDmessage[i].reserve(MAX_TEXT_LENGTH);
       lastLCDLine[i] = emptyLine;
       scrollEnabled[i] = false;
       lcdLineScroll[i].reserve(MAX_TEXT_LENGTH);
       lcdLineScroll[i] = emptyLine;
       scrollDirection[i] = true;

   }

   lastScrollTime = 0;
   scrollIndex = 0;

   tempLine1.reserve(MAX_TEXT_LENGTH);
   tempLine2.reserve(MAX_TEXT_LENGTH);

   initIcons();

}

void LCD::displayHelloMessage() {

    lastLCDLine[0] = helloMessage;
    lcd_gotoxy(0,0);

    for (int i=0; i<(int)lastLCDLine[0].length(); i++)  {

        lcd_putc(lastLCDLine[0][i]);
        newDelay(75);

    } newDelay(250); lineChange[0] = true;

}

void LCD::clearPadData()    {

    #if MODE_SERIAL > 0
        Serial.println(F("Clearing pad data from LCD"));
    #endif

    _clearPadData = true;
    lastPadDataClearTime = newMillis();

}

void LCD::clearRow(uint8_t rowNumber)   {

    lcdLine[rowNumber] = emptyLine;

}

void LCD::displayNoteChange(changeOutput_t result, changeType_t type, int8_t value) {

    clearMessage();

    if (type == noteUpOrDown)  {

        if (result == outputChanged)
            lcdLineMessage[1] = value ? "One note up" : "One note down";
        else if (result == outOfRange)  {

            lcdLineMessage[1] = "Out of range!";

        }   else if (result == notAllowed)  {

            lcdLineMessage[1] = "Switch to";
            lcdLineMessage[2] = "predefined preset";

        }

    }   else {  //octave/tonic change

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lcdLineMessage[i] = emptyLine;

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[type])));
        lcdLineMessage[1] = nameBuffer;
        if (type == octaveChange) lcdLineMessage[1] += value;
        else if (type == noteChange) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            lcdLineMessage[1] += nameBuffer;

        }

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::displayMIDIchannelChange(uint8_t channel) {

    clearMessage();

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[5])));
    lcdLineMessage[1] = nameBuffer;
    lcdLineMessage[1] += channel;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::displayOctaveChange(uint8_t octave)   {

    clearMessage();

    lcdLineMessage[1] = "Octave set to " + octave;

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress

    checkClearScreen();
    if ((newMillis() - lastLCDupdateTime < LCD_REFRESH_TIME) && !_clearPadData) return;

    if (displayMessage_var)   {

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                if (lcdLineMessage[i][j] != lastLCDLine[i][j])  {

                    lcd_gotoxy(j, i);
                    lcd_putc(lcdLineMessage[i][j]);

                }

            }

            lastLCDLine[i] = lcdLineMessage[i];

        }   displayMessage_var = false; messageActivated = true;

    }

    if (messageActivated)   {

        if ((!((newMillis() - messageDisplayTime) > LCD_MESSAGE_DURATION)) || keepMessage) return;

        if (restoreMessage) {

            for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

                lcdLineMessage[i] = lastLCDmessage[i];

            }

            restoreMessage = false;
            displayMessage_var = true;
            keepMessage = true;
            return;

        }

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lineChange[i] = true;
        clearMessage();

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        if (scrollEnabled[i])   {

            if ((newMillis() - lastScrollTime) > LCD_SCROLL_TIME)    {

                lcdLineScroll[i] = lcdLine[i].substring(scrollIndex, MAX_TEXT_LENGTH);
                expandLine(i, scrollLine);

                for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                    if (lastLCDLine[i][j] != lcdLineScroll[i][j])   {

                        lcd_gotoxy(j, i);
                        lcd_putc(lcdLineScroll[i][j]);

                    }

                }

                lastScrollTime = newMillis();

                if (scrollDirection[i])    {

                    if (((lcdLine[i].length()-1) - scrollIndex) > NUMBER_OF_LCD_COLUMNS) {

                        scrollIndex++;

                    }   else scrollDirection[i] = false;

                    }   else {

                    scrollIndex--;
                    if (scrollIndex < 0) { scrollDirection[i] = true; scrollIndex = 0; }

                }

                lastLCDLine[i] = lcdLineScroll[i];

            }

        }

        else if (lineChange[i])  {

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                if (lcdLine[i][j] != lastLCDLine[i][j]) {

                    lcd_gotoxy(j, i);
                    lcd_putc(lcdLine[i][j]);

                }

            }

            lastLCDLine[i] = lcdLine[i];

        }

        lineChange[i] = false;

    }   lastRefreshTime = newMillis();

}

void LCD::expandLine(uint8_t lineNumber, lcdLineType_t lineType)    {

    switch (lineType)   {

        case regularLine:
        if (lcdLine[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)
        do {

            lcdLine[lineNumber] += " ";

        }   while (lcdLine[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

        case messageLine:
        if (lcdLineMessage[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)

        do {

            lcdLineMessage[lineNumber] += " ";

        }   while (lcdLineMessage[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

        case scrollLine:
        if (lcdLineScroll[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)

        do {

            lcdLineScroll[lineNumber] += " ";

        }   while (lcdLineScroll[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

    }

}

void LCD::displayPadEditMode(uint8_t padNumber)  {

    clearRow(0);
    clearRow(1);
    clearRow(2);
    clearRow(3);

    lcdLine[0] = "Editing pad ";
    lcdLine[0] += padNumber;
    expandLine(0, regularLine);

    expandLine(1, regularLine);
    expandLine(2, regularLine);
    expandLine(3, regularLine);
    lineChange[0] = true;
    lineChange[1] = true;
    lineChange[2] = true;
    lineChange[3] = true;

}

void LCD::clearPadEditMode()    {

    clearRow(1);
    clearRow(2);
    clearRow(3);

    scrollIndex = 0;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        scrollDirection[i] = true;
        scrollEnabled[i] = false;

    }

    lineChange[1] = true;
    lineChange[2] = true;
    lineChange[3] = true;

}

void LCD::displayEditModeNotAllowed(padEditError_t errorType)   {

    uint8_t errorIndex = 0;

    switch (errorType)  {

        case noUserPreset:
        errorIndex = 0;
        break;

        case padNotReleased:
        errorIndex = 2;
        break;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lcdLineMessage[i] = emptyLine;
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex])));
    lcdLineMessage[1] = nameBuffer; 
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex+1])));
    lcdLineMessage[2] = nameBuffer;

    expandLine(1, messageLine);
    expandLine(2, messageLine);

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::displayActiveOctave(int8_t octave)   {

    lcdLine[2] = "Active octave: ";
    lcdLine[2] += octave;

    lineChange[2] = true;
    expandLine(2, regularLine);

}

void LCD::displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes)  {

    clearRow(1);

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
        if (!i) lcdLine[1] = nameBuffer;
        else lcdLine[1] += nameBuffer;
        lcdLine[1] += octaves[i];
        lcdLine[1] += " ";

    }

    if (lcdLine[PAD_NOTE_ROW].length() > NUMBER_OF_LCD_COLUMNS)   {

        //scrolling logic
        scrollEnabled[1] = true;

    }   else {

        scrollEnabled[1] = false;
        scrollIndex = 0;
        scrollDirection[1] = true;

    }

    lineChange[1] = true;
    expandLine(1, regularLine);

}

void LCD::displayMessage(uint8_t row, const char *message, bool stayOn)  {

    if (lcdLineMessage[row] == message) return; //same message

    lcdLineMessage[row] = message;
    expandLine(row, messageLine);

    messageDisplayTime = newMillis();
    displayMessage_var = true;
    keepMessage = stayOn;

}

void LCD::clearMessage(bool forceClear)    {

    //check if there is message that's supposed to be on
    if (keepMessage)    {

        if (!forceClear)    {

            for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
                lastLCDmessage[i] = lcdLineMessage[i];

            restoreMessage = true;

        }   else {

            restoreMessage = false;
            displayMessage_var = false;
            messageActivated = false;
            for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
                lineChange[i] = true;

        }

    }   else if (restoreMessage && forceClear)  {

        restoreMessage = false;

    }   else {

        //clear all previous messages
        messageActivated = false;
        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

            lcdLineMessage[i] = emptyLine;

        }

    }   keepMessage = false;

}

bool LCD::checkClearScreen()    {

    bool clearScreen = false;

    if (_clearPadData)  {

        if (lastPadDataClearTime - lastLCDupdateTime > LCD_PAD_DATA_CLEAR_DELAY) {

            clearScreen = true;

        }   else

        if (newMillis() - lastPadDataClearTime > LCD_PAD_DATA_CLEAR_DELAY) {

            clearScreen = true;

        }

    }

    if (clearScreen)    {

        clearRow(PAD_NOTE_ROW);
        clearRow(PAD_V_XY_AT_ROW);
        clearRow(XY_ROW);

        lineChange[PAD_NOTE_ROW] = true;
        lineChange[PAD_V_XY_AT_ROW] = true;
        lineChange[XY_ROW] = true;
        scrollEnabled[PAD_NOTE_ROW] = false;
        scrollDirection[PAD_NOTE_ROW] = true;
        scrollIndex = 0;
        _clearPadData = false;

        return true;

    }   return false;

}

void LCD::displayServiceMenu()  {

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(menu_types[serviceMenu])));
    lcdLine[0] = nameBuffer;
    expandLine(0, regularLine);
    lineChange[0] = true;

    for (int i=0; i<(int16_t)progmemArraySize(service_menu_options); i++)    {

        (!i) ? lcdLine[i+1] = ">" : lcdLine[i+1] = " ";
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
        lcdLine[i+1] += nameBuffer;
        expandLine(i+1, regularLine);
        lineChange[i+1] = true;

    }

}

void LCD::changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption) {

    //we can display up to three options/suboptions at the time
    uint8_t markerOption = (option > 2) ? (NUMBER_OF_LCD_ROWS-1) : option;

    switch(type)    {

        case serviceMenu:
        for (int i=0; i<(int)progmemArraySize(service_menu_options); i++)    {

            if (i == markerOption)  {

                lcdLine[i+1] = ">";
                strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
                lcdLine[i+1] += nameBuffer;

            }   else {

                strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
                lcdLine[i+1] = " ";
                lcdLine[i+1] += nameBuffer;

            }

            expandLine(i+1, regularLine);

        }
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        lineChange[i] = true;

}

void LCD::selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption)  {



}

void LCD::displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex)    {

    if (!startIndex)
        //overwrite current text on selected line
        lcdLine[row] = text;
    else {

        //append characters
        uint8_t charArrayIndex = 0;
        while (!text[charArrayIndex] != '\0')   {

            lcdLine[row][startIndex+charArrayIndex] = text[charArrayIndex];
            charArrayIndex++;

        }

    }

    expandLine(row, regularLine);
    lineChange[row] = true;

    if (size > NUMBER_OF_LCD_COLUMNS) {

        scrollEnabled[row] = true;
        scrollDirection[row] = true;
        scrollIndex = 0;

    }   else scrollEnabled[row] = false;

    lastLCDupdateTime = newMillis();

}

LCD lcDisplay;
