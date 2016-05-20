#include "LCD.h"

#ifdef LCD_H_
#include "../../Scales.h"
#include "../pads/Pads.h"

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
        wait(75);
        charIndex++;

    }

    strcpy(lastLCDLine[0], stringBuffer);
    wait(250);

}

//messages

void LCD::displayOutOfRange()    {

    strcpy_P(stringBuffer, outOfRange_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, progmemCharArraySize(outOfRange_string));

    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = NUMBER_OF_LCD_COLUMNS;
    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);

}

void LCD::displayModifierEnabled()   {

    strcpy_P(stringBuffer, modifierEnabled_string);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, progmemCharArraySize(modifierEnabled_string));

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

void LCD::displayCCchange(ccType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)   {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    size = pgm_read_byte(&ccArray_sizes[type]);
    addNumberToCharArray(ccValue, size);
    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCurveChange(curveCoordinate_t coordinate, bool _splitState, curveType_t type, uint8_t padNumber)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    strcpy_P(tempBuffer, (char*)pgm_read_word(&(curveNameArray[type])));

    strcat(stringBuffer, tempBuffer);

    size = pgm_read_byte(&curveCoordinateArray_sizes[(uint8_t)coordinate]) + pgm_read_byte(&curveNameArray_sizes[type]);

    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCClimitChange(ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));
    size = pgm_read_byte(&ccLimitArray_sizes[(uint8_t)type]);
    addNumberToCharArray(ccValue, size);

    updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayOnOff(functionsOnOff_t messageType, bool _splitState, uint8_t functionState, uint8_t padNumber)  {

    uint8_t size = 0;

    switch(messageType) {

        case featureNotes:
        case featureAftertouch:
        case featureX:
        case featureY:
        if (!functionState) {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(offArray[messageType])));
            size = pgm_read_byte(&offArray_sizes[messageType]);

        } else {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(onArray[messageType])));
            size = pgm_read_byte(&onArray_sizes[messageType]);

        }

        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

        displayPadAmount(_splitState, padNumber);
        break;

        case featureSplit:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(splitArray[_splitState])));
        size = pgm_read_byte(&splitArray_sizes[_splitState]);
        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

        strcpy_P(stringBuffer, emptyLine_string);
        size = NUMBER_OF_LCD_COLUMNS;
        updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
        break;

        case featureAftertouchType:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(aftertouchTypeArray[functionState])));
        size = pgm_read_byte(&aftertouchTypeArray_sizes[functionState]);
        updateDisplay(lcdElements.messageText1.row, message, lcdElements.messageText1.startIndex, true, size);

        strcpy_P(stringBuffer, emptyLine_string);
        size = NUMBER_OF_LCD_COLUMNS;
        updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
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

void LCD::displayTransportControl(transportControl_t type, bool state)  {

    uint8_t size = 0;

    switch(type)    {

        case transportRecord:
        if (state) {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[2])));
            size = pgm_read_byte(&transportControlChangeArray_sizes[2]);

        } else {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[3])));
            size = pgm_read_byte(&transportControlChangeArray_sizes[3]);

        }
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

//text

void LCD::displayProgramAndPreset(uint8_t program, uint8_t preset)   {

    //program and preset are displayed in single row
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);

    addNumberToCharArray(program, size);
    addSpaceToCharArray(size, 1);

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        size += pgm_read_byte(&presetNameArray_sizes[preset]);
        strncat(stringBuffer, tempBuffer, pgm_read_byte(&presetNameArray_sizes[preset]));

    }   else {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[NUMBER_OF_PREDEFINED_SCALES])));
        size += pgm_read_byte(&presetNameArray_sizes[NUMBER_OF_PREDEFINED_SCALES]);
        strcat(stringBuffer, tempBuffer);
        addSpaceToCharArray(size, 1);
        addNumberToCharArray((preset - NUMBER_OF_PREDEFINED_SCALES + 1), size);

    }

    updateDisplay(lcdElements.programAndPreset.row, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        if (i == lcdElements.programAndPreset.row) continue;
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

void LCD::displayXYposition(uint8_t position, ccType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)    {

        case ccTypeX:
        lcdCoordinate = lcdElements.xpos.startIndex;
        lcdRow = lcdElements.xpos.row;
        strcpy_P(stringBuffer, xPosition_string);
        size = progmemCharArraySize(xPosition_string);
        break;

        case ccTypeY:
        lcdCoordinate = lcdElements.ypos.startIndex;
        lcdRow = lcdElements.ypos.row;
        strcpy_P(stringBuffer, yPosition_string);
        size = progmemCharArraySize(yPosition_string);
        break;

    }

    addNumberToCharArray(position, size);

    if (position < 10)         addSpaceToCharArray(size, 2);
    else if (position < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);

}

void LCD::displayXYcc(uint8_t ccXY, ccType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)    {

        case ccTypeX:
        strcpy_P(stringBuffer, xCCid_string);
        size = progmemCharArraySize(xCCid_string);
        lcdCoordinate = lcdElements.ccx.startIndex;
        lcdRow = lcdElements.ccx.row;
        break;

        case ccTypeY:
        strcpy_P(stringBuffer, yCCid_string);
        size = progmemCharArraySize(yCCid_string);
        lcdCoordinate = lcdElements.ccy.startIndex;
        lcdRow = lcdElements.ccy.row;
        break;

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
    strcpy_P(stringBuffer, notesClear_string);
    size = progmemCharArraySize(notesClear_string);
    updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size, true);

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
            addSpaceToCharArray(size, 1);

        }

        if (pads.editModeActive())
            updateDisplay(lcdElements.notes.row, text, 0, true, size);
        else updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size);

    }

}

void LCD::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    strcpy_P(stringBuffer, activeOctave_string);
    uint8_t size = progmemCharArraySize(activeOctave_string);
    addNumberToCharArray(octave, size);

    updateDisplay(lcdElements.activeOctave.row, text, lcdElements.activeOctave.startIndex, true, size);

}

void LCD::displayDFUmode()   {

    //this function writes directly to screen
    lcd_clrscr();

    strcpy_P(stringBuffer, dfu_string);
    lcd_puts(stringBuffer);

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

void LCD::clearXYposition(ccType_t type)    {

    strcpy_P(stringBuffer, xyPositionClear_string);
    uint8_t size = progmemCharArraySize(xyPositionClear_string);

    switch(type)    {

        case ccTypeX:
        updateDisplay(lcdElements.xpos.row, text, lcdElements.xpos.startIndex, false, size);
        break;

        case ccTypeY:
        updateDisplay(lcdElements.ypos.row, text, lcdElements.ypos.startIndex, false, size);
        break;

    }

}

void LCD::clearXYcc(ccType_t type)  {

    strcpy_P(stringBuffer, xyCCclear_string);
    uint8_t size = progmemCharArraySize(xyCCclear_string);

    switch(type)    {

        case ccTypeX:
        updateDisplay(lcdElements.ccx.row, text, lcdElements.ccx.startIndex, false, size);
        break;

        case ccTypeY:
        updateDisplay(lcdElements.ccy.row, text, lcdElements.ccy.startIndex, false, size);
        break;

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