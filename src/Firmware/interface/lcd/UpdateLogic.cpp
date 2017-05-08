#include "LCD.h"

U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

LCD::LCD()
{
    displayMessage_var = false;
    messageDisplayTime = 0;
    _delay_ms(250);
    setupLCDlayout();
}

void LCD::init()
{
    u8x8.begin();
    u8x8.setPowerSave(0);
    u8x8.setFlipMode(1);

    u8x8.setFont(u8x8_font_pressstart2p_r);
    u8x8.clear();

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        lineChange[i] = false;
        scrollEnabled[i] = false;
        scrollDirection[i] = true;
        //scrollStartIndex[i] = 0;
        //scrollIndex[i] = 0;
    }

    //init char arrays
    for (int i=0; i<LCD_HEIGHT; i++)
    {
        lcdLine[i][0] = '\0';
        lcdLineMessage[i][0] = '\0';
        lastLCDLine[i][0] = '\0';
        //lcdLineScroll[i][0] = '\0';
    }

    //empty init, hacky
    lineChange[0] = true;
    update();
    lineChange[0] = false;

   lastScrollTime = 0;
   displayMessage_var = false;

   _delay_ms(100);
}

bool LCD::update()
{
    if ((rTimeMs() - lastLCDupdateTime) < LCD_REFRESH_TIME)
        return false; //we don't need to update lcd in real time

    //get message status to determine what to print
    messageStatus_t messageStatus = getMessageStatus();

    //use char pointer to point to line we're going to print
    char *charPointer;

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        //checkScroll(i);

        if (!lineChange[i])
            continue;

        switch(messageStatus)
        {
            case showMessage:
            charPointer = lcdLineMessage[i];
            break;

            case messageDisplayed:
            if (messageTime != INFINITE_MESSAGE_TIME)
            {
                //line change + message shown = wait
                return false;
            }
            else
            {
                removeMessage();
                //it's possible that we're changing midi channel with preset encoder - clear the modifier
                buttons.setModifierState(false);
                //if (scrollEnabled[i])
                    //charPointer = lcdLineScroll[i];
                //else
                    charPointer = lcdLine[i];
                break;
            }
            break;

            default:
            //if (scrollEnabled[i])
                //charPointer = lcdLineScroll[i];
            //else
                charPointer = lcdLine[i];
            break;
        }

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
                    u8x8.drawGlyph(j, i, charPointer[j]);
                }
            }
            else
            {
                //this index is longer then previous line, just print
                u8x8.drawGlyph(j, i, charPointer[j]);
            }
        }

        //now fill remaining columns with spaces
        for (int j=characters; j<LCD_WIDTH; j++)
        {
            u8x8.drawGlyph(j, i, SPACE_CHAR);
        }

        //lastLCDLine doesn't need to be null-terminated
        //because of other checks
        strcpy(lastLCDLine[i], charPointer);
    }

    for (int i=0; i<LCD_HEIGHT; i++)
        lineChange[i] = false;

    lastLCDupdateTime = rTimeMs();

    return true;
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

//void LCD::checkScroll(uint8_t row)
//{
    //if (!scrollEnabled[row])
        //return;
//
    //if ((rTimeMs() - lastScrollTime) < LCD_SCROLL_TIME)
        //return;
//
    ////copy string up to scrollStartIndex
    //for (int i=0; i<scrollStartIndex[row]; i++)
        //lcdLineScroll[row][i] = lcdLine[row][i];
//
    ////scrollIndex is validated below
    //for (int i=scrollStartIndex[row]; i<LCD_WIDTH; i++)
        //lcdLineScroll[row][i] = lcdLine[row][i+scrollIndex[row]];
//
    //if (scrollDirection[row])
    //{
        //if (((int8_t)strlen(lcdLine[row]) - 1 - scrollStartIndex[row] - scrollIndex[row]) > (LCD_WIDTH - scrollStartIndex[row] - 1))
            //scrollIndex[row]++;
        //else
            //scrollDirection[row] = false;
    //}
    //else
    //{
        //scrollIndex[row]--;
//
        //if (scrollIndex[row] < 0)
        //{
            //scrollDirection[row] = true;
            //scrollIndex[row] = 0;
        //}
    //}
//
    //lineChange[row] = true;
    //lastScrollTime = rTimeMs();
//}

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

        //append spaces to beginning
        if (startIndex)
        {
            //first, start new line with startIndex spaces
            //use tempBuffer since it's highly likely that text actually points to stringBuffer
            uint8_t size_sb = 0;
            startLine(true);
            addSpaceToCharArray(startIndex, size_sb, true);
            endLine(size_sb, true);

            //join tempbuffer with text
            strcat(tempBuffer, text);

            //finally, copy new line to lcdLine[row]
            strcpy(lcdLine[row], tempBuffer);
        }
        else
        {
            strcpy(lcdLine[row], text);
        }
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
        //scrollIndex[row] = 0;
    }
    else
    {
        //scrollEnabled[row] = false;
        //scrollIndex[row] = 0;
    }
}

void LCD::setScrollStart(uint8_t row, uint8_t index)
{
    //scrollStartIndex[row] = index;
}

void LCD::setMessageTime(int32_t msgTime)
{
    messageTime = -1;
}

void LCD::setDirectWriteState(bool state)
{
    directWrite = state;
}

LCD display;
