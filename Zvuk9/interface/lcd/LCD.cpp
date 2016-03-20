#include "LCD.h"
#include <util/delay.h>

LCD::LCD()  {

    displayMessage_var = false;
    messageDisplayTime = 0;

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
    //init lcdline
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        for (int j=0; j<MAX_TEXT_LENGTH; j++)   {

            lcdLine_char[i][j] = ' ';
            lcdLineMessage_char[i][j] = ' ';
            lastLCDLine_char[i][j] = ' ';

        }

        lcdLine_char[i][MAX_TEXT_LENGTH] = '\0';
        lcdLineMessage_char[i][NUMBER_OF_LCD_COLUMNS] = '\0';
        lastLCDLine_char[i][NUMBER_OF_LCD_COLUMNS] = '\0';

    }

    //init scroll line
    for (int i=0; i<MAX_TEXT_LENGTH; i++)
        lcdLineScroll_char[i] = ' ';
    lcdLineScroll_char[MAX_TEXT_LENGTH] = '\0';

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

    char *charPointer;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        //checkScroll(i);

        if (lineChange[i])  {

            switch(messageStatus)   {

                case showMessage:
                charPointer = lcdLineMessage_char[i];
                break;

                default:
                charPointer = lcdLine_char[i];
                break;

            }

            for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                if (charPointer[j] != lastLCDLine_char[i][j])  {

                    lcd_gotoxy(j, i);
                    lcd_putc(charPointer[j]);

                }

            }

            strcpy(lastLCDLine_char[i], charPointer);
            lineChange[i] = false;

        }

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

    //if (scrollEnabled[row])   {
//
        //if ((newMillis() - lastScrollTime) > LCD_SCROLL_TIME)    {
//
            //lcdLineScroll = lcdLine[row].substring(scrollIndex[row], MAX_TEXT_LENGTH);
            ////strcpy(lcdLineScroll_char, )
//
            //for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {
//
                //if (lastLCDLine_char[row][j] != lcdLineScroll_char[j])   {
//
                    //lcd_gotoxy(j, row);
                    //lcd_putc(lcdLineScroll_char[j]);
//
                //}
//
            //}
//
            //lastScrollTime = newMillis();
//
            //if (scrollDirection[row])    {
//
                //#if MODE_SERIAL > 0
                    //Serial.println(F("Scrolling LCD line"));
                    //Serial.print(row+1);
                    //Serial.println(F(" left"));
                    //Serial.print(F("Scroll index: "));
                    //Serial.println(scrollIndex[row]);
                //#endif
//
                //if (((lcdLine[row].length()-1) - scrollIndex[row]) > NUMBER_OF_LCD_COLUMNS) {
//
                    //scrollIndex[row]++;
//
                //}   else scrollDirection[row] = false;
//
            //}   else {
//
                //#if MODE_SERIAL > 0
                    //Serial.println(F("Scrolling LCD line"));
                    //Serial.print(row+1);
                    //Serial.println(F(" right"));
                    //Serial.print(F("Scroll index: "));
                    //Serial.println(scrollIndex[row]);
                //#endif
//
                //scrollIndex[row]--;
                //if (scrollIndex[row] < 0) { scrollDirection[row] = true; scrollIndex[row] = 0; }
//
            //}
//
            //lastLCDLine[row] = lcdLineScroll;
            //strcpy(lastLCDLine_char[row], lcdLineScroll_char);
//
        //}
//
    //}

}


void LCD::displayMessage(uint8_t row, const char *message, uint8_t size)  {

    strcpy(lcdLineMessage_char[row], message);

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

    if (overwrite) {

        //overwrite current text on selected line
        strcpy(lcdLine_char[row], text);

    } else {

        //append characters
        uint8_t charArrayIndex = 0;
        while (text[charArrayIndex] != '\0')   {

            lcdLine_char[row][startIndex+charArrayIndex] = text[charArrayIndex];
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

        scrollEnabled[row] = false;
        scrollIndex[row] = 0;

    }

}

LCD lcDisplay;
