#include "../LCD.h"
#include "../../pads/Pads.h"
#include "../../../versioning/src/avr/Version.h"
#include "../../../database/blocks/Scales.h"

char stringBuffer[STRING_BUFFER_SIZE];
char tempBuffer[STRING_BUFFER_SIZE_TEMP];

void LCD::setupHomeScreen()
{
    clearAll();

    uint8_t size = 0;

    displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());

    //blank pad
    displayPad();

    //clear all notes
    displayActivePadNotes(false);

    //blank velocity
    displayVelocity();

    //blank aftertouch
    displayAftertouch();

    //blank midi channel
    displayMIDIchannel();

    //xy text
    startLine();
    size = 0;
    appendText("X | ", size);
    endLine(size);
    updateText(LCD_ROW_PRESS_INFO_X, lcdtext_still, LCD_POSITION_PRESS_INFO_X);

    startLine();
    size = 0;
    appendText("Y | ", size);
    endLine(size);
    updateText(LCD_ROW_PRESS_INFO_Y, lcdtext_still, LCD_POSITION_PRESS_INFO_Y);

    //delimiter between velocity and aftertouch
    startLine();
    size = 0;
    appendText("| ", size);
    endLine(size);
    updateText(LCD_ROW_PRESS_INFO_DELIMITER_1, lcdtext_still, LCD_POSITION_PRESS_INFO_DELIMITER_1);

    //delimiter between xy and message type
    updateText(LCD_ROW_PRESS_INFO_X, lcdtext_still, LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER);
    updateText(LCD_ROW_PRESS_INFO_Y, lcdtext_still, LCD_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER);
}

void LCD::setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh)
{
    uint8_t size;

    if (forceRefresh)
    {
        clearAll();

        strcpy_P(stringBuffer, assignedNotes_string);
        size = ARRAY_SIZE_CHAR(assignedNotes_string);

        updateText(LCD_ROW_PAD_EDIT_ASSIGNED_NOTES_INFO, lcdtext_still, getTextCenter(size));
    }

    strcpy_P(stringBuffer, padEditMode_string);
    size = ARRAY_SIZE_CHAR(padEditMode_string);
    appendText(" | pad ", size);
    addNumberToCharArray(pad, size);
    appendText(" | ", size);

    uint8_t tempSize = 0;

    strcpy_P(tempBuffer, padEditOctave_string);
    tempSize = strlen(padEditOctave_string);
    addNumberToCharArray(normalizeOctave(octave), tempSize, true);

    strcat(stringBuffer, tempBuffer);
    size += tempSize;

    //make sure entire row is filled
    while (size < LCD_WIDTH)
        addSpaceToCharArray(1, size);

    updateText(LCD_ROW_PAD_EDIT_PAD_NUMBER, lcdtext_still, getTextCenter(size));

    displayActivePadNotes();
}

void LCD::setupCalibrationScreen(padCoordinate_t coordinate, bool scrollCalibration)
{
    clearRow(LCD_ROW_CALIBRATION_VALUES);
    clearRow(LCD_ROW_CALIBRATION_SCROLL_INFO);
    clearRow(LCD_ROW_PRESS_INFO_PAD_NUMBER);

    displayPad();

    strcpy_P(stringBuffer, calibration_rawValue_string);
    updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_RAW_VALUE_TEXT);

    strcpy_P(stringBuffer, calibration_midiValue_string);
    updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_MIDI_VALUE_TEXT);

    displayCalibrationStatus(coordinate, scrollCalibration);
}

void LCD::displayCalibrationStatus(padCoordinate_t coordinate, bool status)
{
    uint8_t size = 0;

    switch(coordinate)
    {
        case coordinateX:
        case coordinateY:
        case coordinateZ:
        //fine
        break;

        default:
        return;
    }

    clearRow(LCD_ROW_CALIBRATION_SCROLL_INFO);

    switch(status)
    {
        case true:
        if (coordinate == coordinateZ)
        {
            size = ARRAY_SIZE_CHAR(pressureCalibrationHold_string);
            strcpy_P(stringBuffer, pressureCalibrationHold_string);
            addSpaceToCharArray(1, size);
            addNumberToCharArray(PRESSURE_ZONE_CALIBRATION_TIMEOUT, size);
            appendText(" seconds", size);
        }
        else
        {
            strcpy_P(stringBuffer, scrollCalibrationOn_string);
            size = ARRAY_SIZE_CHAR(scrollCalibrationOn_string);
        }
        break;

        case false:
        strcpy_P(stringBuffer, scrollCalibrationOff_string);
        size = ARRAY_SIZE_CHAR(scrollCalibrationOff_string);
        break;
    }

    updateText(LCD_ROW_CALIBRATION_SCROLL_INFO, lcdtext_still, getTextCenter(size));
}

void LCD::displayPressureCalibrationTime(uint8_t seconds, uint8_t zone, bool done)
{
    uint8_t size;

    clearRow(LCD_ROW_CALIBRATION_SCROLL_INFO);

    if (!done)
    {
        size = ARRAY_SIZE_CHAR(pressureCalibrationInitiated_string);
        strcpy_P(stringBuffer, pressureCalibrationInitiated_string);
        addNumberToCharArray(zone, size);
        appendText(" in ", size);
        addNumberToCharArray(seconds, size);
    }
    else
    {
        size = ARRAY_SIZE_CHAR(pressureCalibrationDone_string);
        strcpy_P(stringBuffer, pressureCalibrationDone_string);
    }

    updateText(LCD_ROW_CALIBRATION_SCROLL_INFO, lcdtext_still, getTextCenter(size));
}

void LCD::displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift)
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, program_string);
    size = ARRAY_SIZE_CHAR(program_string);
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
        else
        {
            while (size < LCD_WIDTH)
                addSpaceToCharArray(1, size);
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

    updateText(LCD_ROW_PROGRAM_INFO_PROGRAM, lcdtext_still, LCD_POSITION_PROGRAM_INFO_PROGRAM);
}

void LCD::displayPad(uint8_t pad)
{
    strcpy_P(stringBuffer, padPress_string);
    uint8_t size = ARRAY_SIZE_CHAR(padPress_string);

    if (pad != 255)
    {
        addNumberToCharArray(pad, size);
        addSpaceToCharArray(1, size);
    }
    else
    {
        addSpaceToCharArray(2, size);
    }

    updateText(LCD_ROW_PRESS_INFO_PAD_NUMBER, lcdtext_still, LCD_POSITION_PRESS_INFO_PAD_NUMBER);
}

void LCD::displayActivePadNotes(bool showNotes)
{
    uint8_t size = 0;
    uint8_t pad = pads.getLastTouchedPad();
    uint8_t note;
    uint8_t tonic;
    int8_t octave;
    uint8_t noteCounter = 0;

    bool padEditMode = pads.getEditModeState();

    startLine();

    if (showNotes)
    {
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

    //string for notes can be long - make sure entire buffer is filled
    if ((size+LCD_POSITION_PRESS_INFO_NOTES) < (STRING_BUFFER_SIZE-2))
        addSpaceToCharArray(STRING_BUFFER_SIZE-2-LCD_POSITION_PRESS_INFO_NOTES-size, size);

    padEditMode ? updateText(LCD_ROW_PAD_EDIT_NOTES, lcdtext_still, LCD_POSITION_PAD_EDIT_NOTES) : updateText(LCD_ROW_PRESS_INFO_NOTES, lcdtext_still, LCD_POSITION_PRESS_INFO_NOTES);
}

void LCD::displayVelocity(uint8_t midiVelocity, int16_t rawPressure)
{
    uint8_t size = 0;

    if (pads.isCalibrationEnabled())
    {
        startLine();
        size = 0;
        if (midiVelocity != 255)
            addNumberToCharArray(midiVelocity, size);
        addSpaceToCharArray(3-size, size);

        updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_MIDI_VALUE_VALUE);

        startLine();
        size = 0;

        if (midiVelocity != 255)
            addNumberToCharArray(rawPressure, size);

        addSpaceToCharArray(4-size, size);

        updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_RAW_VALUE_VALUE);
    }
    else
    {
        strcpy_P(stringBuffer, velocity_string);
        size = ARRAY_SIZE_CHAR(velocity_string);

        if (midiVelocity != 255)
            addNumberToCharArray(midiVelocity, size);

        while (size < (ARRAY_SIZE_CHAR(velocity_string)+3))
            addSpaceToCharArray(1, size); //ensure three characters

        updateText(LCD_ROW_PRESS_INFO_VELOCITY, lcdtext_still, LCD_POSITION_PRESS_INFO_VELOCITY);
    }
}

void LCD::displayAftertouch(uint8_t aftertouch)
{
    strcpy_P(stringBuffer, aftertouch_string);
    uint8_t size = ARRAY_SIZE_CHAR(aftertouch_string);

    if (aftertouch != 255)
    {
        addNumberToCharArray(aftertouch, size);

        while (size < (ARRAY_SIZE_CHAR(aftertouch_string)+3))
            addSpaceToCharArray(1, size); //ensure three characters
    }
    else
    {
        addSpaceToCharArray(3, size); //three spaces for max three-digit value
    }

    updateText(LCD_ROW_PRESS_INFO_ATOUCH, lcdtext_still, LCD_POSITION_PRESS_INFO_ATOUCH);
}

void LCD::displayXYvalue(padCoordinate_t type, midiMessageType_t messageType, int16_t value1, int16_t value2)
{
    uint8_t lcdCoordinate = 0, size = 0, lcdRow = 0;

    if (pads.isCalibrationEnabled())
    {
        //pad calibration screen is already set, only update values
        lcdRow = LCD_ROW_CALIBRATION_VALUES;

        startLine();
        size = 0;
        lcdCoordinate = LCD_POSITION_CALIBRATION_RAW_VALUE_VALUE;

        if (value1 != 10000)
            addNumberToCharArray(value1, size);

        addSpaceToCharArray(4-size, size);
        endLine(size);

        updateText(lcdRow, lcdtext_still, lcdCoordinate);

        startLine();
        size = 0;
        lcdCoordinate = LCD_POSITION_CALIBRATION_MIDI_VALUE_VALUE;

        if (value1 != 10000)
            addNumberToCharArray(value2, size);

        addSpaceToCharArray(3-size, size);
        endLine(size);

        updateText(lcdRow, lcdtext_still, lcdCoordinate);
    }
    else
    {
        switch(type)
        {
            case coordinateX:
            lcdCoordinate = LCD_POSITION_PRESS_INFO_X_VALUE;
            lcdRow = LCD_ROW_PRESS_INFO_X;
            break;

            case coordinateY:
            lcdCoordinate = LCD_POSITION_PRESS_INFO_Y_VALUE;
            lcdRow = LCD_ROW_PRESS_INFO_Y;
            break;

            default:
            return;
        }

        if (value1 != 10000)
        {
            switch(messageType)
            {
                case midiMessageControlChange:
                strcpy_P(stringBuffer, xyCC_string);
                size = ARRAY_SIZE_CHAR(xyCC_string);
                addNumberToCharArray(value1, size);
                addSpaceToCharArray(1, size);
                addNumberToCharArray(value2, size);
                break;

                case midiMessagePitchBend:
                strcpy_P(stringBuffer, xyPitchBend_string);
                size = ARRAY_SIZE_CHAR(xyPitchBend_string);
                addSpaceToCharArray(1, size);
                addNumberToCharArray(value1, size);
                break;

                default:
                return; //invalid
            }
        }

        addSpaceToCharArray(LCD_WIDTH-lcdCoordinate-size, size);
        updateText(lcdRow, lcdtext_still, lcdCoordinate);
    }
}

void LCD::displayMIDIchannel(uint8_t channel)
{
    strcpy_P(stringBuffer, function_midiChannel);
    uint8_t size = ARRAY_SIZE_CHAR(function_midiChannel);

    if (channel != 255)
    {
        addNumberToCharArray(channel, size);

        while (size < (ARRAY_SIZE_CHAR(function_midiChannel)+2))
            addSpaceToCharArray(1, size); //ensure two characters
    }
    else
    {
        addSpaceToCharArray(2, size); //two spaces for max two-digit velocity value
    }

    updateText(LCD_ROW_PRESS_INFO_MIDI_CHANNEL, lcdtext_still, LCD_POSITION_PRESS_INFO_MIDI_CHANNEL);
}

void LCD::clearPadPressData()
{
    if (!pads.isCalibrationEnabled())
    {
        displayActivePadNotes(false);
        displayAftertouch();
        displayMIDIchannel();
        displayVelocity();
    }

    displayPad();
    displayXYvalue(coordinateX);
    displayXYvalue(coordinateY);
}

void LCD::clearRow(uint8_t row)
{
    startLine();
    uint8_t size = 0;
    addSpaceToCharArray(LCD_WIDTH, size);
    endLine(size);
    updateText(row, lcdtext_still, 0);
}

void LCD::clearAll()
{
    //clear entire display
    for (int i=0; i<LCD_HEIGHT; i++)
        clearRow(i);
}

void LCD::displayDeviceInfo()
{
    uint8_t size = 0;
    strcpy_P(stringBuffer, deviceInfo_swVersion_string);
    size += ARRAY_SIZE_CHAR(deviceInfo_swVersion_string);
    addNumberToCharArray(getSWversion(swVersion_major), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_minor), size);
    appendText(".", size);
    addNumberToCharArray(getSWversion(swVersion_revision), size);

    updateText(LCD_ROW_MENU_DEVICE_INFO_1, lcdtext_still, LCD_POSITION_MENU_DEVICE_INFO_1);

    size = 0;
    strcpy_P(stringBuffer, deviceInfo_hwVersion_string);
    size += ARRAY_SIZE_CHAR(deviceInfo_hwVersion_string);
    addNumberToCharArray(HARDWARE_VERSION_MAJOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_MINOR, size);
    appendText(".", size);
    addNumberToCharArray(HARDWARE_VERSION_REVISION, size);
    updateText(LCD_ROW_MENU_DEVICE_INFO_2, lcdtext_still, LCD_POSITION_MENU_DEVICE_INFO_2);
}

void LCD::displayFactoryResetConfirm()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, menuOption_factoryReset_title_string);
    size = ARRAY_SIZE_CHAR(menuOption_factoryReset_title_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_TITLE, lcdtext_still, location);

    strcpy_P(stringBuffer, factory_reset_info_confirm_string);
    size = ARRAY_SIZE_CHAR(factory_reset_info_confirm_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_INFO_CONFIRM, lcdtext_still, location);

    strcpy_P(stringBuffer, factory_reset_pads_string);
    size = ARRAY_SIZE_CHAR(factory_reset_pads_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_PADS, lcdtext_still, location);

    strcpy_P(stringBuffer, factory_reset_info_cancel_string);
    size = ARRAY_SIZE_CHAR(factory_reset_info_cancel_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_INFO_CANCEL, lcdtext_still, location);
}

void LCD::displayFactoryResetStart()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_start_1_string);
    size = ARRAY_SIZE_CHAR(factory_reset_start_1_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_1, lcdtext_still, location);

    strcpy_P(stringBuffer, factory_reset_start_2_string);
    size = ARRAY_SIZE_CHAR(factory_reset_start_2_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_2, lcdtext_still, location);
}

void LCD::displayFactoryResetEnd()
{
    uint8_t size = 0;
    uint8_t location;

    strcpy_P(stringBuffer, factory_reset_end_1_string);
    size = ARRAY_SIZE_CHAR(factory_reset_end_1_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_1, lcdtext_still, location);

    strcpy_P(stringBuffer, factory_reset_end_2_string);
    size = ARRAY_SIZE_CHAR(factory_reset_end_2_string);
    location = getTextCenter(size);
    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_2, lcdtext_still, location);
}