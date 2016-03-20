#include "LCD.h"
#include <util/delay.h>

LCD::LCD()  {

    displayMessage_var = false;
    messageDisplayTime = 0;
    string_line.reserve(MAX_TEXT_SIZE);

    lcd_init(LCD_DISP_ON);
    _delay_ms(100);

}

void LCD::init()    {

    lcd_clrscr();

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        lineChange[i] = false;
        scrollEnabled[i] = false;
        scrollDirection[i] = true;

        scrollIndex[i] = 0;

    }

    //init char arrays
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        for (int j=0; j<MAX_TEXT_SIZE; j++)   {

            lcdLine_char[i][j] = ' ';
            lcdLineMessage_char[i][j] = ' ';
            lastLCDLine_char[i][j] = ' ';

        }

        lcdLine_char[i][MAX_TEXT_SIZE] = '\0';
        lcdLineMessage_char[i][NUMBER_OF_LCD_COLUMNS] = '\0';
        lastLCDLine_char[i][NUMBER_OF_LCD_COLUMNS] = '\0';

    }

    //init scroll line
    for (int i=0; i<MAX_TEXT_SIZE; i++)
        lcdLineScroll_char[i] = ' ';
    lcdLineScroll_char[MAX_TEXT_SIZE] = '\0';

   lastScrollTime = 0;
   displayMessage_var = false;

   _delay_ms(100);
   lcd_gotoxy(0,0);

}

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress
    if ((newMillis() - lastLCDupdateTime) < LCD_REFRESH_TIME) return; //we don't need to update lcd in real time

    //get message status to determine what to print
    messageStatus_t messageStatus = getMessageStatus();

    //use char pointer to point to line we're going to print
    char *charPointer;

    checkPadDataClear();

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        checkScroll(i);

        if (!lineChange[i]) continue;

        switch(messageStatus)   {

            case showMessage:
            charPointer = lcdLineMessage_char[i];
            break;

            default:
            if (scrollEnabled[i])   charPointer = lcdLineScroll_char;
            else                    charPointer = lcdLine_char[i];
            break;

        }

        //this is to avoid buffer overflow when comparing current and previous
        //lines and current is longer than previous
        uint8_t characters = strlen(charPointer);
        uint8_t last_characters = strlen(lastLCDLine_char[i]);

        if (characters >= NUMBER_OF_LCD_COLUMNS) characters = NUMBER_OF_LCD_COLUMNS;

        for (int j=0; j<characters; j++) {

            if (j < last_characters)    {

                if (charPointer[j] != lastLCDLine_char[i][j])  {

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
            lcd_putc(' ');

        }

        //lastLCDLine doesn't need to be null-terminated
        //because of other checks
        strcpy(lastLCDLine_char[i], charPointer);
        lineChange[i] = false;

    }

    lastLCDupdateTime = newMillis();

}

messageStatus_t LCD::getMessageStatus()    {

    if (displayMessage_var)   {

        #if MODE_SERIAL > 0
            Serial.println(F("Displaying LCD message"));
        #endif

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lineChange[i] = true;

        displayMessage_var = false;
        messageActivated = true;

        return showMessage;

    }

    if (messageActivated)   {

        if (!((newMillis() - messageDisplayTime) > LCD_MESSAGE_DURATION)) return messageDisplayed;

        messageActivated = false;

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++)
            lcdLineMessage_char[i][j] = ' ';
            lcdLineMessage_char[i][NUMBER_OF_LCD_COLUMNS] = '\0';
            lineChange[i] = true;

        }

        #if MODE_SERIAL > 0
            Serial.println(F("Clearing LCD message"));
        #endif

        return clearMessage;

    }

    return noMessage;

}

void LCD::checkScroll(uint8_t row) {

    if (!scrollEnabled[row]) return;

    if ((newMillis() - lastScrollTime) < LCD_SCROLL_TIME) return;

    //scrollIndex is validated below
    for (int i=0; i<NUMBER_OF_LCD_COLUMNS; i++)
        lcdLineScroll_char[i] = lcdLine_char[row][i+scrollIndex[row]];

    if (scrollDirection[row])    {

        if (((strlen(lcdLine_char[row])-1) - scrollIndex[row]) > NUMBER_OF_LCD_COLUMNS) {

            scrollIndex[row]++;

        }   else scrollDirection[row] = false;

    }   else {

        scrollIndex[row]--;
        if (scrollIndex[row] < 0) { scrollDirection[row] = true; scrollIndex[row] = 0; }

    }

    lineChange[row] = true;

    lastScrollTime = newMillis();

}

void LCD::displayMessage(uint8_t row, const char *message)  {

    strcpy(lcdLineMessage_char[row], message);

    messageDisplayTime = newMillis();
    displayMessage_var = true;

}

void LCD::displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite)    {

    uint8_t size = strlen(text);

    if (overwrite) {

        //overwrite current text on selected line
        strcpy(lcdLine_char[row], text);

    } else {

        //append characters
        uint8_t charArrayIndex = 0;
        while (charArrayIndex < size)   {

            lcdLine_char[row][startIndex+charArrayIndex] = text[charArrayIndex];
            charArrayIndex++;

        }

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

void LCD::checkPadDataClear()   {

    //clear pad data with some delay to allow rapid pad press without clearing display every time

    if (padClearDelay)  {

        if ((newMillis() - padClearDelay) > LCD_PAD_DATA_CLEAR_DELAY)
            clearPadData(true);

    }

}

LCD display;
