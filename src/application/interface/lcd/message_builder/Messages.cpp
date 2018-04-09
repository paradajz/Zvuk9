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

#include "../LCD.h"
#include "../../pads/Pads.h"

void LCD::displayFirmwareUpdated()
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(firmware_updated);
    stringBuffer.endLine();

    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_swVersion_string);
    stringBuffer.appendInt(2);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(0);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(0);
    stringBuffer.endLine();

    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(stringBuffer.getSize()));
}

void LCD::displayWelcomeMessage()
{
    uint8_t charIndex;
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceName_string);
    stringBuffer.endLine();
    location = getTextCenter(stringBuffer.getSize());
    charIndex = 0;

    while (stringBuffer.buffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, rowMap[LCD_ROW_DEVICE_NAME_MESSAGE], stringBuffer.buffer[charIndex]);
        charIndex++;
    }

    wait_ms(1500);

    stringBuffer.startLine();
    stringBuffer.appendText_P(welcome_string);
    stringBuffer.endLine();
    location = getTextCenter(stringBuffer.getSize());
    charIndex = 0;

    while (stringBuffer.buffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, rowMap[LCD_ROW_WELCOME_MESSAGE], stringBuffer.buffer[charIndex]);
        wait_ms(50);
        charIndex++;
    }

    wait_ms(250);

    clearAll();
}

void LCD::displayError(function_t function, changeResult_t result)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P((char*)pgm_read_word(&(functionErrorArray[function])));
    stringBuffer.endLine();

    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();
    stringBuffer.appendText_P((char*)pgm_read_word(&(changeResultArray[result])));
    stringBuffer.endLine();

    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(stringBuffer.getSize()));
}

void LCD::displayChangeResult(function_t function, int16_t value, settingType_t type)
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

    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(stringBuffer.getSize()));

    stringBuffer.startLine();

    switch(type)
    {
        case globalSetting:
        stringBuffer.appendChar(' ', LCD_WIDTH);
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

    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(stringBuffer.getSize()));
}
