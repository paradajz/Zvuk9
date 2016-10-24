#include "LCD.h"

#ifdef LCD_H_
#include "../pads/Pads.h"
#include "../../version/Firmware.h"
#include "../../version/Hardware.h"

void LCD::displayPadAmount(bool singlePad, uint8_t padNumber)  {

    uint8_t size = 0;

    strcpy_P(stringBuffer, singlePad ? padAmountSingle_string : padAmountAll_string);

    size = singlePad ? progmemCharArraySize(padAmountSingle_string) : progmemCharArraySize(padAmountAll_string);

    if (singlePad) { addSpaceToCharArray(size, 1); addNumberToCharArray(padNumber, size); }

    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayHelloMessage() {

    strcpy_P(stringBuffer, welcome_string);

    uint8_t charIndex = 0;
    while (stringBuffer[charIndex] != '\0')   {

        //write directly to screen
        lcd_putc(stringBuffer[charIndex]);
        wait_ms(75);
        charIndex++;

    }

    strcpy(lastLCDLine[0], stringBuffer);
    wait_ms(250);

}

//messages

void LCD::displayOutOfRange()    {

    strcpy_P(stringBuffer, outOfRange_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, progmemCharArraySize(outOfRange_string));

    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = NUMBER_OF_LCD_COLUMNS;
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayMIDIchannelChange(uint8_t channel, bool _splitState, uint8_t padNumber) {

    uint8_t size = 0;
    strcpy_P(stringBuffer, midiChannelChange_string);
    size = progmemCharArraySize(midiChannelChange_string);
    addNumberToCharArray(channel, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCCchange(coordinateType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)   {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    size = pgm_read_byte(&ccArray_sizes[type]);
    addNumberToCharArray(ccValue, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCurveChange(coordinateType_t coordinate, bool _splitState, int8_t curveValue, uint8_t padNumber)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    size = pgm_read_byte(&curveCoordinateArray_sizes[(uint8_t)coordinate]);

    switch(curveValue)  {

        case curveTypeLinear:
        strcpy_P(tempBuffer, curveTypeLinear_string);
        strcat(stringBuffer, tempBuffer);
        size += progmemCharArraySize(curveTypeLinear_string);
        break;

        case curveTypeWideEnds:
        strcpy_P(tempBuffer, curveTypeWideEnds_string);
        strcat(stringBuffer, tempBuffer);
        size += progmemCharArraySize(curveTypeWideEnds_string);
        break;

        case curveTypeWideMiddle:
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

void LCD::displayCClimitChange(coordinateType_t coordinate, ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)  {

    //FIXME
    uint8_t size = 0;

    switch(coordinate)  {

        case coordinateX:
        switch(type)    {

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
        switch(type)    {

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

void LCD::displayOnOff(onOff_t type, bool _splitState, uint8_t functionState, uint8_t padNumber)  {

    uint8_t size = 0;

    switch(type) {

        case onOff_notes:
        case onOff_aftertouch:
        case onOff_x:
        case onOff_y:
        case onOff_split:
        if (!functionState) {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(offArray[type])));
            size = pgm_read_byte(&offArray_sizes[type]);

        } else {

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

void LCD::displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value) {

    uint8_t size = 0;

    switch(type)    {

        case tonicChange:
        switch(result)  {

            case outputChanged:
            case noChange:
            strcpy_P(stringBuffer, tonic_string);
            size = progmemCharArraySize(tonic_string);

            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            size += pgm_read_byte(&noteNameArray_sizes[value]);

            strcat(stringBuffer, tempBuffer);

            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = NUMBER_OF_LCD_COLUMNS;

            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);

            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = NUMBER_OF_LCD_COLUMNS;

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
        size = NUMBER_OF_LCD_COLUMNS;

        updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
        break;

        case noteShift:
        switch (result) {

            case outputChanged:
            strcpy_P(stringBuffer, value ? noteUp_string : noteDown_string);
            size = value ? progmemCharArraySize(noteUp_string) : progmemCharArraySize(noteDown_string);

            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = NUMBER_OF_LCD_COLUMNS;

            updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);

            updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

            strcpy_P(stringBuffer, emptyLine_string);
            size = NUMBER_OF_LCD_COLUMNS;

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

void LCD::displayEditModeNotAllowed(padEditModeResult_t errorType)   {

    uint8_t size = 0;

    switch (errorType)  {

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

void LCD::displayTransportControl(transportControl_t type)  {

    uint8_t size = 0;

    switch(type)    {

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
    size = NUMBER_OF_LCD_COLUMNS;
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayMaxNotesSet()   {

    uint8_t size = 0;

    strcpy_P(stringBuffer, maxNotesSet0_string);
    size = progmemCharArraySize(maxNotesSet0_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    strcpy_P(stringBuffer, maxNotesSet1_string);
    size = progmemCharArraySize(maxNotesSet1_string);
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayPadReleaseError(padReleaseError_t error)    {

    uint8_t size = 0;
    strcpy_P(stringBuffer, relasePad_string);
    size = progmemCharArraySize(relasePad_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    switch(error)   {

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

    }   updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayPadEditChangeParametersError()  {

    uint8_t size = 0;

    strcpy_P(stringBuffer, exitPadMode_string);
    size = progmemCharArraySize(exitPadMode_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    strcpy_P(stringBuffer, changeParameters_string);
    size = progmemCharArraySize(changeParameters_string);
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayFirmwareUpdated()  {

    strcpy_P(stringBuffer, firmware_updated);
    uint8_t size = progmemCharArraySize(firmware_updated);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);
    if (getSWversion(swVersion_development))
        appendText("d", size);
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);



}

//text

void LCD::displayProgramAndScale(uint8_t program, uint8_t scale)   {

    //program and preset are displayed in single row
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);

    addNumberToCharArray(program, size);
    addSpaceToCharArray(size, 1);

    if ((scale >= 0) && (scale < PREDEFINED_SCALES))  {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[scale])));
        size += pgm_read_byte(&presetNameArray_sizes[scale]);
        strncat(stringBuffer, tempBuffer, pgm_read_byte(&presetNameArray_sizes[scale]));

    }   else {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[PREDEFINED_SCALES])));
        size += pgm_read_byte(&presetNameArray_sizes[PREDEFINED_SCALES]);
        strcat(stringBuffer, tempBuffer);
        addSpaceToCharArray(size, 1);
        addNumberToCharArray((scale - PREDEFINED_SCALES + 1), size);

    }

    updateDisplay(lcdElements.programAndScale.row, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        if (i == lcdElements.programAndScale.row) continue;
        updateDisplay(i, text, 0, true, size);

    }

}

void LCD::displayVelocity(uint8_t velocity)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, velocity_string);
    size = progmemCharArraySize(velocity_string);
    addNumberToCharArray(velocity, size);

    if (velocity < 10)         addSpaceToCharArray(size, 2);
    else if (velocity < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(lcdElements.velocity.row, text, lcdElements.velocity.startIndex, false, size);

}

void LCD::displayAftertouch(uint8_t afterTouch)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, aftertouch_string);
    size = progmemCharArraySize(aftertouch_string);
    addNumberToCharArray(afterTouch, size);

    if (afterTouch < 10)       addSpaceToCharArray(size, 2);
    else if (afterTouch < 100) addSpaceToCharArray(size, 1);

    updateDisplay(lcdElements.aftertouch.row, text, lcdElements.aftertouch.startIndex, false, size);

}

void LCD::displayXYposition(uint8_t position, coordinateType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)    {

        case coordinateX:
        lcdCoordinate = lcdElements.xpos.startIndex;
        lcdRow = lcdElements.xpos.row;
        strcpy_P(stringBuffer, xPosition_string);
        size = progmemCharArraySize(xPosition_string);
        break;

        case coordinateY:
        lcdCoordinate = lcdElements.ypos.startIndex;
        lcdRow = lcdElements.ypos.row;
        strcpy_P(stringBuffer, yPosition_string);
        size = progmemCharArraySize(yPosition_string);
        break;

        default:
        return;

    }

    addNumberToCharArray(position, size);

    if (position < 10)         addSpaceToCharArray(size, 2);
    else if (position < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);

}

void LCD::displayXYcc(uint8_t ccXY, coordinateType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)    {

        case coordinateX:
        strcpy_P(stringBuffer, xCCid_string);
        size = progmemCharArraySize(xCCid_string);
        lcdCoordinate = lcdElements.ccx.startIndex;
        lcdRow = lcdElements.ccx.row;
        break;

        case coordinateY:
        strcpy_P(stringBuffer, yCCid_string);
        size = progmemCharArraySize(yCCid_string);
        lcdCoordinate = lcdElements.ccy.startIndex;
        lcdRow = lcdElements.ccy.row;
        break;

        default:
        return;

    }

    addNumberToCharArray(ccXY, size);

    if (ccXY < 10)         addSpaceToCharArray(size, 2);
    else if (ccXY < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);

}

void LCD::displayActivePadNotes(uint8_t notes[], int8_t octaves[], uint8_t numberOfNotes, bool padEditMode)  {

    uint8_t size = 0;

    //always clear notes first since they can have large size
    //issues are raised if we don't do this
    //a bit hacky...
    //if (pads.isUserScale(pads.getActiveScale()))   {

    strcpy_P(stringBuffer, notesClear_string);
    size = progmemCharArraySize(notesClear_string);
    padEditMode ?   updateDisplay(lcdElements.notes.row, text, 0, false, size, true) : 
                    updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size, true);

    //}

    //pad edit mode and regular mode differ in start index of notes
    //change scroll start index depending on whether pad edit mode is active
    if (!pads.editModeActive())
        display.setScrollStart(lcdElements.notes.row, lcdElements.notes.startIndex);
    else display.setScrollStart(lcdElements.notes.row, 0);

    if (numberOfNotes) {

        for (int i=0; i<numberOfNotes; i++) {

            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
            //start appending after first note (don't clear string)
            if (i) {

                strcat(stringBuffer, tempBuffer);
                size += pgm_read_byte(&noteNameArray_sizes[notes[i]]);

            } else {

                strcpy(stringBuffer, tempBuffer);
                size = pgm_read_byte(&noteNameArray_sizes[notes[i]]);

            }

            addNumberToCharArray(octaves[i], size);
            //don't add space on last note
            if (i != numberOfNotes-1)
                addSpaceToCharArray(size, 1);

        }

        if (pads.editModeActive())
            updateDisplay(lcdElements.notes.row, text, 0, true, size);
        else updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size);

    }   else {

        if (padEditMode)    {

            strcpy_P(stringBuffer, noNotes_string);
            size = strlen_P(noNotes_string);
            updateDisplay(lcdElements.notes.row, text, 0, true, size);

        }

    }

}

void LCD::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    strcpy_P(stringBuffer, activeOctave_string);
    uint8_t size = progmemCharArraySize(activeOctave_string);
    addNumberToCharArray(octave, size);

    updateDisplay(lcdElements.activeOctave.row, text, lcdElements.activeOctave.startIndex, true, size);

}

void LCD::displayPadEditMode(uint8_t padNumber)  {

    uint8_t size = 0;

    strcpy_P(stringBuffer, editingPad_string);
    size = progmemCharArraySize(editingPad_string);

    addSpaceToCharArray(size, 1);
    addNumberToCharArray(padNumber, size);

    updateDisplay(0, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = NUMBER_OF_LCD_COLUMNS;

    updateDisplay(1, text, 0, true, size);
    updateDisplay(2, text, 0, true, size);
    updateDisplay(3, text, 0, true, size);

}

void LCD::displayPad(uint8_t pad)   {

    strcpy_P(stringBuffer, padAmountSingle_string);
    uint8_t size = progmemCharArraySize(padAmountSingle_string);
    addNumberToCharArray(pad, size);

    updateDisplay(lcdElements.padNumber.row, text, lcdElements.padNumber.startIndex, false, size);

}

void LCD::displayMIDIchannel(uint8_t channel)   {

    strcpy_P(stringBuffer, midiChannel_string);
    uint8_t size = progmemCharArraySize(midiChannel_string);
    addNumberToCharArray(channel, size);

    if (channel < 10) addSpaceToCharArray(size, 1);

    updateDisplay(lcdElements.midiChannel.row, text, lcdElements.midiChannel.startIndex, false, size);

}

//clear

void LCD::clearVelocity()   {

    strcpy_P(stringBuffer, velocityClear_string);
    uint8_t size = progmemCharArraySize(velocityClear_string);
    updateDisplay(lcdElements.velocity.row, text, lcdElements.velocity.startIndex, false, size);

}

void LCD::clearAftertouch() {

    strcpy_P(stringBuffer, aftertouchClear_string);
    uint8_t size = progmemCharArraySize(aftertouchClear_string);
    updateDisplay(lcdElements.aftertouch.row, text, lcdElements.aftertouch.startIndex, false, size);

}

void LCD::clearXYposition(coordinateType_t type)    {

    strcpy_P(stringBuffer, xyPositionClear_string);
    uint8_t size = progmemCharArraySize(xyPositionClear_string);

    switch(type)    {

        case coordinateX:
        updateDisplay(lcdElements.xpos.row, text, lcdElements.xpos.startIndex, false, size);
        break;

        case coordinateY:
        updateDisplay(lcdElements.ypos.row, text, lcdElements.ypos.startIndex, false, size);
        break;

        default:
        return;

    }

}

void LCD::clearXYcc(coordinateType_t type)  {

    strcpy_P(stringBuffer, xyCCclear_string);
    uint8_t size = progmemCharArraySize(xyCCclear_string);

    switch(type)    {

        case coordinateX:
        updateDisplay(lcdElements.ccx.row, text, lcdElements.ccx.startIndex, false, size);
        break;

        case coordinateY:
        updateDisplay(lcdElements.ccy.row, text, lcdElements.ccy.startIndex, false, size);
        break;

        default:
        return;

    }

}

void LCD::clearPadEditMode()    {

    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = NUMBER_OF_LCD_COLUMNS;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true, size);

}

void LCD::clearPad()    {

    strcpy_P(stringBuffer, padClear_string);
    updateDisplay(lcdElements.padNumber.row, text, lcdElements.padNumber.startIndex, false, progmemCharArraySize(padClear_string));

}

void LCD::clearPadData()    {

    strcpy_P(stringBuffer, emptyLine_string);
    for (int i=1; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true, progmemCharArraySize(emptyLine_string));

}

void LCD::clearMIDIchannel()    {

    strcpy_P(stringBuffer, midiChannelClear_string);
    updateDisplay(lcdElements.midiChannel.row, text, lcdElements.midiChannel.startIndex, false, progmemCharArraySize(midiChannelClear_string));

}

void LCD::clearLine(uint8_t row)    {

    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = progmemCharArraySize(emptyLine_string);
    updateDisplay(row, text, 0, true, size);

}

void LCD::displayDeviceInfo()   {

    uint8_t size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);
    if (getSWversion(swVersion_development))
        appendText("d", size);
    updateDisplay(1, text, 0, true, size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_hwVersion_string);
    size += progmemCharArraySize(deviceInfo_hwVersion_string);
    addNumberToCharArray(hardwareVersion.major, size);
    appendText(".", size);
    addNumberToCharArray(hardwareVersion.minor, size);
    appendText(".", size);
    addNumberToCharArray(hardwareVersion.revision, size);
    updateDisplay(2, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);

    display.updateDisplay(3, text, 0, true, size);

}

void LCD::displayFactoryResetWarning()   {

    //write directly to screen

    lcd_clrscr();

    lcd_gotoxy(0, 0);

    strcpy_P(stringBuffer, menuOption_factoryReset_caps_string);
    lcd_puts(stringBuffer);
    strcpy(lastLCDLine[0], stringBuffer);

    lcd_gotoxy(0, 1);
    strcpy_P(stringBuffer, factory_reset_warning_1_string);
    lcd_puts(stringBuffer);
    strcpy(lastLCDLine[1], stringBuffer);

    lcd_gotoxy(0, 2);
    strcpy_P(stringBuffer, factory_reset_warning_2_string);
    lcd_puts(stringBuffer);
    strcpy(lastLCDLine[2], stringBuffer);

    lcd_gotoxy(0, 3);
    strcpy_P(stringBuffer, factory_reset_warning_3_string);
    lcd_puts(stringBuffer);
    strcpy(lastLCDLine[3], stringBuffer);

}

void LCD::displayNoteShiftLevel(int8_t level)   {

    uint8_t size = 0;
    strcpy_P(stringBuffer, noteShift_string);
    size += progmemCharArraySize(noteShift_string);
    addNumberToCharArray(level, size);
    updateDisplay(lcdElements.noteShiftLevel.row, text, lcdElements.noteShiftLevel.startIndex, false, size);

}

//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size, bool endOfLine)    {

    stringBuffer[size] = '\0'; //just a precaution

    switch(type)    {

        case text:
        display.displayText(row, stringBuffer, startIndex, overwrite, endOfLine);
        break;

        case message:
        display.displayMessage(row, stringBuffer);
        break;

        default:
        break;

    }

}

#endif