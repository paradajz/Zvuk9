#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../versioning/src/avr/Version.h"

void LCD::displayFirmwareUpdated()
{
    strcpy_P(stringBuffer, firmware_updated);
    uint8_t size = ARRAY_SIZE_CHAR(firmware_updated);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += ARRAY_SIZE_CHAR(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);

    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayWelcomeMessage()
{
    strcpy_P(stringBuffer, welcome_string);

    uint8_t charIndex = 0;
    uint8_t location = getTextCenter(ARRAY_SIZE_CHAR(welcome_string));

    while (stringBuffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, 3, stringBuffer[charIndex]);
        wait_ms(50);
        charIndex++;
    }

    wait_ms(250);
}

void LCD::displayError(function_t function, changeResult_t result)
{
    uint8_t size;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(functionErrorArray[function])));
    size = pgm_read_byte(&functionErrorArray_sizes[function]);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(changeResultArray[result])));
    size = pgm_read_byte(&changeResultArray_sizes[result]);
    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayChangeResult(function_t function, int16_t value, settingType_t type)
{
    uint8_t size;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(functionArray[function])));
    size = pgm_read_byte(&functionArray_sizes[function]);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    strcpy_P(stringBuffer, function_value);
    size = ARRAY_SIZE_CHAR(function_value);

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
        value > 0 ? appendText("Enabled", size) : appendText("Disabled", size);
        break;

        case functionXCurve:
        case functionYCurve:
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(curveNameArray[value])));
        strcat(stringBuffer, tempBuffer);
        size += strlen(tempBuffer);
        break;

        case functionPlay:
        case functionStop:
        clearLine(LCD_ROW_MESSAGE_2, false);
        break;

        case functionRecord:
        value > 0 ? appendText("On", size) : appendText("Off", size);
        break;

        default:
        break;
    }

    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);

    switch(type)
    {
        case globalSetting:
        clearLine(LCD_ROW_MESSAGE_3, false);
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

    updateDisplay(LCD_ROW_MESSAGE_3, message, getTextCenter(size), size);
}