/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

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
        for (int j=0; j<STRING_BUFFER_SIZE-2; j++)
        {
            lcdRowStillText[i][j] = ' ';
            lcdRowTempText[i][j] = ' ';
        }

        lcdRowStillText[i][STRING_BUFFER_SIZE-1] = '\0';
        lcdRowTempText[i][STRING_BUFFER_SIZE-1] = '\0';

        scrollEvent[i].size = 0;
        scrollEvent[i].startIndex = 0;
        scrollEvent[i].currentIndex = 0;
        scrollEvent[i].direction = scroll_ltr;
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

    updateTempTextStatus();

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        if (activeTextType == lcdtext_still)
        {
            //scrolling is possible only with still text
            updateScrollStatus(i);
            charPointer = lcdRowStillText[i];
        }
        else
        {
            charPointer = lcdRowTempText[i];
        }

        if (!charChange[i])
            continue;

        int8_t string_len = strlen(charPointer) > LCD_WIDTH ? LCD_WIDTH : strlen(charPointer);

        for (int j=0; j<string_len; j++)
        {
            if (bitRead(charChange[i], j))
                u8x8.drawGlyph(j, rowMap[i], charPointer[j+scrollEvent[i].currentIndex]);
        }

        //now fill remaining columns with spaces
        for (int j=string_len; j<LCD_WIDTH; j++)
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
    uint8_t scrollSize = 0;

    if (size+startIndex >= STRING_BUFFER_SIZE-2)
        size = STRING_BUFFER_SIZE-2-startIndex; //trim string

    if (directWriteState)
    {
        for (int j=0; j<size; j++)
            u8x8.drawGlyph(j+startIndex, rowMap[row], stringBuffer[j]);
    }
    else
    {
        bool scrollingEnabled = false;

        switch(textType)
        {
            case lcdtext_still:
            for (int i=0; i<size; i++)
            {
                lcdRowStillText[row][startIndex+i] = stringBuffer[i];
                bitWrite(charChange[row], startIndex+i, 1);
            }

            //scrolling is enabled only if some characters are found after LCD_WIDTH-1 index
            for (int i=LCD_WIDTH; i<STRING_BUFFER_SIZE-1; i++)
            {
                if ((lcdRowStillText[row][i] != ' ') && (lcdRowStillText[row][i] != '\0'))
                {
                    scrollingEnabled = true;
                    scrollSize++;
                }
            }

            if (scrollingEnabled && !scrollEvent[row].size)
            {
                //enable scrolling
                scrollEvent[row].size = scrollSize;
                scrollEvent[row].startIndex = startIndex;
                scrollEvent[row].currentIndex = 0;
                scrollEvent[row].direction = scroll_ltr;

                lastScrollTime = rTimeMs();
            }
            else if (!scrollingEnabled && scrollEvent[row].size)
            {
                scrollEvent[row].size = 0;
                scrollEvent[row].startIndex = 0;
                scrollEvent[row].currentIndex = 0;
                scrollEvent[row].direction = scroll_ltr;
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

///
/// \brief Updates status of temp text on display.
///
void LCD::updateTempTextStatus()
{
    if (activeTextType == lcdText_temp)
    {
        //temp text - check if temp text should be removed
        if ((rTimeMs() - messageDisplayTime) > LCD_MESSAGE_DURATION)
        {
            activeTextType = lcdtext_still;

            //make sure all characters are updated once temp text is removed
            for (int j=0; j<LCD_HEIGHT; j++)
                charChange[j] = (uint32_t)0xFFFFFFFF;
        }
    }
}

///
/// \brief Updates status of scrolling text on display.
/// @param [in] row     Row which is being checked.
///
void LCD::updateScrollStatus(uint8_t row)
{
    if (!scrollEvent[row].size)
        return;

    if ((rTimeMs() - lastScrollTime) < LCD_SCROLL_TIME)
        return;

    switch(scrollEvent[row].direction)
    {
        case scroll_ltr:
        //left to right
        scrollEvent[row].currentIndex++;

        if (scrollEvent[row].currentIndex == scrollEvent[row].size)
        {
            //switch direction
            scrollEvent[row].direction = scroll_rtl;
        }
        break;

        case scroll_rtl:
        //right to left
        scrollEvent[row].currentIndex--;

        if (scrollEvent[row].currentIndex == 0)
        {
            //switch direction
            scrollEvent[row].direction = scroll_ltr;
        }
        break;
    }

    for (int i=scrollEvent[row].startIndex; i<LCD_WIDTH; i++)
        bitWrite(charChange[row], i, 1);

    lastScrollTime = rTimeMs();
}

///
/// \brief Checks for currently active text type on display.
/// \returns Active text type (enumerated type). See lcdTextType_t enumeration.
///
lcdTextType_t LCD::getActiveTextType()
{
    return activeTextType;
}

LCD display;
