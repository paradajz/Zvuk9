#include "LCD.h"
#include <util/delay.h>
//#include "Icons.h"

#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15
#define CC_X_START                      0
#define CC_Y_START                      9

const String emptyLine = "                                        ";

LCD::LCD()  {

    displayMessage_var = false;
    messageDisplayTime = 0;

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

       lastLCDLine[i].reserve(MAX_TEXT_LENGTH);
       lastLCDLine[i] = emptyLine;

       lastLCDmessage[i].reserve(MAX_TEXT_LENGTH);
       lastLCDmessage[i] = emptyLine;

       lineChange[i] = false;
       scrollEnabled[i] = false;
       scrollDirection[i] = true;

       scrollIndex[i] = 0;

   }

   lcdLineScroll.reserve(MAX_TEXT_LENGTH);
   lcdLineScroll = emptyLine;
   lastScrollTime = 0;
   displayMessage_var = false;

   _delay_ms(100);
   lcd_gotoxy(0,0);

}

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress

    if ((newMillis() - lastLCDupdateTime) < LCD_REFRESH_TIME) return;

    checkMessage();

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        checkScroll(i);

        if (lineChange[i])  {

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                if (lcdLine[i][j] != lastLCDLine[i][j]) {

                    lcd_gotoxy(j, i);
                    lcd_putc(lcdLine[i][j]);

                }

            }

            lastLCDLine[i] = lcdLine[i];

        }

        lineChange[i] = false;

    }   lastLCDupdateTime = newMillis();

}

void LCD::checkMessage()    {

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

        if (!((newMillis() - messageDisplayTime) > LCD_MESSAGE_DURATION)) return;

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lineChange[i] = true;
        messageActivated = false;

    }

}

void LCD::checkScroll(uint8_t row) {

    if (scrollEnabled[row])   {

        if ((newMillis() - lastScrollTime) > LCD_SCROLL_TIME)    {

            lcdLineScroll = lcdLine[row].substring(scrollIndex[row], MAX_TEXT_LENGTH);
            expandLine(row, scrollLine);

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                if (lastLCDLine[row][j] != lcdLineScroll[j])   {

                    lcd_gotoxy(j, row);
                    lcd_putc(lcdLineScroll[j]);

                }

            }

            lastScrollTime = newMillis();

            if (scrollDirection[row])    {

                #if MODE_SERIAL > 0
                    Serial.println(F("Scrolling LCD line"));
                    Serial.print(row+1);
                    Serial.println(F(" left"));
                    Serial.print(F("Scroll index: "));
                    Serial.println(scrollIndex[row]);
                #endif

                if (((lcdLine[row].length()-1) - scrollIndex[row]) > NUMBER_OF_LCD_COLUMNS) {

                    scrollIndex[row]++;

                }   else scrollDirection[row] = false;

            }   else {

                #if MODE_SERIAL > 0
                    Serial.println(F("Scrolling LCD line"));
                    Serial.print(row+1);
                    Serial.println(F(" right"));
                    Serial.print(F("Scroll index: "));
                    Serial.println(scrollIndex[row]);
                #endif

                scrollIndex[row]--;
                if (scrollIndex[row] < 0) { scrollDirection[row] = true; scrollIndex[row] = 0; }

            }

            lastLCDLine[row] = lcdLineScroll;

        }

    }

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
        if (lcdLineScroll.length() < NUMBER_OF_LCD_COLUMNS)

        do {

            lcdLineScroll += " ";

        }   while (lcdLineScroll.length() < NUMBER_OF_LCD_COLUMNS);
        break;

    }

}

void LCD::displayMessage(uint8_t row, const char *message)  {

    lcdLineMessage[row] = message;
    expandLine(row, messageLine);

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::displayText(uint8_t row, const char *text, uint8_t size, uint8_t startIndex, bool overwrite)    {

    #if MODE_SERIAL > 0
        Serial.print(F("Displaying text on LCD. Row "));
        Serial.print(row+1);
        Serial.print(F(", size: "));
        Serial.println(size);
    #endif

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

    lineChange[row] = true;

    if (size > NUMBER_OF_LCD_COLUMNS) {

        #if MODE_SERIAL > 0
            Serial.print(F("Scrolling line "));
            Serial.println(row+1);
        #endif

        scrollEnabled[row] = true;
        scrollDirection[row] = true;
        scrollIndex[row] = 0;

    }   else {

        expandLine(row, regularLine);
        scrollEnabled[row] = false;
        scrollIndex[row] = 0;

    }

}

LCD lcDisplay;
