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
#include "../../../versioning/src/avr/Version.h"

void LCD::displayFirmwareUpdated()
{
    strcpy_P(stringBuffer, firmware_updated);
    uint8_t size = ARRAY_SIZE_CHAR(firmware_updated);
    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(size));

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += ARRAY_SIZE_CHAR(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);

    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(size));
}

void LCD::displayWelcomeMessage()
{
    uint8_t charIndex;
    uint8_t location;

    strcpy_P(stringBuffer, deviceName_string);
    location = getTextCenter(ARRAY_SIZE_CHAR(deviceName_string));
    charIndex = 0;

    while (stringBuffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, rowMap[LCD_ROW_DEVICE_NAME_MESSAGE], stringBuffer[charIndex]);
        charIndex++;
    }

    wait_ms(1500);

    strcpy_P(stringBuffer, welcome_string);
    location = getTextCenter(ARRAY_SIZE_CHAR(welcome_string));
    charIndex = 0;

    while (stringBuffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, rowMap[LCD_ROW_WELCOME_MESSAGE], stringBuffer[charIndex]);
        wait_ms(50);
        charIndex++;
    }

    wait_ms(250);

    clearAll();
}

void LCD::displayError(function_t function, changeResult_t result)
{
    uint8_t size;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(functionErrorArray[function])));
    size = pgm_read_byte(&functionErrorArray_sizes[function]);
    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(size));

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(changeResultArray[result])));
    size = pgm_read_byte(&changeResultArray_sizes[result]);
    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(size));
}

void LCD::displayChangeResult(function_t function, int16_t value, settingType_t type)
{
    uint8_t size;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(functionArray[function])));
    size = pgm_read_byte(&functionArray_sizes[function]);

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
        addNumberToCharArray(value, size);
        break;

        case functionOctave:
        addNumberToCharArray(normalizeOctave(value), size);
        break;

        case functionNotes:
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
        strcat(stringBuffer, tempBuffer);
        size += strlen(tempBuffer);
        break;

        case functionOnOffNotes:
        case functionOnOffAftertouch:
        case functionOnOffX:
        case functionOnOffY:
        case functionOnOffSplit:
        case functionRecord:
        value > 0 ? appendText("on", size) : appendText("off", size);
        break;

        case functionXCurve:
        case functionYCurve:
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(curveNameArray[value])));
        strcat(stringBuffer, tempBuffer);
        size += strlen(tempBuffer);
        break;

        default:
        break;
    }

    updateText(LCD_ROW_MESSAGE_1, lcdText_temp, getTextCenter(size));

    switch(type)
    {
        case globalSetting:
        startLine();
        size = 0;
        addSpaceToCharArray(LCD_WIDTH, size);
        endLine(size);
        break;

        case singlePadSetting:
        strcpy_P(stringBuffer, settinType_singlePad_string);
        size = ARRAY_SIZE_CHAR(settinType_singlePad_string);
        addNumberToCharArray(pads.getLastTouchedPad() + 1, size);
        break;

        case allPadsSetting:
        strcpy_P(stringBuffer, settinType_allPads_string);
        size = ARRAY_SIZE_CHAR(settinType_allPads_string);
        break;
    }

    updateText(LCD_ROW_MESSAGE_2, lcdText_temp, getTextCenter(size));
}
