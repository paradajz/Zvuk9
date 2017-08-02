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

    //init char arrays
    for (int i=0; i<LCD_HEIGHT; i++)
    {
        for (int j=0; j<MAX_TEXT_SIZE-2; j++)
        {
            lcdLine[i][j] = ' ';
            lcdLineMessage[i][j] = ' ';
        }

        lcdLine[i][MAX_TEXT_SIZE-1] = '\0';
        lcdLineMessage[i][MAX_TEXT_SIZE-1] = '\0';
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

void LCD::displayMessage(uint8_t row, const char *message, uint8_t startIndex)
{
    uint8_t size = strlen(message);

    //clear entire message first
    for (int j=0; j<MAX_TEXT_SIZE-2; j++)
        lcdLineMessage[row][j] = ' ';

    lcdLineMessage[row][MAX_TEXT_SIZE-1] = '\0';

    for (int i=0; i<size; i++)
        lcdLineMessage[row][startIndex+i] = message[i];

    messageDisplayTime = rTimeMs();
    displayMessage_var = true;

    //reset if set before
    if (messageTime == INFINITE_MESSAGE_TIME)
        messageTime = 0;

    for (int i=0; i<LCD_HEIGHT; i++)
        charChange[i] = (uint32_t)0xFFFFFFFF;
}

void LCD::displayText(uint8_t row, const char *text, uint8_t startIndex)
{
    uint8_t size = strlen(text);

    //#ifdef DEBUG
    //printf_P(PSTR("Received following text for row %d: >%s<\nText size: %d\n"), row, text, size);
    //#endif

    for (int i=0; i<size; i++)
    {
        lcdLine[row][startIndex+i] = text[i];
        bitWrite(charChange[row], startIndex+i, 1);
    }

    //#ifdef DEBUG
    //for (int i=0; i<MAX_TEXT_SIZE; i++)
    //{
        //if (lcdLine[row][i] == '\0')
            //printf_P(PSTR("EOL\n"));
        //else
            //printf_P(PSTR("%c\n"), lcdLine[row][i]);
    //}
    //#endif
}

void LCD::setMessageTime(int32_t msgTime)
{
    messageTime = -1;
}

void LCD::setDirectWriteState(bool state)
{
    directWrite = state;
}

uint8_t LCD::getTextCenter(uint8_t textSize)
{
    return LCD_WIDTH/2 - (textSize/2);
}

LCD display;
