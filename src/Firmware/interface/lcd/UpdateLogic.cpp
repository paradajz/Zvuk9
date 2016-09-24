#include "LCD.h"

#ifdef LCD_H_
#include <util/delay.h>

#define SCROLL_LINE_LENGTH  (NUMBER_OF_LCD_COLUMNS-5)

LCD::LCD()  {

    displayMessage_var = false;
    messageDisplayTime = 0;
    _delay_ms(250);
    lcd_init(LCD_DISP_ON);

    setupLCDlayout();

}

void LCD::init()    {

    lcd_clrscr();

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        lineChange[i] = false;
        scrollEnabled[i] = false;
        scrollDirection[i] = true;
        scrollStartIndex[i] = 0;
        scrollIndex[i] = 0;

    }

    //init char arrays
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        lcdLine[i][0] = '\0';
        lcdLineMessage[i][0] = '\0';
        lastLCDLine[i][0] = '\0';
        lcdLineScroll[i][0] = '\0';

    }

   lastScrollTime = 0;
   displayMessage_var = false;

   _delay_ms(100);
   lcd_gotoxy(0,0);

}

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress
    if ((rTimeMillis() - lastLCDupdateTime) < LCD_REFRESH_TIME) return; //we don't need to update lcd in real time

    //get message status to determine what to print
    messageStatus_t messageStatus = getMessageStatus();

    //use char pointer to point to line we're going to print
    char *charPointer;

    //checkPadDataClear();

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        checkScroll(i);

        if (!lineChange[i]) continue;

        switch(messageStatus)   {

            case showMessage:
            charPointer = lcdLineMessage[i];
            break;

            case messageDisplayed:
            return; //line change + message shown = wait

            default:
            if (scrollEnabled[i])   charPointer = lcdLineScroll[i];
            else                    charPointer = lcdLine[i];
            break;

        }

        //this is to avoid buffer overflow when comparing current and previous
        //lines and current is longer than previous
        uint8_t characters = strlen(charPointer);
        uint8_t last_characters = strlen(lastLCDLine[i]);

        if (characters >= NUMBER_OF_LCD_COLUMNS) characters = NUMBER_OF_LCD_COLUMNS;

        for (int j=0; j<characters; j++) {

            if (j < last_characters)    {

                if (charPointer[j] != lastLCDLine[i][j])  {

                    lcd_gotoxy(j, i);
                    lcd_putc(charPointer[j]);

                }

            }   else { //this index is longer then previous line, just print

                lcd_gotoxy(j, i);
                lcd_putc(charPointer[j]);

            }

        }

        //now fill remaining columns with spaces
        for (int j=characters; j<NUMBER_OF_LCD_COLUMNS; j++)    {

            lcd_gotoxy(j, i);
            lcd_putc(SPACE_CHAR);

        }

        //lastLCDLine doesn't need to be null-terminated
        //because of other checks
        strcpy(lastLCDLine[i], charPointer);
        lineChange[i] = false;

    }

    lastLCDupdateTime = rTimeMillis();

}

messageStatus_t LCD::getMessageStatus()    {

    if (displayMessage_var)   {

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lineChange[i] = true;

        displayMessage_var = false;
        messageActivated = true;

        return showMessage;

    }

    if (messageActivated)   {

        if (!((rTimeMillis() - messageDisplayTime) > LCD_MESSAGE_DURATION)) return messageDisplayed;

        messageActivated = false;

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++)
            lcdLineMessage[i][j] = ' ';
            lcdLineMessage[i][NUMBER_OF_LCD_COLUMNS] = '\0';
            lineChange[i] = true;

        }

        return clearMessage;

    }

    return noMessage;

}

void LCD::checkScroll(uint8_t row) {

    if (!scrollEnabled[row]) return;

    if ((rTimeMillis() - lastScrollTime) < LCD_SCROLL_TIME) return;

    //copy string up to scrollStartIndex
    for (int i=0; i<scrollStartIndex[row]; i++)
        lcdLineScroll[row][i] = lcdLine[row][i];

    //scrollIndex is validated below
    for (int i=scrollStartIndex[row]; i<NUMBER_OF_LCD_COLUMNS; i++)
        lcdLineScroll[row][i] = lcdLine[row][i+scrollIndex[row]];

    if (scrollDirection[row])    {

        if (((int8_t)strlen(lcdLine[row]) - 1 - scrollStartIndex[row] - scrollIndex[row]) > (NUMBER_OF_LCD_COLUMNS - scrollStartIndex[row] - 1)) {

            scrollIndex[row]++;

        }   else scrollDirection[row] = false;

    }   else {

        scrollIndex[row]--;
        if (scrollIndex[row] < 0) { scrollDirection[row] = true; scrollIndex[row] = 0; }

    }

    lineChange[row] = true;

    lastScrollTime = rTimeMillis();

}

void LCD::displayMessage(uint8_t row, const char *message)  {

    strcpy(lcdLineMessage[row], message);

    messageDisplayTime = rTimeMillis();
    displayMessage_var = true;

}

void LCD::displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite, bool endOfLine)    {

    uint8_t size = strlen(text);

    if (overwrite) {

        //overwrite current text on selected line
        strcpy(lcdLine[row], text);

    } else {

        //append characters
        //we need to find out current string size
        uint8_t currentStringSize = strlen(lcdLine[row]);

        //now, we append the characters
        uint8_t charArrayIndex = 0;
        while (charArrayIndex < size)   { //make sure EOL isn't copied

            lcdLine[row][startIndex+charArrayIndex] = text[charArrayIndex];
            charArrayIndex++;

        }

        if (endOfLine)  {

            lcdLine[row][startIndex] = '\0';

        }   else {

            //we need to determine whether we need to change current EOL index
            bool changeEOLindex = (size+startIndex) > currentStringSize;

            switch(changeEOLindex)  {

                case false:
                break;

                case true:
                //we need to set EOL char to a new position
                lcdLine[row][startIndex+size] = '\0';
                break;

            }

        }

        size = strlen(lcdLine[row]);

    }

    lineChange[row] = true;

    if (size > NUMBER_OF_LCD_COLUMNS) {

        scrollEnabled[row] = true;
        scrollDirection[row] = true;
        scrollIndex[row] = 0;

    }   else {

        scrollEnabled[row] = false;
        scrollIndex[row] = 0;

    }

}

void LCD::setScrollStart(uint8_t row, uint8_t index)    {

    scrollStartIndex[row] = index;

}

LCD display;
#endif