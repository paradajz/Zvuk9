#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../version/Firmware.h"
#include "../../../version/Hardware.h"

void LCD::displayPadAmount(bool singlePad, uint8_t padNumber)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, singlePad ? padAmountSingle_string : padAmountAll_string);
    size = singlePad ? progmemCharArraySize(padAmountSingle_string) : progmemCharArraySize(padAmountAll_string);
    if (singlePad)
    {
        addSpaceToCharArray(size, 1);
        addNumberToCharArray(padNumber, size);
    }

    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
}

void LCD::displayWelcomeMessage()
{
    strcpy_P(stringBuffer, welcome_string);

    uint8_t charIndex = 0;

    while (stringBuffer[charIndex] != '\0')
    {
        //write directly to screen
        #ifdef BOARD_R1
        lcd_putc(stringBuffer[charIndex]);
        wait_ms(75);
        #elif defined (BOARD_R2)
        u8x8.drawGlyph(10+charIndex, 2, stringBuffer[charIndex]);
        wait_ms(50);
        #endif
        charIndex++;
    }

    strcpy_P(stringBuffer, welcome_string);
    strcpy(lastLCDLine[0], stringBuffer);
    wait_ms(250);
}


//text

void LCD::displayProgramAndScale(uint8_t program, uint8_t scale)
{
    //program and scale are displayed in single row
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);
    addNumberToCharArray(program, size);
    addSpaceToCharArray(size, 1);

    if ((scale >= 0) && (scale < PREDEFINED_SCALES))
    {
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[scale])));
        size += pgm_read_byte(&presetNameArray_sizes[scale]);
        strncat(stringBuffer, tempBuffer, pgm_read_byte(&presetNameArray_sizes[scale]));
    }
    else
    {
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[PREDEFINED_SCALES])));
        size += pgm_read_byte(&presetNameArray_sizes[PREDEFINED_SCALES]);
        strcat(stringBuffer, tempBuffer);
        addSpaceToCharArray(size, 1);
        addNumberToCharArray((scale - PREDEFINED_SCALES + 1), size);
    }

    updateDisplay(lcdElements.programAndScale.row, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);

    for (int i=0; i<LCD_HEIGHT; i++)
    {
        if (i == lcdElements.programAndScale.row)
            continue;

        updateDisplay(i, text, 0, true, size);
    }
}

void LCD::displayVelocity(uint8_t velocity)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, velocity_string);
    size = progmemCharArraySize(velocity_string);
    addNumberToCharArray(velocity, size);

    if (velocity < 10)
        addSpaceToCharArray(size, 2);
    else if (velocity < 100)
        addSpaceToCharArray(size, 1);

    updateDisplay(lcdElements.velocity.row, text, lcdElements.velocity.startIndex, false, size);
}

void LCD::displayAftertouch(uint8_t afterTouch)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, aftertouch_string);
    size = progmemCharArraySize(aftertouch_string);
    addNumberToCharArray(afterTouch, size);

    if (afterTouch < 10)
        addSpaceToCharArray(size, 2);
    else if (afterTouch < 100)
        addSpaceToCharArray(size, 1);

    updateDisplay(lcdElements.aftertouch.row, text, lcdElements.aftertouch.startIndex, false, size);
}

void LCD::displayXYposition(uint8_t position, padCoordinate_t type)
{
    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)
    {
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

    if (position < 10)
        addSpaceToCharArray(size, 2);
    else if (position < 100)
        addSpaceToCharArray(size, 1);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);
}

void LCD::displayXYcc(uint8_t ccXY, padCoordinate_t type)
{
    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)
    {
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

    if (ccXY < 10)
        addSpaceToCharArray(size, 2);
    else if (ccXY < 100)
        addSpaceToCharArray(size, 1);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);
}

void LCD::displayActivePadNotes(uint8_t notes[], int8_t octaves[], uint8_t numberOfNotes, bool padEditMode)
{
    uint8_t size = 0;

    //always clear notes first since they can have large size
    //issues are raised if we don't do this
    //a bit hacky...
    //if (pads.isUserScale(pads.getActiveScale()))   {

    if (padEditMode)
    {
        strcpy_P(stringBuffer, notesClear_editMode_string);
        size = progmemCharArraySize(notesClear_editMode_string);
        updateDisplay(lcdElements.notes.row, text, 0, false, size, true);
    }
    else
    {
        strcpy_P(stringBuffer, notesClear_string);
        size = progmemCharArraySize(notesClear_string);
        updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size, false);
    }

    //}

    //pad edit mode and regular mode differ in start index of notes
    //change scroll start index depending on whether pad edit mode is active
    if (!pads.getEditModeState())
        display.setScrollStart(lcdElements.notes.row, lcdElements.notes.startIndex);
    else
        display.setScrollStart(lcdElements.notes.row, 0);

    if (numberOfNotes)
    {
        for (int i=0; i<numberOfNotes; i++)
        {
            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
            //start appending after first note (don't clear string)
            if (i)
            {
                strcat(stringBuffer, tempBuffer);
                size += pgm_read_byte(&noteNameArray_sizes[notes[i]]);
            }
            else
            {
                strcpy(stringBuffer, tempBuffer);
                size = pgm_read_byte(&noteNameArray_sizes[notes[i]]);
            }

            addNumberToCharArray(octaves[i], size);
            //don't add space on last note
            if (i != numberOfNotes-1)
                addSpaceToCharArray(size, 1);
        }

        if (pads.getEditModeState())
            updateDisplay(lcdElements.notes.row, text, 0, true, size);
        else
            updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size);
    }
    else
    {
        if (padEditMode)
        {
            strcpy_P(stringBuffer, noNotes_string);
            size = strlen_P(noNotes_string);
            updateDisplay(lcdElements.notes.row, text, 0, true, size);
        }
    }
}

void LCD::displayActiveOctave(int8_t octave)
{
    //used only in pad edit mode
    strcpy_P(stringBuffer, activeOctave_string);
    uint8_t size = progmemCharArraySize(activeOctave_string);
    addNumberToCharArray(octave, size);
    updateDisplay(lcdElements.activeOctave.row, text, lcdElements.activeOctave.startIndex, true, size);
}

void LCD::displayPadEditMode(uint8_t padNumber)
{
    uint8_t size = 0;

    strcpy_P(stringBuffer, editingPad_string);
    size = progmemCharArraySize(editingPad_string);
    addSpaceToCharArray(size, 1);
    addNumberToCharArray(padNumber, size);
    updateDisplay(0, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = LCD_WIDTH;
    updateDisplay(1, text, 0, true, size);
    updateDisplay(2, text, 0, true, size);
    updateDisplay(3, text, 0, true, size);
}

void LCD::displayPad(uint8_t pad)
{
    strcpy_P(stringBuffer, padAmountSingle_string);
    uint8_t size = progmemCharArraySize(padAmountSingle_string);
    addNumberToCharArray(pad, size);
    updateDisplay(lcdElements.padNumber.row, text, lcdElements.padNumber.startIndex, false, size);
}

void LCD::displayMIDIchannel(uint8_t channel)
{
    strcpy_P(stringBuffer, midiChannel_string);
    uint8_t size = progmemCharArraySize(midiChannel_string);
    addNumberToCharArray(channel, size);

    if (channel < 10)
        addSpaceToCharArray(size, 1);
    updateDisplay(lcdElements.midiChannel.row, text, lcdElements.midiChannel.startIndex, false, size);
}

//clear

void LCD::clearVelocity()
{
    strcpy_P(stringBuffer, velocityClear_string);
    uint8_t size = progmemCharArraySize(velocityClear_string);
    updateDisplay(lcdElements.velocity.row, text, lcdElements.velocity.startIndex, false, size);
}

void LCD::clearAftertouch()
{
    strcpy_P(stringBuffer, aftertouchClear_string);
    uint8_t size = progmemCharArraySize(aftertouchClear_string);
    updateDisplay(lcdElements.aftertouch.row, text, lcdElements.aftertouch.startIndex, false, size);
}

void LCD::clearXYposition(padCoordinate_t type)
{
    strcpy_P(stringBuffer, xyPositionClear_string);
    uint8_t size = progmemCharArraySize(xyPositionClear_string);

    switch(type)
    {
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

void LCD::clearXYcc(padCoordinate_t type)
{
    strcpy_P(stringBuffer, xyCCclear_string);
    uint8_t size = progmemCharArraySize(xyCCclear_string);

    switch(type)
    {
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

void LCD::clearPadEditMode()
{
    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = LCD_WIDTH;

    for (int i=0; i<LCD_HEIGHT; i++)
        updateDisplay(i, text, 0, true, size);
}

void LCD::clearPad()
{
    strcpy_P(stringBuffer, padClear_string);
    updateDisplay(lcdElements.padNumber.row, text, lcdElements.padNumber.startIndex, false, progmemCharArraySize(padClear_string));
}

void LCD::clearPadData()
{
    strcpy_P(stringBuffer, emptyLine_string);

    for (int i=1; i<LCD_HEIGHT; i++)
        updateDisplay(i, text, 0, true, progmemCharArraySize(emptyLine_string));
}

void LCD::clearMIDIchannel()
{
    strcpy_P(stringBuffer, midiChannelClear_string);
    updateDisplay(lcdElements.midiChannel.row, text, lcdElements.midiChannel.startIndex, false, progmemCharArraySize(midiChannelClear_string));
}

void LCD::clearLine(uint8_t row)
{
    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = progmemCharArraySize(emptyLine_string);
    updateDisplay(row, text, 0, true, size);
}

void LCD::displayDeviceInfo()
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(version_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(version_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(version_revision), size);

    updateDisplay(1, text, 0, true, size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_hwVersion_string);
    size += progmemCharArraySize(deviceInfo_hwVersion_string);
    addNumberToCharArray(HARDWARE_VERSION_MAJOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_MINOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_REVISION, size);
    updateDisplay(2, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);
    updateDisplay(3, text, 0, true, size);
}

void LCD::displayFactoryResetWarning()
{
    //write directly to screen
    //lcd_clrscr();
    //lcd_gotoxy(0, 0);
//
    //strcpy_P(stringBuffer, menuOption_factoryReset_caps_string);
    //lcd_puts(stringBuffer);
    //strcpy(lastLCDLine[0], stringBuffer);
//
    //lcd_gotoxy(0, 1);
    //strcpy_P(stringBuffer, factory_reset_warning_1_string);
    //lcd_puts(stringBuffer);
    //strcpy(lastLCDLine[1], stringBuffer);
//
    //lcd_gotoxy(0, 2);
    //strcpy_P(stringBuffer, factory_reset_warning_2_string);
    //lcd_puts(stringBuffer);
    //strcpy(lastLCDLine[2], stringBuffer);
//
    //lcd_gotoxy(0, 3);
    //strcpy_P(stringBuffer, factory_reset_warning_3_string);
    //lcd_puts(stringBuffer);
    //strcpy(lastLCDLine[3], stringBuffer);
}

void LCD::displayNoteShiftLevel(int8_t level)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, noteShift_string);
    size += progmemCharArraySize(noteShift_string);
    addNumberToCharArray(level, size);
    updateDisplay(lcdElements.noteShiftLevel.row, text, lcdElements.noteShiftLevel.startIndex, false, size);
}

//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size, bool endOfLine)
{
    stringBuffer[size] = '\0'; //just a precaution

    switch(type)
    {
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