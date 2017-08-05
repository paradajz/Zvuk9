#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../versioning/src/avr/Version.h"

void LCD::displayOutOfRange()
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, outOfRange_string);
    size = progmemCharArraySize(outOfRange_string);

    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
}

void LCD::displayMIDIchannelChange()
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad();
    uint8_t channel = pads.getMIDIchannel(pad);
    strcpy_P(stringBuffer, midiChannelChange_string);
    size = progmemCharArraySize(midiChannelChange_string);

    addNumberToCharArray(channel, size);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);

    displayPadAmount(pad+1);
    display.setMessageTime(INFINITE_MESSAGE_TIME);
}

void LCD::displayCCchange(padCoordinate_t type, uint8_t ccValue)
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad()+1;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    size = pgm_read_byte(&ccArray_sizes[type]);
    addNumberToCharArray(ccValue, size);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);

    displayPadAmount(pad);
}

void LCD::displayCurveChange(padCoordinate_t coordinate)
{
    uint8_t size = 0;
    uint8_t padNumber = pads.getLastTouchedPad();

    curve_t curve = pads.getCCcurve(padNumber, coordinate);
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    size = pgm_read_byte(&curveCoordinateArray_sizes[(uint8_t)coordinate]);

    strcpy_P(tempBuffer, (char*)pgm_read_word(&(curveNameArray[(uint8_t)curve])));
    strcat(stringBuffer, tempBuffer);
    size += pgm_read_byte(&curveNameArray_sizes[(uint8_t)curve]);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);

    displayPadAmount(padNumber+1);
}

void LCD::displayCClimitChange(padCoordinate_t coordinate, limitType_t type, uint8_t ccValue)
{
    uint8_t size = 0;
    uint8_t padNumber = pads.getLastTouchedPad()+1;

    switch(coordinate)
    {
        case coordinateX:
        switch(type)
        {
            case limitTypeMin:
            strcpy_P(stringBuffer, xMin_string);
            size = progmemCharArraySize(xMin_string);
            break;

            case limitTypeMax:
            strcpy_P(stringBuffer, xMax_string);
            size = progmemCharArraySize(xMax_string);
            break;
        }
        break;

        case coordinateY:
        switch(type)
        {
            case limitTypeMin:
            strcpy_P(stringBuffer, yMin_string);
            size = progmemCharArraySize(yMin_string);
            break;

            case limitTypeMax:
            strcpy_P(stringBuffer, yMax_string);
            size = progmemCharArraySize(yMax_string);
            break;
        }
        break;

        default:
        return;
    }

    addNumberToCharArray(ccValue, size);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);

    displayPadAmount(padNumber);
}

void LCD::displayOnOffChange(function_t type, uint8_t functionState)
{
    uint8_t size = 0;
    uint8_t padNumber = pads.getLastTouchedPad()+1;

    switch(type)
    {
        case function_notes:
        case function_aftertouch:
        case function_x:
        case function_y:
        case function_split:
        if (!functionState)
        {
            strcpy_P(stringBuffer, (char*)pgm_read_word(&(offArray[type])));
            size = pgm_read_byte(&offArray_sizes[type]);
        }
        else
        {
            strcpy_P(stringBuffer, (char*)pgm_read_word(&(onArray[type])));
            size = pgm_read_byte(&onArray_sizes[type]);
        }

        updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

        if (type != function_split)
        {
            displayPadAmount(padNumber);
        }
        else
        {
            clearLine(LCD_ROW_MESSAGE_2, false);
            updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
        }
        break;

        default:
        break;
    }
}

void LCD::displayNoteChange(changeResult_t result, noteChangeType_t type, int8_t value)
{
    uint8_t size = 0;

    switch(type)
    {
        case tonicChange:
        switch(result)
        {
            case outputChanged:
            case noChange:
            strcpy_P(stringBuffer, tonic_string);
            size = progmemCharArraySize(tonic_string);
            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            size += pgm_read_byte(&noteNameArray_sizes[value]);
            strcat(stringBuffer, tempBuffer);
            updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

            clearLine(LCD_ROW_MESSAGE_2, false);
            updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

            clearLine(LCD_ROW_MESSAGE_2, false);
            updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
            break;

            default:
            return;
        }
        break;

        case octaveChange:
        //always display active octave, ignore out of range here
        strcpy_P(stringBuffer, octave_string);
        size = progmemCharArraySize(octave_string);
        addNumberToCharArray(normalizeOctave(value), size);
        updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

        clearLine(LCD_ROW_MESSAGE_2, false);
        updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
        break;

        case noteShift:
        switch (result)
        {
            case outputChanged:
            strcpy_P(stringBuffer, notesShifted_string);
            size = progmemCharArraySize(notesShifted_string);
            updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

            strcpy_P(stringBuffer, notesShiftLevel_string);
            size = progmemCharArraySize(notesShiftLevel_string);

            if (value > 0)
            {
                appendText("+", size);
                addNumberToCharArray(value, size);
            }
            else if (value == 0)
            {
                strcpy_P(tempBuffer, notesShiftNoLevel_string);
                strcat(stringBuffer, tempBuffer);
                size += progmemCharArraySize(notesShiftNoLevel_string);
            }
            else
            {
                addNumberToCharArray(value, size);
            }

            updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

            clearLine(LCD_ROW_MESSAGE_2, false);
            updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
            break;

            default:
            return;
        }
        break;

        default:
        break;
    }
}

void LCD::displayEditModeNotAllowed(padEditModeResult_t errorType)
{
    uint8_t size = 0;

    switch (errorType)
    {
        case notUserPreset:
        strcpy_P(stringBuffer, editModeEnterError0_string);
        size = progmemCharArraySize(editModeEnterError0_string);
        updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

        strcpy_P(stringBuffer, editModeEnterError1_string);
        size = progmemCharArraySize(editModeEnterError1_string);
        updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
        break;

        case padNotReleased:
        displayPadReleaseError(enterPadEditMode);
        break;

        default:
        return;
    }
}

void LCD::displayNoNotesError()
{
    strcpy_P(stringBuffer, noNotes_string);
    uint8_t size = progmemCharArraySize(noNotes_string);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);
}

void LCD::displayTransportControl(transportControl_t type)
{
    uint8_t size = 0;

    switch(type)
    {
        case transportRecordOn:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[2])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[2]);
        break;

        case transportRecordOff:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[3])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[3]);
        break;

        case transportPlay:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[0])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[0]);
        break;

        case transportStop:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[1])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[1]);
        break;
    }

    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    clearLine(LCD_ROW_MESSAGE_2, false);
    updateDisplay(LCD_ROW_MESSAGE_2, message, LCD_POSITION_MESSAGE_2, size);
}

void LCD::displayMaxNotesSet()
{
    uint8_t size = 0;

    strcpy_P(stringBuffer, maxNotesSet0_string);
    size = progmemCharArraySize(maxNotesSet0_string);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    strcpy_P(stringBuffer, maxNotesSet1_string);
    size = progmemCharArraySize(maxNotesSet1_string);
    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayPadReleaseError(padReleaseError_t error)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, relasePad_string);
    size = progmemCharArraySize(relasePad_string);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    switch(error)
    {
        case changeProgram:
        strcpy_P(stringBuffer, changeProgram_string);
        size = progmemCharArraySize(changeProgram_string);
        break;

        case changePreset:
        strcpy_P(stringBuffer, changePreset_string);
        size = progmemCharArraySize(changePreset_string);
        break;

        case changeCCnumber:
        strcpy_P(stringBuffer, changeCC_string);
        size = progmemCharArraySize(changeCC_string);
        break;

        case changeCClimit:
        strcpy_P(stringBuffer, changeCClimit_string);
        size = progmemCharArraySize(changeCClimit_string);
        break;

        case changeCurve:
        strcpy_P(stringBuffer, changeCurve_string);
        size = progmemCharArraySize(changeCurve_string);
        break;

        case enterPadEditMode:
        strcpy_P(stringBuffer, enterPadEditMode_string);
        size = progmemCharArraySize(enterPadEditMode_string);
        break;

        case calibrationMode:
        strcpy_P(stringBuffer, calibrationMode_string);
        size = progmemCharArraySize(calibrationMode_string);
        break;

        default:
        return;
    }

    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayPadEditChangeParametersError()
{
    uint8_t size = 0;

    strcpy_P(stringBuffer, exitPadMode_string);
    size = progmemCharArraySize(exitPadMode_string);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    strcpy_P(stringBuffer, changeParameters_string);
    size = progmemCharArraySize(changeParameters_string);
    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayFirmwareUpdated()
{
    strcpy_P(stringBuffer, firmware_updated);
    uint8_t size = progmemCharArraySize(firmware_updated);
    updateDisplay(LCD_ROW_MESSAGE_1, message, getTextCenter(size), size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);

    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayPadAmount(uint8_t padNumber)
{
    uint8_t size = 0;

    bool singlePad = pads.getSplitState();

    strcpy_P(stringBuffer, singlePad ? padAmountSingle_string : padAmountAll_string);
    size = singlePad ? progmemCharArraySize(padAmountSingle_string) : progmemCharArraySize(padAmountAll_string);

    if (singlePad)
    {
        addSpaceToCharArray(1, size);
        addNumberToCharArray(padNumber, size);
    }

    updateDisplay(LCD_ROW_MESSAGE_2, message, getTextCenter(size), size);
}

void LCD::displayWelcomeMessage()
{
    strcpy_P(stringBuffer, welcome_string);

    uint8_t charIndex = 0;
    uint8_t location = getTextCenter(progmemCharArraySize(welcome_string));

    while (stringBuffer[charIndex] != '\0')
    {
        u8x8.drawGlyph(location+charIndex, 3, stringBuffer[charIndex]);
        wait_ms(50);
        charIndex++;
    }

    wait_ms(250);
}