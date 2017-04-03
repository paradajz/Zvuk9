#include "LCD.h"

#ifdef BOARD_R2
u8g_t u8g;
#endif

LCD::LCD()
{
    displayMessage_var = false;
    messageDisplayTime = 0;
    _delay_ms(250);
    #ifdef BOARD_R1
    lcd_init(LCD_DISP_ON);
    #endif
    setupLCDlayout();
}

void LCD::init()
{
    #ifdef BOARD_R1
    lcd_clrscr();
    #elif defined (BOARD_R2)
    /*
        SCK:    PORTB, Bit 1 --> PN(1,1)
        MOSI:   PORTB, Bit 2 --> PN(1,2)
        CS:     PORTA, Bit 4 --> PN(0,4)
        A0:     PORTA, Bit 3 --> PN(0,3)
        RESET:  PORTA, Bit 5 --> PN(0,5)
    */

    u8g_InitHWSPI(&u8g, &u8g_dev_ssd1322_nhd31oled_2x_bw_hw_spi, PN(0,4), PN(0,3), PN(0,5));
    u8g_Begin(&u8g);
    u8g_SetFont(&u8g, u8g_font_profont15);
    u8g_SetCursorFont(&u8g, u8g_font_profont15);
    #endif

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        lineChange[i] = false;
        scrollEnabled[i] = false;
        scrollDirection[i] = true;
        scrollStartIndex[i] = 0;
        scrollIndex[i] = 0;
    }

    //init char arrays
    for (int i=0; i<LCD_HEIGHT; i++)
    {
        lcdLine[i][0] = '\0';
        lcdLineMessage[i][0] = '\0';
        lastLCDLine[i][0] = '\0';
        lcdLineScroll[i][0] = '\0';
    }

    //empty init, hacky
    lineChange[0] = true;
    update();
    lineChange[0] = false;

   lastScrollTime = 0;
   displayMessage_var = false;

   _delay_ms(100);
   #ifdef BOARD_R1
   lcd_gotoxy(0,0);
   #endif
}

void LCD::update()
{
    #ifdef BOARD_R1
    if ((rTimeMs() - lastLCDupdateTime) < LCD_REFRESH_TIME)
        return; //we don't need to update lcd in real time
    #endif

    //get message status to determine what to print
    messageStatus_t messageStatus = getMessageStatus();

    //use char pointer to point to line we're going to print
    char *charPointer;

    #ifdef BOARD_R2
    bool proceed = false;
    #endif

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        //display on r2 is larger - no scrolling required
        #ifdef BOARD_R1
        checkScroll(i);
        #endif

        if (!lineChange[i])
            continue;

        //change occurred
        proceed = true;

        switch(messageStatus)
        {
            case showMessage:
            charPointer = lcdLineMessage[i];
            break;

            case messageDisplayed:
            if (messageTime != INFINITE_MESSAGE_TIME)
            {
                //line change + message shown = wait
                return;
            }
            else
            {
                removeMessage();
                if (scrollEnabled[i])
                    charPointer = lcdLineScroll[i];
                else
                    charPointer = lcdLine[i];
                break;
            }
            break;

            default:
            if (scrollEnabled[i])
                charPointer = lcdLineScroll[i];
            else
                charPointer = lcdLine[i];
            break;
        }

        #ifdef BOARD_R1
        //this is to avoid buffer overflow when comparing current and previous
        //lines and current is longer than previous
        uint8_t characters = strlen(charPointer);
        uint8_t last_characters = strlen(lastLCDLine[i]);

        if (characters >= LCD_WIDTH)
            characters = LCD_WIDTH;

        for (int j=0; j<characters; j++)
        {
            if (j < last_characters)
            {
                if (charPointer[j] != lastLCDLine[i][j])
                {
                    lcd_gotoxy(j, i);
                    lcd_putc(charPointer[j]);
                }
            }
            else
            {
                //this index is longer then previous line, just print
                lcd_gotoxy(j, i);
                lcd_putc(charPointer[j]);
            }
        }

        //now fill remaining columns with spaces
        for (int j=characters; j<LCD_WIDTH; j++)
        {
            lcd_gotoxy(j, i);
            lcd_putc(SPACE_CHAR);
        }
        #endif

        //lastLCDLine doesn't need to be null-terminated
        //because of other checks
        strcpy(lastLCDLine[i], charPointer);
    }

    #ifdef BOARD_R2
    static uint8_t updated = 0;

    if (proceed || updated)
    {
        if (messageStatus == showMessage)
        {
            u8g_FirstPage(&u8g);
            do
            {
                u8g_DrawStr(&u8g, 0, ROW_SPACING, lcdLineMessage[0]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 2, lcdLineMessage[1]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 3, lcdLineMessage[2]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 4, lcdLineMessage[3]);
            } while (u8g_NextPage(&u8g));
        }
        else
        {
            if (!updated)
            {
                u8g_FirstPage(&u8g);
                updated = 1;
            }
            //do
            //{
                u8g_DrawStr(&u8g, 0, ROW_SPACING, lcdLine[0]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 2, lcdLine[1]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 3, lcdLine[2]);
                u8g_DrawStr(&u8g, 0, ROW_SPACING * 4, lcdLine[3]);
            //}
            if (!u8g_NextPage(&u8g))
                updated = 0;
        }
    }
    #endif

    for (int i=0; i<LCD_HEIGHT; i++)
        lineChange[i] = false;

    #ifdef BOARD_R1
    lastLCDupdateTime = rTimeMs();
    #endif
}

messageStatus_t LCD::getMessageStatus()
{
    if (displayMessage_var)
    {
        for (int i=0; i<LCD_HEIGHT; i++)
            lineChange[i] = true;
        displayMessage_var = false;
        messageActivated = true;

        return showMessage;
    }

    if (messageActivated)
    {
        if ((!((rTimeMs() - messageDisplayTime) > LCD_MESSAGE_DURATION)) || (messageTime == INFINITE_MESSAGE_TIME))
            return messageDisplayed;

        removeMessage();

        return clearMessage;
    }

    return noMessage;
}

void LCD::removeMessage()
{
    messageActivated = false;

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        for (int j=0; j<LCD_WIDTH; j++)
            lcdLineMessage[i][j] = ' ';

        lcdLineMessage[i][LCD_WIDTH] = '\0';
            lineChange[i] = true;
    }

    messageTime = 0;
}

void LCD::checkScroll(uint8_t row)
{
    if (!scrollEnabled[row])
        return;

    if ((rTimeMs() - lastScrollTime) < LCD_SCROLL_TIME)
        return;

    //copy string up to scrollStartIndex
    for (int i=0; i<scrollStartIndex[row]; i++)
        lcdLineScroll[row][i] = lcdLine[row][i];

    //scrollIndex is validated below
    for (int i=scrollStartIndex[row]; i<LCD_WIDTH; i++)
        lcdLineScroll[row][i] = lcdLine[row][i+scrollIndex[row]];

    if (scrollDirection[row])
    {
        if (((int8_t)strlen(lcdLine[row]) - 1 - scrollStartIndex[row] - scrollIndex[row]) > (LCD_WIDTH - scrollStartIndex[row] - 1))
            scrollIndex[row]++;
        else
            scrollDirection[row] = false;
    }
    else
    {
        scrollIndex[row]--;

        if (scrollIndex[row] < 0)
        {
            scrollDirection[row] = true;
            scrollIndex[row] = 0;
        }
    }

    lineChange[row] = true;
    lastScrollTime = rTimeMs();
}

void LCD::displayMessage(uint8_t row, const char *message)
{
    strcpy(lcdLineMessage[row], message);
    messageDisplayTime = rTimeMs();
    displayMessage_var = true;

    //reset if set before
    if (messageTime == INFINITE_MESSAGE_TIME)
        messageTime = 0;
}

void LCD::displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite, bool endOfLine)
{
    uint8_t size = strlen(text);

    if (overwrite)
    {
        //overwrite current text on selected line
        strcpy(lcdLine[row], text);
    }
    else
    {
        //append characters
        //we need to find out current string size
        uint8_t currentStringSize = strlen(lcdLine[row]);

        //now, we append the characters
        uint8_t charArrayIndex = 0;
        while (charArrayIndex < size)
        {
            //make sure EOL isn't copied
            lcdLine[row][startIndex+charArrayIndex] = text[charArrayIndex];
            charArrayIndex++;
        }

        if (endOfLine)
        {
            lcdLine[row][startIndex] = '\0';
        }
        else
        {
            //we need to determine whether we need to change current EOL index
            bool changeEOLindex = (size+startIndex) > currentStringSize;

            switch(changeEOLindex)
            {
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

    if (size > LCD_WIDTH)
    {
        scrollEnabled[row] = true;
        scrollDirection[row] = true;
        scrollIndex[row] = 0;
    }
    else
    {
        scrollEnabled[row] = false;
        scrollIndex[row] = 0;
    }
}

void LCD::setScrollStart(uint8_t row, uint8_t index)
{
    scrollStartIndex[row] = index;
}

void LCD::setMessageTime(int32_t msgTime)
{
    messageTime = -1;
}

LCD display;
