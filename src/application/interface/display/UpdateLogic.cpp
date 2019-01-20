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

#include "Display.h"
#include "core/src/general/Timing.h"
#include "core/src/general/BitManipulation.h"

StringBuffer stringBuffer;

///
/// \brief Default constructor.
///
Display::Display()
{
    
}

///
/// \brief Initialize display driver and variables.
///
void Display::init()
{
    U8X8::initDisplay();
    U8X8::setFlipMode(1);
    U8X8::setFont(u8x8_font_pressstart2p_r);
    U8X8::clearDisplay();

    //init char arrays
    for (int i=0; i<DISPLAY_HEIGHT; i++)
    {
        for (int j=0; j<STRING_BUFFER_SIZE-2; j++)
        {
            displayRowStillText[i][j] = ' ';
            displayRowTempText[i][j] = ' ';
        }

        displayRowStillText[i][STRING_BUFFER_SIZE-1] = '\0';
        displayRowTempText[i][STRING_BUFFER_SIZE-1] = '\0';

        scrollEvent[i].size = 0;
        scrollEvent[i].startIndex = 0;
        scrollEvent[i].currentIndex = 0;
        scrollEvent[i].direction = scroll_ltr;
    }
}

///
/// \brief Checks if display requires updating continuously.
///
bool Display::update()
{
    if ((rTimeMs() - lastDisplayUpdateTime) < DISPLAY_REFRESH_TIME)
        return false; //we don't need to update display in real time

    //use char pointer to point to line we're going to print
    char *charPointer;

    updateTempTextStatus();

    for (int i=0; i<DISPLAY_HEIGHT; i++)
    {
        if (activeTextType == displayText_still)
        {
            //scrolling is possible only with still text
            updateScrollStatus(i);
            charPointer = displayRowStillText[i];
        }
        else
        {
            charPointer = displayRowTempText[i];
        }

        if (!charChange[i])
            continue;

        int8_t string_len = strlen(charPointer) > DISPLAY_WIDTH ? DISPLAY_WIDTH : strlen(charPointer);

        for (int j=0; j<string_len; j++)
        {
            if (BIT_READ(charChange[i], j))
                U8X8::drawGlyph(j, rowMap[i], charPointer[j+scrollEvent[i].currentIndex]);
        }

        //now fill remaining columns with spaces
        for (int j=string_len; j<DISPLAY_WIDTH; j++)
            U8X8::drawGlyph(j, rowMap[i], ' ');

        charChange[i] = 0;
    }

    lastDisplayUpdateTime = rTimeMs();

    return true;
}

///
/// \brief Updates text to be shown on display.
/// This function only updates internal buffers with received text, actual updating is done in update() function.
/// Text isn't passed directly, instead, value in stringBuffer is used.
/// @param [in] row             Row which is being updated.
/// @param [in] textType        Type of text to be shown on display (enumerated type). See displayTextType_t enumeration.
/// @param [in] startIndex      Index on which received text should on specified row.
///
void Display::updateText(uint8_t row, displayTextType_t textType, uint8_t startIndex)
{
    uint8_t size = stringBuffer.getSize();
    uint8_t scrollSize = 0;

    if (size+startIndex >= STRING_BUFFER_SIZE-2)
        size = STRING_BUFFER_SIZE-2-startIndex; //trim string

    if (directWriteState)
    {
        char* buffer = stringBuffer.getString();

        for (int j=0; j<size; j++)
            U8X8::drawGlyph(j+startIndex, rowMap[row], buffer[j]);
    }
    else
    {
        bool scrollingEnabled = false;
        char* buffer = stringBuffer.getString();

        switch(textType)
        {
            case displayText_still:
            for (int i=0; i<size; i++)
            {
                displayRowStillText[row][startIndex+i] = buffer[i];
                BIT_WRITE(charChange[row], startIndex+i, 1);
            }

            //scrolling is enabled only if some characters are found after DISPLAY_WIDTH-1 index
            for (int i=DISPLAY_WIDTH; i<STRING_BUFFER_SIZE-1; i++)
            {
                if ((displayRowStillText[row][i] != ' ') && (displayRowStillText[row][i] != '\0'))
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

            case displayText_temp:
            //clear entire message first
            for (int j=0; j<DISPLAY_WIDTH-2; j++)
                displayRowTempText[row][j] = ' ';

            displayRowTempText[row][DISPLAY_WIDTH-1] = '\0';

            for (int i=0; i<size; i++)
                displayRowTempText[row][startIndex+i] = buffer[i];

            //make sure message is properly EOL'ed
            displayRowTempText[row][startIndex+size] = '\0';

            activeTextType = displayText_temp;
            messageDisplayTime = rTimeMs();

            //update all characters on display
            for (int i=0; i<DISPLAY_HEIGHT; i++)
                charChange[i] = (uint32_t)0xFFFFFFFF;
            break;

            default:
            return;
        }
    }
}

///
/// \brief Enables or disables direct writing to display.
/// When enabled, low-level APIs are used to write text to display directly.
/// Otherwise, update() function takes care of updating display.
/// @param [in] state   New direct write state.
///
void Display::setDirectWriteState(bool state)
{
    directWriteState = state;
}
///
/// \brief Calculates position on which text needs to be set on display to be in center of display row.
/// @param [in] textSize    Size of text for which center position on display is being calculated.
/// \returns Center position of text on display.
///
uint8_t Display::getTextCenter(uint8_t textSize)
{
    return DISPLAY_WIDTH/2 - (textSize/2);
}

///
/// \brief Updates status of temp text on display.
///
void Display::updateTempTextStatus()
{
    if (activeTextType == displayText_temp)
    {
        //temp text - check if temp text should be removed
        if ((rTimeMs() - messageDisplayTime) > DISPLAY_MESSAGE_DURATION)
        {
            activeTextType = displayText_still;

            //make sure all characters are updated once temp text is removed
            for (int j=0; j<DISPLAY_HEIGHT; j++)
                charChange[j] = (uint32_t)0xFFFFFFFF;
        }
    }
}

///
/// \brief Updates status of scrolling text on display.
/// @param [in] row     Row which is being checked.
///
void Display::updateScrollStatus(uint8_t row)
{
    if (!scrollEvent[row].size)
        return;

    if ((rTimeMs() - lastScrollTime) < DISPLAY_SCROLL_TIME)
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

    for (int i=scrollEvent[row].startIndex; i<DISPLAY_WIDTH; i++)
        BIT_WRITE(charChange[row], i, 1);

    lastScrollTime = rTimeMs();
}

///
/// \brief Checks for currently active text type on display.
/// \returns Active text type (enumerated type). See displayTextType_t enumeration.
///
displayTextType_t Display::getActiveTextType()
{
    return activeTextType;
}

Display display;
