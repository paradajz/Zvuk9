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

#include "../Display.h"
#include "../../analog/pads/Pads.h"
#include "../../../Version.h"
#include "core/src/general/Timing.h"

///
/// \brief Displays "Firmware updated" and firmware version message.
///
void Display::displayFirmwareUpdated()
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(firmware_updated);
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_1, displayText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_swVersion_string);
    stringBuffer.appendChar('v', 1);
    stringBuffer.appendInt(SW_VERSION_MAJOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(SW_VERSION_MINOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(SW_VERSION_MINOR);
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_2, displayText_temp, getTextCenter(stringBuffer.getSize()));
}

///
/// \brief Displays "Welome!" message character by character on startup.
///
void Display::displayWelcomeMessage()
{
    uint8_t charIndex;
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceName_string);
    stringBuffer.endLine();
    location = getTextCenter(stringBuffer.getSize());
    charIndex = 0;

    char* buffer = stringBuffer.getString();

    while (buffer[charIndex] != '\0')
    {
        U8X8::drawGlyph(location+charIndex, rowMap[DISPLAY_ROW_DEVICE_NAME_MESSAGE], buffer[charIndex]);
        charIndex++;
    }

    wait_ms(1500);

    stringBuffer.startLine();
    stringBuffer.appendText_P(welcome_string);
    stringBuffer.endLine();
    location = getTextCenter(stringBuffer.getSize());
    charIndex = 0;

    while (buffer[charIndex] != '\0')
    {
        U8X8::drawGlyph(location+charIndex, rowMap[DISPLAY_ROW_WELCOME_MESSAGE], buffer[charIndex]);
        wait_ms(50);
        charIndex++;
    }

    wait_ms(250);

    clearAll();
}

///
/// \brief Displays specified error message.
/// @param [in] function    Function for which error should be displayed (see function_t).
/// @param [in] result      Specific error which occured (see changeResult_t).
///
void Display::displayError(function_t function, changeResult_t result)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P((char*)pgm_read_word(&(functionErrorArray[function])));
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_1, displayText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();
    stringBuffer.appendText_P((char*)pgm_read_word(&(changeResultArray[result])));
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_2, displayText_temp, getTextCenter(stringBuffer.getSize()));
}

///
/// \brief Displays result of function change.
/// @param [in] function    Function for which result should be displayed (see function_t).
/// @param [in] value       Result of function change.
/// @param [in] type        Scope to which change is applied (see settingType_t).
///
void Display::displayChangeResult(function_t function, int16_t value, settingType_t type)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P((char*)pgm_read_word(&(functionArray[function])));

    //special cases
    switch(function)
    {
        case functionChannel:
        case functionNoteShift:
        case functionXCC:
        case functionYCC:
        case functionXMinLimit:
        case functionXMaxLimit:
        case functionYMinLimit:
        case functionYMaxLimit:
        stringBuffer.appendInt(value);
        break;

        case functionOctave:
        stringBuffer.appendInt(normalizeOctave(value));
        break;

        case functionNotes:
        stringBuffer.appendText_P((char*)pgm_read_word(&(noteNameArray[value])));
        break;

        case functionOnOffNotes:
        case functionOnOffAftertouch:
        case functionOnOffX:
        case functionOnOffY:
        case functionOnOffSplit:
        case functionRecord:
        value > 0 ? stringBuffer.appendText("on") : stringBuffer.appendText("off");
        break;

        case functionXCurve:
        case functionYCurve:
        stringBuffer.appendText_P((char*)pgm_read_word(&(curveNameArray[value])));
        break;

        default:
        break;
    }

    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_1, displayText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();

    switch(type)
    {
        case globalSetting:
        stringBuffer.appendChar(' ', DISPLAY_WIDTH);
        break;

        case singlePadSetting:
        stringBuffer.appendText_P(settinType_singlePad_string);
        stringBuffer.appendInt(pads.getLastTouchedPad() + 1);
        break;

        case allPadsSetting:
        stringBuffer.appendText_P(settinType_allPads_string);
        break;
    }

    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MESSAGE_2, displayText_temp, getTextCenter(stringBuffer.getSize()));
}
