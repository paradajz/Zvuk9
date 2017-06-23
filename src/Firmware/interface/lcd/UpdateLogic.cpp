#include "LCD.h"

U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

LCD::LCD()
{
    displayMessage_var = false;
    messageDisplayTime = 0;
    _delay_ms(250);
}

void LCD::init()
{
    u8x8.begin();
    u8x8.setPowerSave(0);
    u8x8.setFlipMode(1);

    u8x8.setFont(u8x8_font_pressstart2p_r);
    u8x8.clear();

    setupLCDlayout();

    //init char arrays
    for (int i=0; i<LCD_HEIGHT; i++)
    {
        lcdLine[i][0] = '\0';
        lcdLineMessage[i][0] = '\0';
    }

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
        if (!charChange[i])
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
                charPointer = lcdLine[i];
                break;
            }
            break;

            default:
            charPointer = lcdLine[i];
            break;
        }

        for (int j=0; j<(int)strlen(charPointer); j++)
        {
            if (bitRead(charChange[i], j))
            {
                u8x8.drawGlyph(j, i, charPointer[j]);
            }
        }

        //now fill remaining columns with spaces
        for (int j=strlen(charPointer); j<LCD_WIDTH; j++)
        {
            u8x8.drawGlyph(j, i, SPACE_CHAR);
        }

        charChange[i] = 0;
    }

    lastLCDupdateTime = rTimeMs();

    return true;
}

messageStatus_t LCD::getMessageStatus()
{
    if (displayMessage_var)
    {
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
        charChange[i] = (uint32_t)0xFFFFFFFF;

    messageTime = 0;
}

void LCD::displayMessage(uint8_t row, const char *message)
{
    strcpy(lcdLineMessage[row], message);
    messageDisplayTime = rTimeMs();
    displayMessage_var = true;

    //reset if set before
    if (messageTime == INFINITE_MESSAGE_TIME)
        messageTime = 0;

    for (int i=0; i<LCD_HEIGHT; i++)
        charChange[i] = (uint32_t)0xFFFFFFFF;
}

void LCD::displayText(uint8_t row, const char *text, uint8_t startIndex, bool overwrite)
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

            for (int i=0; i<(int)strlen(tempBuffer); i++)
            {
                if (tempBuffer[i] != lcdLine[row][i])
                    bitWrite(charChange[row], i, 1);
            }

            //finally, copy new line to lcdLine[row]
            strcpy(lcdLine[row], tempBuffer);
        }
        else
        {
            strcpy(lcdLine[row], text);
            charChange[row] = (uint32_t)0xFFFFFFFF;
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
            bitWrite(charChange[row], startIndex+charArrayIndex, 1);
            charArrayIndex++;
        }

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
