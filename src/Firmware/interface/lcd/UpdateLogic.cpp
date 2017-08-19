#include "LCD.h"

U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8;

///
/// \brief Default constructor.
///
LCD::LCD()
{
    
}

///
/// \brief Initialize LCD driver and variables.
///
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
            lcdRowStillText[i][j] = ' ';
            lcdRowTempText[i][j] = ' ';
        }

        lcdRowStillText[i][MAX_TEXT_SIZE-1] = '\0';
        lcdRowTempText[i][MAX_TEXT_SIZE-1] = '\0';
    }
}

///
/// \brief Checks if LCD requires updating continuously.
///
bool LCD::update()
{
    if ((rTimeMs() - lastLCDupdateTime) < LCD_REFRESH_TIME)
        return false; //we don't need to update lcd in real time

    //use char pointer to point to line we're going to print
    char *charPointer;

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        if (activeTextType == lcdtext_still)
        {
            charPointer = lcdRowStillText[i];
        }
        else
        {
            //temp text - check if temp text should be removed
            if ((rTimeMs() - messageDisplayTime) < LCD_MESSAGE_DURATION)
            {
                charPointer = lcdRowTempText[i];
            }
            else
            {
                charPointer = lcdRowStillText[i];
                activeTextType = lcdtext_still;

                //make sure all characters are updated once temp text is removed
                for (int j=0; j<LCD_HEIGHT; j++)
                    charChange[j] = (uint32_t)0xFFFFFFFF;
            }

            #ifdef DEBUG
            printf_P(PSTR("message!!\n"));
            #endif
        }

        if (!charChange[i])
            continue;

        for (int j=0; j<(int)strlen(charPointer); j++)
        {
            if (bitRead(charChange[i], j))
                u8x8.drawGlyph(j, rowMap[i], charPointer[j]);
        }

        //now fill remaining columns with spaces
        for (int j=strlen(charPointer); j<LCD_WIDTH; j++)
            u8x8.drawGlyph(j, rowMap[i], ' ');

        charChange[i] = 0;
    }

    lastLCDupdateTime = rTimeMs();

    return true;
}

///
/// \brief Updates text to be shown on display.
/// This function only updates internal buffers with received text, actual updating is done in update() function.
/// Text isn't passed directly, instead, value in stringBuffer is used.
/// @param [in] row             Row which is being updated.
/// @param [in] textType        Type of text to be shown on display (enumerated type). See lcdTextType_t enumeration.
/// @param [in] startIndex      Index on which received text should on specified row.
///
void LCD::updateText(uint8_t row, lcdTextType_t textType, uint8_t startIndex)
{
    uint8_t size = strlen(stringBuffer);

    if (directWriteState)
    {
        for (int j=0; j<size; j++)
            u8x8.drawGlyph(j+startIndex, rowMap[row], stringBuffer[j]);
    }
    else
    {
        switch(textType)
        {
            case lcdtext_still:
            for (int i=0; i<size; i++)
            {
                lcdRowStillText[row][startIndex+i] = stringBuffer[i];
                bitWrite(charChange[row], startIndex+i, 1);
            }

            if ((startIndex+size) >= LCD_WIDTH)
            {
                //enable scrolling
                bitWrite(scrollEnabled, row, true);
                scrollStartIndex[row] = startIndex;
            }
            else
            {
                bitWrite(scrollEnabled, row, false);
            }
            break;

            case lcdText_temp:
            //clear entire message first
            for (int j=0; j<LCD_WIDTH-2; j++)
                lcdRowTempText[row][j] = ' ';

            lcdRowTempText[row][LCD_WIDTH-1] = '\0';

            for (int i=0; i<size; i++)
                lcdRowTempText[row][startIndex+i] = stringBuffer[i];

            //make sure message is properly EOL'ed
            lcdRowTempText[row][startIndex+size] = '\0';

            activeTextType = lcdText_temp;
            messageDisplayTime = rTimeMs();

            //update all characters on display
            for (int i=0; i<LCD_HEIGHT; i++)
                charChange[i] = (uint32_t)0xFFFFFFFF;
            break;

            default:
            return;
        }
    }
}

///
/// \brief Enables or disables direct writing to LCD.
/// When enabled, low-level APIs are used to write text to LCD directly.
/// Otherwise, update() function takes care of updating LCD.
/// @param [in] state   New direct write state.
///
void LCD::setDirectWriteState(bool state)
{
    directWriteState = state;
}
///
/// \brief Calculates position on which text needs to be set on display to be in center of display row.
/// @param [in] textSize    Size of text for which center position on display is being calculated.
/// \returns Center position of text on display.
///
uint8_t LCD::getTextCenter(uint8_t textSize)
{
    return LCD_WIDTH/2 - (textSize/2);
}

//void LCD::checkScroll(uint8_t row)
//{
    //if (!bitRead(scrollEnabled, row))
        //return;
//
    //if ((rTimeMs() - lastScrollTime) < LCD_SCROLL_TIME)
        //return;
//
    //////copy string up to scrollStartIndex
    ////for (int i=0; i<scrollStartIndex[row]; i++)
        ////lcdLineScroll[row][i] = lcdLine[row][i];
////
    //////scrollIndex is validated below
    ////for (int i=scrollStartIndex[row]; i<NUMBER_OF_LCD_COLUMNS; i++)
    ////lcdLineScroll[row][i] = lcdLine[row][i+scrollIndex[row]];
////
    ////if (scrollDirection[row])
    ////{
        ////if (((int8_t)strlen(lcdLine[row]) - 1 - scrollStartIndex[row] - scrollIndex[row]) > (NUMBER_OF_LCD_COLUMNS - scrollStartIndex[row] - 1))
        ////scrollIndex[row]++;
        ////else
        ////scrollDirection[row] = false;
    ////}
    ////else
    ////{
        ////scrollIndex[row]--;
////
        ////if (scrollIndex[row] < 0)
        ////{
            ////scrollDirection[row] = true;
            ////scrollIndex[row] = 0;
        ////}
    ////}
////
    ////lineChange[row] = true;
    ////lastScrollTime = rTimeMillis();
//}

LCD display;
