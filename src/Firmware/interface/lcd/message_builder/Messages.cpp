#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../version/Firmware.h"
#include "../../../version/Hardware.h"

void LCD::displayOutOfRange()
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, outOfRange_string);
    size = progmemCharArraySize(outOfRange_string);

    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = LCD_WIDTH;
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayMIDIchannelChange(uint8_t channel, bool _splitState, uint8_t padNumber)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, midiChannelChange_string);
    size = progmemCharArraySize(midiChannelChange_string);

    addNumberToCharArray(channel, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
    displayPadAmount(_splitState, padNumber);
    #ifdef BOARD_R2
    display.setMessageTime(INFINITE_MESSAGE_TIME);
    #endif
}

void LCD::displayCCchange(padCoordinate_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    size = pgm_read_byte(&ccArray_sizes[type]);
    addNumberToCharArray(ccValue, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
    displayPadAmount(_splitState, padNumber);
}

void LCD::displayCurveChange(padCoordinate_t coordinate, bool _splitState, int8_t curveValue, uint8_t padNumber)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    size = pgm_read_byte(&curveCoordinateArray_sizes[(uint8_t)coordinate]);

    switch(curveValue)
    {
        case curveLinear:
        strcpy_P(tempBuffer, curveTypeLinear_string);
        strcat(stringBuffer, tempBuffer);
        size += progmemCharArraySize(curveTypeLinear_string);
        break;

        case curveWideEnds:
        strcpy_P(tempBuffer, curveTypeWideEnds_string);
        strcat(stringBuffer, tempBuffer);
        size += progmemCharArraySize(curveTypeWideEnds_string);
        break;

        case curveWideMiddle:
        strcpy_P(tempBuffer, curveTypeWideMiddle_string);
        strcat(stringBuffer, tempBuffer);
        size += progmemCharArraySize(curveTypeWideMiddle_string);
        break;

        default:
        break;

    }

    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
    displayPadAmount(_splitState, padNumber);
}

void LCD::displayCClimitChange(padCoordinate_t coordinate, ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)
{
    uint8_t size = 0;

    switch(coordinate)
    {
        case coordinateX:
        switch(type)
        {
            case ccLimitTypeMin:
            strcpy_P(stringBuffer, xMin_string);
            size = progmemCharArraySize(xMin_string);
            break;

            case ccLimitTypeMax:
            strcpy_P(stringBuffer, xMax_string);
            size = progmemCharArraySize(xMax_string);
            break;
        }
        break;

        case coordinateY:
        switch(type)
        {
            case ccLimitTypeMin:
            strcpy_P(stringBuffer, yMin_string);
            size = progmemCharArraySize(yMin_string);
            break;

            case ccLimitTypeMax:
            strcpy_P(stringBuffer, yMax_string);
            size = progmemCharArraySize(yMax_string);
            break;
        }
        break;

        default:
        return;
    }

    addNumberToCharArray(ccValue, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
    displayPadAmount(_splitState, padNumber);
}

void LCD::displayOnOff(onOff_t type, bool _splitState, uint8_t functionState, uint8_t padNumber)
{
    uint8_t size = 0;

    switch(type)
    {
        case onOff_notes:
        case onOff_aftertouch:
        case onOff_x:
        case onOff_y:
        case onOff_split:
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

        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

        if (type != onOff_split)
        displayPadAmount(_splitState, padNumber);
        break;

        default:
        break;
    }
}

void LCD::displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value)
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
            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = LCD_WIDTH;
            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = LCD_WIDTH;
            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
            break;

            default:
            return;
        }
        break;

        case octaveChange:
        //always display active octave, ignore out of range here
        strcpy_P(stringBuffer, octave_string);
        size = progmemCharArraySize(octave_string);
        addNumberToCharArray(value, size);

        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
        strcpy_P(stringBuffer, emptyLine_string);
        size = LCD_WIDTH;
        updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
        break;

        case noteShift:
        switch (result)
        {
            case outputChanged:
            strcpy_P(stringBuffer, notesShifted_string);
            size = progmemCharArraySize(notesShifted_string);
            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, notesShiftLevel_string);
            size = progmemCharArraySize(notesShiftLevel_string);
            addNumberToCharArray(value, size);
            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = LCD_WIDTH;
            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
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
        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

        strcpy_P(stringBuffer, editModeEnterError1_string);
        size = progmemCharArraySize(editModeEnterError1_string);
        updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
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
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
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

    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);
    strcpy_P(stringBuffer, emptyLine_string);
    size = LCD_WIDTH;
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayMaxNotesSet()
{
    uint8_t size = 0;

    strcpy_P(stringBuffer, maxNotesSet0_string);
    size = progmemCharArraySize(maxNotesSet0_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    strcpy_P(stringBuffer, maxNotesSet1_string);
    size = progmemCharArraySize(maxNotesSet1_string);
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayPadReleaseError(padReleaseError_t error)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, relasePad_string);
    size = progmemCharArraySize(relasePad_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

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

    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayPadEditChangeParametersError()
{
    uint8_t size = 0;

    strcpy_P(stringBuffer, exitPadMode_string);
    size = progmemCharArraySize(exitPadMode_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    strcpy_P(stringBuffer, changeParameters_string);
    size = progmemCharArraySize(changeParameters_string);
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayFirmwareUpdated()
{
    strcpy_P(stringBuffer, firmware_updated);
    uint8_t size = progmemCharArraySize(firmware_updated);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(version_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(version_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(version_revision), size);

    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}