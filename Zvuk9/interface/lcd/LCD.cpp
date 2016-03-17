#include "LCD.h"
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

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress

    if ((newMillis() - lastLCDupdateTime) < LCD_REFRESH_TIME) return;

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

void LCD::displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex, bool overwrite)    {

    if (overwrite)
        //overwrite current text on selected line
        lcdLine[row] = text;
    else {

        //append characters
        uint8_t charArrayIndex = 0;
        while (text[charArrayIndex] != '\0')   {

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
