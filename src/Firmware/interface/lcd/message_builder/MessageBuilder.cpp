#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../version/Version.h"
#include "../../../database/blocks/Scales.h"

char stringBuffer[MAX_TEXT_SIZE+1];
char tempBuffer[MAX_TEXT_SIZE+1];

void LCD::clearLines(uint8_t startIndex, uint8_t endIndex)
{
    strcpy_P(stringBuffer, emptyLine_string);
    uint8_t size = progmemCharArraySize(emptyLine_string);

    for (int i=startIndex; i<LCD_HEIGHT; i++)
    {
        updateDisplay(i, text, 0, true, size);
    }
}

uint8_t LCD::getTextCenter(uint8_t textSize)
{
    return LCD_WIDTH/2 - (textSize/2);
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

    updateDisplay(lcdElements.messageText2.row, message, lcdElements.messageText2.startIndex, true, size);
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


//text

void LCD::displayProgramAndScale(uint8_t program, uint8_t scale)
{
    //program and scale are displayed in single row
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);
    addNumberToCharArray(program, size);
    addSpaceToCharArray(1, size);

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
        addSpaceToCharArray(1, size);
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
        addSpaceToCharArray(2, size);
    else if (velocity < 100)
        addSpaceToCharArray(1, size);

    updateDisplay(lcdElements.velocity.row, text, lcdElements.velocity.startIndex, false, size);
}

void LCD::displayAftertouch(uint8_t afterTouch)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, aftertouch_string);
    size = progmemCharArraySize(aftertouch_string);
    addNumberToCharArray(afterTouch, size);

    if (afterTouch < 10)
        addSpaceToCharArray(2, size);
    else if (afterTouch < 100)
        addSpaceToCharArray(1, size);

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
        addSpaceToCharArray(2, size);
    else if (position < 100)
        addSpaceToCharArray(1, size);

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
        addSpaceToCharArray(2, size);
    else if (ccXY < 100)
        addSpaceToCharArray(1, size);

    updateDisplay(lcdRow, text, lcdCoordinate, false, size);
}

void LCD::displayActivePadNotes()
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad();
    uint8_t note;
    uint8_t tonic;
    uint8_t octave;
    uint8_t noteCounter = 0;

    bool padEditMode = pads.getEditModeState();

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

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        note = pads.getPadNote(pad, i);

        if (note == BLANK_NOTE)
            continue;

        tonic = pads.getTonicFromNote(note);
        octave = normalizeOctave(pads.getOctaveFromNote(note));

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[tonic])));
        //start appending after first note (don't clear string)
        if (noteCounter)
        {
            strcat(stringBuffer, tempBuffer);
            size += pgm_read_byte(&noteNameArray_sizes[tonic]);
        }
        else
        {
            strcpy(stringBuffer, tempBuffer);
            size = pgm_read_byte(&noteNameArray_sizes[tonic]);
        }

        addNumberToCharArray(octave, size);
        addSpaceToCharArray(1, size);
        noteCounter++;
    }

    if (!noteCounter)
    {
        strcpy_P(stringBuffer, noNotes_string);
        size = strlen_P(noNotes_string);
    }

    if (padEditMode)
        updateDisplay(lcdElements.notes.row, text, 0, true, size);
    else
        updateDisplay(lcdElements.notes.row, text, lcdElements.notes.startIndex, false, size);
}

void LCD::displayActiveOctave()
{
    //used only in pad edit mode
    uint8_t octave = normalizeOctave(pads.getActiveOctave());
    strcpy_P(stringBuffer, activeOctave_string);
    uint8_t size = progmemCharArraySize(activeOctave_string);
    addNumberToCharArray(octave, size);
    updateDisplay(lcdElements.activeOctave.row, text, lcdElements.activeOctave.startIndex, true, size);
}

void LCD::displayPadEditMode()
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad()+1;

    strcpy_P(stringBuffer, editingPad_string);
    size = progmemCharArraySize(editingPad_string);
    addSpaceToCharArray(1, size);
    addNumberToCharArray(pad, size);
    updateDisplay(0, text, 0, true, size);

    strcpy_P(stringBuffer, emptyLine_string);
    size = LCD_WIDTH;
    updateDisplay(1, text, 0, true, size);
    updateDisplay(2, text, 0, true, size);
    updateDisplay(3, text, 0, true, size);
}

void LCD::displayPad()
{
    strcpy_P(stringBuffer, padAmountSingle_string);
    uint8_t size = progmemCharArraySize(padAmountSingle_string);
    uint8_t pad = pads.getLastTouchedPad()+1;
    addNumberToCharArray(pad, size);
    updateDisplay(lcdElements.padNumber.row, text, lcdElements.padNumber.startIndex, false, size);
}

void LCD::displayMIDIchannel()
{
    uint8_t channel = pads.getMIDIchannel(pads.getLastTouchedPad());

    strcpy_P(stringBuffer, midiChannel_string);
    uint8_t size = progmemCharArraySize(midiChannel_string);
    addNumberToCharArray(channel, size);

    if (channel < 10)
        addSpaceToCharArray(1, size);

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

void LCD::displayNoteShiftLevel(int8_t level)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, noteShift_string);
    size += progmemCharArraySize(noteShift_string);
    addNumberToCharArray(level, size);
    updateDisplay(lcdElements.noteShiftLevel.row, text, lcdElements.noteShiftLevel.startIndex, false, size);
}

//menu
void LCD::displayDeviceInfo()
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += progmemCharArraySize(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);

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
}

void LCD::displayFactoryResetConfirm()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, menuOption_factoryReset_caps_string);
    size = progmemCharArraySize(menuOption_factoryReset_caps_string);
    location = getTextCenter(size);
    updateDisplay(2, text, location, true, size);

    strcpy_P(stringBuffer, factory_reset_warning_1_string);
    size = progmemCharArraySize(factory_reset_warning_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, true, size);

    strcpy_P(stringBuffer, factory_reset_warning_2_string);
    size = progmemCharArraySize(factory_reset_warning_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, true, size);

    strcpy_P(stringBuffer, factory_reset_warning_3_string);
    size = progmemCharArraySize(factory_reset_warning_3_string);
    location = getTextCenter(size);
    updateDisplay(5, text, location, true, size);
}

void LCD::displayFactoryResetStart()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_start_1_string);
    size = progmemCharArraySize(factory_reset_start_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, true, size);

    strcpy_P(stringBuffer, factory_reset_start_2_string);
    size = progmemCharArraySize(factory_reset_start_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, true, size);
}

void LCD::displayFactoryResetEnd()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_end_1_string);
    size = progmemCharArraySize(factory_reset_end_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, true, size);

    strcpy_P(stringBuffer, factory_reset_end_2_string);
    size = progmemCharArraySize(factory_reset_end_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, true, size);
}

//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size, bool endOfLine)
{
    stringBuffer[size] = '\0'; //just a precaution

    switch(type)
    {
        case text:
        if (directWrite)
            u8x8.drawString(startIndex, row, stringBuffer);
        else
            display.displayText(row, stringBuffer, startIndex, overwrite, endOfLine);
        break;

        case message:
        if (directWrite)
            u8x8.drawString(startIndex, row, stringBuffer);
        else
            display.displayMessage(row, stringBuffer);
        break;

        default:
        break;
    }
}