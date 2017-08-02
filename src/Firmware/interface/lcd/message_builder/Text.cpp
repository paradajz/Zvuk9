#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../versioning/src/avr/Version.h"
#include "../../../database/blocks/Scales.h"

char stringBuffer[MAX_TEXT_SIZE+1];
char tempBuffer[MAX_TEXT_SIZE+1];

void LCD::setupHomeScreen()
{
    clearAll();

    displayProgramInfo(pads.getActiveProgram()+1, pads.getActiveScale(), pads.getActiveTonic(), pads.getScaleShiftLevel());
    displayPad();
    displayActivePadNotes(false);
    displayVelocity();
    displayAftertouch();

    displayXYcc(coordinateX);
    displayXYcc(coordinateY);

    displayXYposition(coordinateX);
    displayXYposition(coordinateY);

    displayMIDIchannel();

    //delimiter between velocity and aftertouch
    startLine();
    uint8_t size = 0;
    appendText("| ", size);
    endLine(size);
    updateDisplay(LCD_ROW_PRESS_INFO_DELIMITER_1, text, LCD_POSITION_PRESS_INFO_DELIMITER_1, size);

    //delimiter between xy cc and position
    updateDisplay(LCD_ROW_PRESS_INFO_X_CC_POS_DELIMITER, text, LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER, size);
    updateDisplay(LCD_ROW_PRESS_INFO_Y_CC_POS_DELIMITER, text, LCD_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER, size);
}

void LCD::displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);
    addNumberToCharArray(program, size);

    while (size < LCD_POSITION_PROGRAM_INFO_DELIMITER_1)
        addSpaceToCharArray(1, size);

    appendText("| ", size);

    if ((scale >= 0) && (scale < PREDEFINED_SCALES))
    {
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[scale])));
        size += pgm_read_byte(&presetNameArray_sizes[scale]);
        strncat(stringBuffer, tempBuffer, pgm_read_byte(&presetNameArray_sizes[scale]));

        while (size < LCD_POSITION_PROGRAM_INFO_DELIMITER_2)
            addSpaceToCharArray(1, size);

        appendText("| ", size);

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[tonic])));
        size += pgm_read_byte(&noteNameArray_sizes[tonic]);
        strcat(stringBuffer, tempBuffer);

        if (scaleShift != 0)
        {
            while (size < LCD_POSITION_PROGRAM_INFO_DELIMITER_3)
                addSpaceToCharArray(1, size);

            appendText("| ", size);

            if (scaleShift > 0)
                appendText("+", size);

            addNumberToCharArray(scaleShift, size);
        }
    }
    else
    {
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[PREDEFINED_SCALES])));
        size += pgm_read_byte(&presetNameArray_sizes[PREDEFINED_SCALES]);
        strcat(stringBuffer, tempBuffer);
        addSpaceToCharArray(1, size);
        addNumberToCharArray((scale - PREDEFINED_SCALES + 1), size);

        //make sure the rest of the line is cleared
        while (size < LCD_WIDTH)
            addSpaceToCharArray(1, size);
    }

    updateDisplay(LCD_ROW_PROGRAM_INFO_PROGRAM, text, LCD_POSITION_PROGRAM_INFO_PROGRAM, size);
}

void LCD::displayPad(uint8_t pad)
{
    strcpy_P(stringBuffer, padPress_string);
    uint8_t size = progmemCharArraySize(padPress_string);

    if (pad != 255)
    {
        addNumberToCharArray(pad, size);
        addSpaceToCharArray(1, size);
    }
    else
    {
        addSpaceToCharArray(2, size);
    }

    updateDisplay(LCD_ROW_PRESS_INFO_PAD_NUMBER, text, LCD_POSITION_PRESS_INFO_PAD_NUMBER, size);
}

void LCD::displayActivePadNotes(bool showNotes)
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad();
    uint8_t note;
    uint8_t tonic;
    uint8_t octave;
    uint8_t noteCounter = 0;

    bool padEditMode = pads.getEditModeState();

    startLine();

    if (showNotes)
    {
        if (padEditMode)
            clearLine(LCD_ROW_PAD_EDIT_NOTES);

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
    }

    if (padEditMode)
    {
        updateDisplay(LCD_ROW_PAD_EDIT_NOTES, text, getTextCenter(size), size);
    }
    else
    {
        while ((size+LCD_POSITION_PRESS_INFO_NOTES) < (MAX_TEXT_SIZE-1))
            addSpaceToCharArray(1, size);

        #ifdef DEBUG
        printf_P(PSTR("Notes string: <%s>\n"), stringBuffer);
        #endif

        updateDisplay(LCD_ROW_PRESS_INFO_NOTES, text, LCD_POSITION_PRESS_INFO_NOTES, size);
    }
}

void LCD::displayVelocity(uint8_t velocity)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, velocity_string);
    size = progmemCharArraySize(velocity_string);

    if (velocity != 255)
    {
        addNumberToCharArray(velocity, size);

        while (size < (progmemCharArraySize(velocity_string)+3))
            addSpaceToCharArray(1, size); //ensure three characters
    }
    else
    {
        addSpaceToCharArray(3, size); //three spaces for max three-digit value
    }

    updateDisplay(LCD_ROW_PRESS_INFO_VELOCITY, text, LCD_POSITION_PRESS_INFO_VELOCITY, size);
}

void LCD::displayAftertouch(uint8_t aftertouch)
{
    strcpy_P(stringBuffer, aftertouch_string);
    uint8_t size = progmemCharArraySize(aftertouch_string);

    if (aftertouch != 255)
    {
        addNumberToCharArray(aftertouch, size);

        while (size < (progmemCharArraySize(aftertouch_string)+3))
            addSpaceToCharArray(1, size); //ensure three characters
    }
    else
    {
        addSpaceToCharArray(3, size); //three spaces for max three-digit value
    }

    updateDisplay(LCD_ROW_PRESS_INFO_ATOUCH, text, LCD_POSITION_PRESS_INFO_ATOUCH, size);
}

void LCD::displayXYcc(padCoordinate_t type, uint8_t cc)
{
    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    switch(type)
    {
        case coordinateX:
        strcpy_P(stringBuffer, xCCid_string);
        size = progmemCharArraySize(xCCid_string);
        lcdCoordinate = LCD_POSITION_PRESS_INFO_X_CC;
        lcdRow = LCD_ROW_PRESS_INFO_X_CC;
        break;

        case coordinateY:
        strcpy_P(stringBuffer, yCCid_string);
        size = progmemCharArraySize(yCCid_string);
        lcdCoordinate = LCD_POSITION_PRESS_INFO_Y_CC;
        lcdRow = LCD_ROW_PRESS_INFO_Y_CC;
        break;

        default:
        return;
    }

    if (cc != 255)
    {
        addNumberToCharArray(cc, size);

        while (size < (progmemCharArraySize(xCCid_string)+3))
            addSpaceToCharArray(1, size); //ensure three characters
    }
    else
    {
        addSpaceToCharArray(3, size); //three spaces for max three-digit value
    }

    updateDisplay(lcdRow, text, lcdCoordinate, size);
}

void LCD::displayXYposition(padCoordinate_t type, uint8_t position)
{
    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    //we aren't performing strcpy here so clear string buffer manually
    startLine();

    switch(type)
    {
        case coordinateX:
        lcdCoordinate = LCD_POSITION_PRESS_INFO_X_POS;
        lcdRow = LCD_ROW_PRESS_INFO_X_POS;
        break;

        case coordinateY:
        lcdCoordinate = LCD_POSITION_PRESS_INFO_Y_POS;
        lcdRow = LCD_ROW_PRESS_INFO_Y_POS;
        break;

        default:
        return;
    }

    if (position != 255)
    {
        addNumberToCharArray(position, size);

        while (size < 3)
            addSpaceToCharArray(1, size); //ensure three characters
    }
    else
    {
        addSpaceToCharArray(3, size); //three spaces for max three-digit value
    }

    endLine(size);

    updateDisplay(lcdRow, text, lcdCoordinate, size);
}

void LCD::displayMIDIchannel(uint8_t channel)
{
    strcpy_P(stringBuffer, midiChannel_string);
    uint8_t size = progmemCharArraySize(midiChannel_string);

    if (channel != 255)
    {
        addNumberToCharArray(channel, size);

        while (size < (progmemCharArraySize(midiChannel_string)+2))
            addSpaceToCharArray(1, size); //ensure two characters
    }
    else
    {
        addSpaceToCharArray(2, size); //two spaces for max two-digit velocity value
    }

    updateDisplay(LCD_ROW_PRESS_INFO_MIDI_CHANNEL, text, LCD_POSITION_PRESS_INFO_MIDI_CHANNEL, size);
}

void LCD::clearPadPressData()
{
    displayPad();
    displayActivePadNotes(false);
    displayVelocity();
    displayAftertouch();
    displayXYcc(coordinateX);
    displayXYcc(coordinateY);
    displayXYposition(coordinateX);
    displayXYposition(coordinateY);
    displayMIDIchannel();
}

void LCD::clearLine(uint8_t row, bool writeToDisplay)
{
    startLine();
    uint8_t size = 0;
    addSpaceToCharArray(LCD_WIDTH, size);

    if (writeToDisplay)
        updateDisplay(row, text, 0, size);
}

void LCD::clearAll()
{
    //clear entire display
    for (int i=0; i<LCD_HEIGHT; i++)
    clearLine(i);
}

void LCD::setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh)
{
    uint8_t size;

    if (forceRefresh)
    {
        clearAll();

        strcpy_P(stringBuffer, assignedNotes_string);
        size = progmemCharArraySize(assignedNotes_string);

        updateDisplay(LCD_ROW_PAD_EDIT_ASSIGNED_NOTES_INFO, text, getTextCenter(size), size);
    }

    strcpy_P(stringBuffer, padEditMode_string);
    size = progmemCharArraySize(padEditMode_string);
    appendText(" | pad ", size);
    addNumberToCharArray(pad, size);
    appendText(" | ", size);

    uint8_t tempSize = 0;

    strcpy_P(tempBuffer, padEditOctave_string);
    tempSize = strlen(padEditOctave_string);
    addNumberToCharArray(normalizeOctave(octave), tempSize, true);

    strcat(stringBuffer, tempBuffer);
    size += tempSize;

    updateDisplay(LCD_ROW_PAD_EDIT_PAD_NUMBER, text, getTextCenter(size), size);

    displayActivePadNotes();
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

    updateDisplay(LCD_ROW_MENU_DEVICE_INFO_1, text, LCD_POSITION_MENU_DEVICE_INFO_1, size);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_hwVersion_string);
    size += progmemCharArraySize(deviceInfo_hwVersion_string);
    addNumberToCharArray(HARDWARE_VERSION_MAJOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_MINOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_REVISION, size);
    updateDisplay(LCD_ROW_MENU_DEVICE_INFO_2, text, LCD_POSITION_MENU_DEVICE_INFO_2, size);
}

void LCD::displayFactoryResetConfirm()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, menuOption_factoryReset_caps_string);
    size = progmemCharArraySize(menuOption_factoryReset_caps_string);
    location = getTextCenter(size);
    updateDisplay(2, text, location, size);

    strcpy_P(stringBuffer, factory_reset_warning_1_string);
    size = progmemCharArraySize(factory_reset_warning_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, size);

    strcpy_P(stringBuffer, factory_reset_warning_2_string);
    size = progmemCharArraySize(factory_reset_warning_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, size);

    strcpy_P(stringBuffer, factory_reset_warning_3_string);
    size = progmemCharArraySize(factory_reset_warning_3_string);
    location = getTextCenter(size);
    updateDisplay(5, text, location, size);
}

void LCD::displayFactoryResetStart()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_start_1_string);
    size = progmemCharArraySize(factory_reset_start_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, size);

    strcpy_P(stringBuffer, factory_reset_start_2_string);
    size = progmemCharArraySize(factory_reset_start_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, size);
}

void LCD::displayFactoryResetEnd()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_end_1_string);
    size = progmemCharArraySize(factory_reset_end_1_string);
    location = getTextCenter(size);
    updateDisplay(3, text, location, size);

    strcpy_P(stringBuffer, factory_reset_end_2_string);
    size = progmemCharArraySize(factory_reset_end_2_string);
    location = getTextCenter(size);
    updateDisplay(4, text, location, size);
}

void LCD::displayScrollCalibrationStatus(bool status)
{
    uint8_t size = 0;

    switch(status)
    {
        case true:
        strcpy_P(stringBuffer, scrollCalibrationOn_string);
        size = progmemCharArraySize(scrollCalibrationOn_string);
        break;

        case false:
        strcpy_P(stringBuffer, scrollCalibrationOff_string);
        size = progmemCharArraySize(scrollCalibrationOff_string);
        break;
    }

    uint8_t location = getTextCenter(size);

    updateDisplay(LCD_HEIGHT-1, text, location, size);
}

void LCD::displayPressureCalibrationTime(uint8_t seconds, uint8_t pressureZone, bool done)
{
    uint8_t size;

    if (!done)
    {
        size = progmemCharArraySize(pressureCalibrationInitiated_string);
        strcpy_P(stringBuffer, pressureCalibrationInitiated_string);
        addSpaceToCharArray(1, size);
        addNumberToCharArray(pressureZone, size);
        appendText(" in ", size);
        addNumberToCharArray(seconds, size);
    }
    else
    {
        size = progmemCharArraySize(pressureCalibrationDone1_string);
        strcpy_P(stringBuffer, pressureCalibrationDone1_string);
        addSpaceToCharArray(1, size);
        addNumberToCharArray(pressureZone, size);
        addSpaceToCharArray(1, size);
        size += progmemCharArraySize(pressureCalibrationDone2_string);
        strcpy_P(tempBuffer, pressureCalibrationDone2_string);
        strcat(stringBuffer, tempBuffer);
    }

    uint8_t location = getTextCenter(size);

    updateDisplay(LCD_HEIGHT-1, text, location, size);
}

void LCD::displayPressureCalibrationStatus(bool status)
{
    uint8_t size = 0;

    if (!status)
    {
        size = progmemCharArraySize(pressureCalibrationOff_string);
        strcpy_P(stringBuffer, pressureCalibrationOff_string);
    }
    else
    {
        size = progmemCharArraySize(pressureCalibrationHold_string);
        strcpy_P(stringBuffer, pressureCalibrationHold_string);
        addSpaceToCharArray(1, size);
        addNumberToCharArray(PRESSURE_ZONE_CALIBRATION_TIMEOUT, size);
        appendText(" seconds", size);
    }

    uint8_t location = getTextCenter(size);

    updateDisplay(LCD_HEIGHT-1, text, location, size);
}

//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, uint8_t size)
{
    stringBuffer[size] = '\0'; //just a precaution

    switch(type)
    {
        case text:
        if (directWrite)
            u8x8.drawString(startIndex, row, stringBuffer);
        else
            display.displayText(row, stringBuffer, startIndex);
        break;

        case message:
        if (directWrite)
            u8x8.drawString(startIndex, row, stringBuffer);
        else
            display.displayMessage(row, stringBuffer, startIndex);
        break;

        default:
        break;
    }
}