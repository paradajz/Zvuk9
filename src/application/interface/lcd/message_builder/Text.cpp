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
#include "../../../database/blocks/Scales.h"

void LCD::setupHomeScreen()
{
    clearAll();

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
    stringBuffer.startLine();
    stringBuffer.appendText("X | ");
    stringBuffer.endLine();
    updateText(LCD_ROW_PRESS_INFO_X, lcdtext_still, LCD_POSITION_PRESS_INFO_X);

    stringBuffer.startLine();
    stringBuffer.appendText("Y | ");
    stringBuffer.endLine();
    updateText(LCD_ROW_PRESS_INFO_Y, lcdtext_still, LCD_POSITION_PRESS_INFO_Y);

    //delimiter between velocity and aftertouch
    stringBuffer.startLine();
    stringBuffer.appendText("| ");
    stringBuffer.endLine();
    updateText(LCD_ROW_PRESS_INFO_DELIMITER_1, lcdtext_still, LCD_POSITION_PRESS_INFO_DELIMITER_1);

    //delimiter between xy and message type
    updateText(LCD_ROW_PRESS_INFO_X, lcdtext_still, LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER);
    updateText(LCD_ROW_PRESS_INFO_Y, lcdtext_still, LCD_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER);
}

void LCD::setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh)
{
    if (forceRefresh)
    {
        clearAll();

        stringBuffer.startLine();
        stringBuffer.appendText_P(assignedNotes_string);
        stringBuffer.endLine();

        updateText(LCD_ROW_PAD_EDIT_ASSIGNED_NOTES_INFO, lcdtext_still, getTextCenter(stringBuffer.getSize()));
    }

    stringBuffer.startLine();
    stringBuffer.appendText_P(padEditMode_string);
    stringBuffer.appendText(" | pad ");
    stringBuffer.appendInt(pad);
    stringBuffer.appendText(" | ");
    stringBuffer.appendText_P(padEditOctave_string);
    stringBuffer.appendInt(normalizeOctave(octave));

    //make sure entire row is filled
    while (stringBuffer.getSize() < LCD_WIDTH)
        stringBuffer.appendChar(' ', 1);

    stringBuffer.endLine();

    updateText(LCD_ROW_PAD_EDIT_PAD_NUMBER, lcdtext_still, getTextCenter(stringBuffer.getSize()));

    displayActivePadNotes();
}

void LCD::setupCalibrationScreen(padCoordinate_t coordinate, bool scrollCalibration)
{
    clearRow(LCD_ROW_CALIBRATION_VALUES);
    clearRow(LCD_ROW_CALIBRATION_SCROLL_INFO);
    clearRow(LCD_ROW_PRESS_INFO_PAD_NUMBER);

    displayPad();

    stringBuffer.startLine();
    stringBuffer.appendText_P(calibration_rawValue_string);
    stringBuffer.endLine();
    updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_RAW_VALUE_TEXT);

    stringBuffer.startLine();
    stringBuffer.appendText_P(calibration_midiValue_string);
    stringBuffer.endLine();
    updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_MIDI_VALUE_TEXT);

    displayCalibrationStatus(coordinate, scrollCalibration);
}

void LCD::displayCalibrationStatus(padCoordinate_t coordinate, bool status)
{
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

    stringBuffer.startLine();

    switch(status)
    {
        case true:
        if (coordinate == coordinateZ)
        {
            stringBuffer.appendText_P(pressureCalibrationHold_string);
            stringBuffer.appendChar(' ', 1);
            stringBuffer.appendInt(PRESSURE_ZONE_CALIBRATION_TIMEOUT);
            stringBuffer.appendText(" seconds");
        }
        else
        {
            stringBuffer.appendText_P(scrollCalibrationOn_string);
        }
        break;

        case false:
        stringBuffer.appendText_P(scrollCalibrationOff_string);
        break;
    }

    stringBuffer.endLine();

    updateText(LCD_ROW_CALIBRATION_SCROLL_INFO, lcdtext_still, getTextCenter(stringBuffer.getSize()));
}

void LCD::displayPressureCalibrationTime(uint8_t seconds, uint8_t zone, bool done)
{
    clearRow(LCD_ROW_CALIBRATION_SCROLL_INFO);

    stringBuffer.startLine();

    if (!done)
    {
        stringBuffer.appendText_P(pressureCalibrationInitiated_string);
        stringBuffer.appendInt(zone);
        stringBuffer.appendText(" in ");
        stringBuffer.appendInt(seconds);
    }
    else
    {
        stringBuffer.appendText_P(pressureCalibrationDone_string);
    }

    stringBuffer.endLine();

    updateText(LCD_ROW_CALIBRATION_SCROLL_INFO, lcdtext_still, getTextCenter(stringBuffer.getSize()));
}

void LCD::displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(program_string);
    stringBuffer.appendInt(program);

    while (stringBuffer.getSize() < LCD_POSITION_PROGRAM_INFO_DELIMITER_1)
        stringBuffer.appendChar(' ', 1);

    stringBuffer.appendText("| ");

    if ((scale >= 0) && (scale < PREDEFINED_SCALES))
    {
        stringBuffer.appendText_P((char*)pgm_read_word(&(presetNameArray[scale])));

        while (stringBuffer.getSize() < LCD_POSITION_PROGRAM_INFO_DELIMITER_2)
            stringBuffer.appendChar(' ', 1);

        stringBuffer.appendText("| ");
        stringBuffer.appendText_P((char*)pgm_read_word(&(noteNameArray[tonic])));

        if (scaleShift != 0)
        {
            while (stringBuffer.getSize() < LCD_POSITION_PROGRAM_INFO_DELIMITER_3)
                stringBuffer.appendChar(' ', 1);

            stringBuffer.appendText("| ");

            if (scaleShift > 0)
                stringBuffer.appendText("+");

            stringBuffer.appendInt(scaleShift);
        }
        else
        {
            while (stringBuffer.getSize() < LCD_WIDTH)
                stringBuffer.appendChar(' ', 1);
        }
    }
    else
    {
        stringBuffer.appendText_P((char*)pgm_read_word(&(presetNameArray[PREDEFINED_SCALES])));
        stringBuffer.appendChar(' ', 1);
        stringBuffer.appendInt((scale - PREDEFINED_SCALES + 1));

        //make sure the rest of the line is cleared
        while (stringBuffer.getSize() < LCD_WIDTH)
            stringBuffer.appendChar(' ', 1);
    }

    stringBuffer.endLine();

    updateText(LCD_ROW_PROGRAM_INFO_PROGRAM, lcdtext_still, LCD_POSITION_PROGRAM_INFO_PROGRAM);
}

void LCD::displayPad(uint8_t pad)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(padPress_string);

    if (pad != 255)
    {
        stringBuffer.appendInt(pad);
        stringBuffer.appendChar(' ', 1);
    }
    else
    {
        stringBuffer.appendChar(' ', 2);
    }

    stringBuffer.endLine();

    updateText(LCD_ROW_PRESS_INFO_PAD_NUMBER, lcdtext_still, LCD_POSITION_PRESS_INFO_PAD_NUMBER);
}

void LCD::displayActivePadNotes(bool showNotes)
{
    uint8_t pad = pads.getLastTouchedPad();
    uint8_t note;
    uint8_t tonic;
    int8_t octave;
    uint8_t noteCounter = 0;

    bool padEditMode = pads.getEditModeState();

    stringBuffer.startLine();

    if (showNotes)
    {
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            note = pads.getPadNote(pad, i);

            if (note == BLANK_NOTE)
                continue;

            tonic = pads.getTonicFromNote(note);
            octave = normalizeOctave(pads.getOctaveFromNote(note));

            stringBuffer.appendText_P((char*)pgm_read_word(&(noteNameArray[tonic])));

            #warning check this
            //start appending after first note (don't clear string)
            // if (noteCounter)
            // {
            //     strcat(stringBuffer, tempBuffer);
            //     size += pgm_read_byte(&noteNameArray_sizes[tonic]);
            // }
            // else
            // {
            //     strcpy(stringBuffer, tempBuffer);
            //     size = pgm_read_byte(&noteNameArray_sizes[tonic]);
            // }

            stringBuffer.appendInt(octave);
            stringBuffer.appendChar(' ', 1);
            noteCounter++;
        }

        if (!noteCounter)
        {
            stringBuffer.appendText_P(noNotes_string);
        }
    }

    //string for notes can be long - make sure entire buffer is filled
    if ((stringBuffer.getSize()+LCD_POSITION_PRESS_INFO_NOTES) < (STRING_BUFFER_SIZE-2))
        stringBuffer.appendChar(' ', STRING_BUFFER_SIZE-2-LCD_POSITION_PRESS_INFO_NOTES-stringBuffer.getSize());

    stringBuffer.endLine();

    padEditMode ? updateText(LCD_ROW_PAD_EDIT_NOTES, lcdtext_still, LCD_POSITION_PAD_EDIT_NOTES) : updateText(LCD_ROW_PRESS_INFO_NOTES, lcdtext_still, LCD_POSITION_PRESS_INFO_NOTES);
}

void LCD::displayVelocity(uint8_t midiVelocity, int16_t rawPressure)
{
    if (pads.isCalibrationEnabled())
    {
        stringBuffer.startLine();

        if (midiVelocity != 255)
            stringBuffer.appendInt(midiVelocity);

        stringBuffer.appendInt(3-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_MIDI_VALUE_VALUE);

        stringBuffer.startLine();

        if (midiVelocity != 255)
            stringBuffer.appendInt(rawPressure);

        stringBuffer.appendChar(' ', 4-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(LCD_ROW_CALIBRATION_VALUES, lcdtext_still, LCD_POSITION_CALIBRATION_RAW_VALUE_VALUE);
    }
    else
    {
        stringBuffer.startLine();
        stringBuffer.appendText_P(velocity_string);

        if (midiVelocity != 255)
            stringBuffer.appendInt(midiVelocity);

        while (stringBuffer.getSize() < (ARRAY_SIZE_CHAR(velocity_string)+3))
            stringBuffer.appendChar(' ', 1); //ensure three characters

        stringBuffer.endLine();

        updateText(LCD_ROW_PRESS_INFO_VELOCITY, lcdtext_still, LCD_POSITION_PRESS_INFO_VELOCITY);
    }
}

void LCD::displayAftertouch(uint8_t aftertouch)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(aftertouch_string);

    if (aftertouch != 255)
    {
        stringBuffer.appendInt(aftertouch);

        while (stringBuffer.getSize() < (ARRAY_SIZE_CHAR(aftertouch_string)+3))
            stringBuffer.appendChar(' ', 1); //ensure three characters
    }
    else
    {
        stringBuffer.appendChar(' ', 3); //three spaces for max three-digit value
    }

    stringBuffer.endLine();

    updateText(LCD_ROW_PRESS_INFO_ATOUCH, lcdtext_still, LCD_POSITION_PRESS_INFO_ATOUCH);
}

void LCD::displayXYvalue(padCoordinate_t type, midiMessageType_t messageType, int16_t value1, int16_t value2)
{
    uint8_t lcdCoordinate = 0, lcdRow = 0;

    if (pads.isCalibrationEnabled())
    {
        //pad calibration screen is already set, only update values
        lcdRow = LCD_ROW_CALIBRATION_VALUES;

        stringBuffer.startLine();
        lcdCoordinate = LCD_POSITION_CALIBRATION_RAW_VALUE_VALUE;

        if (value1 != 10000)
            stringBuffer.appendInt(value1);

        stringBuffer.appendChar(' ', 4-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(lcdRow, lcdtext_still, lcdCoordinate);

        stringBuffer.startLine();
        lcdCoordinate = LCD_POSITION_CALIBRATION_MIDI_VALUE_VALUE;

        if (value1 != 10000)
            stringBuffer.appendInt(value2);

        stringBuffer.appendChar(' ', 3-stringBuffer.getSize());
        stringBuffer.endLine();

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

        stringBuffer.startLine();

        if (value1 != 10000)
        {
            switch(messageType)
            {
                case midiMessageControlChange:
                stringBuffer.appendText_P(xyCC_string);
                stringBuffer.appendInt(value1);
                stringBuffer.appendChar(' ', 1);
                stringBuffer.appendInt(value2);
                break;

                case midiMessagePitchBend:
                stringBuffer.appendText_P(xyPitchBend_string);
                stringBuffer.appendChar(' ', 1);
                stringBuffer.appendInt(value1);
                break;

                default:
                return; //invalid
            }
        }

        stringBuffer.appendChar(' ', LCD_WIDTH-lcdCoordinate-stringBuffer.getSize());
        stringBuffer.endLine();
        updateText(lcdRow, lcdtext_still, lcdCoordinate);
    }
}

void LCD::displayMIDIchannel(uint8_t channel)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(function_midiChannel);

    if (channel != 255)
    {
        stringBuffer.appendInt(channel);

        while (stringBuffer.getSize() < (ARRAY_SIZE_CHAR(function_midiChannel)+2))
            stringBuffer.appendChar(' ', 1); //ensure two characters
    }
    else
    {
        stringBuffer.appendChar(' ', 2); //two spaces for max two-digit velocity value
    }

    stringBuffer.endLine();

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
    stringBuffer.startLine();
    stringBuffer.appendChar(' ', LCD_WIDTH);
    stringBuffer.endLine();
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
    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_swVersion_string);
    stringBuffer.appendInt(2);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(0);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(0);
    stringBuffer.endLine();

    updateText(LCD_ROW_MENU_DEVICE_INFO_1, lcdtext_still, LCD_POSITION_MENU_DEVICE_INFO_1);

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_hwVersion_string);
    stringBuffer.appendInt(HARDWARE_VERSION_MAJOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(HARDWARE_VERSION_MINOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(HARDWARE_VERSION_REVISION);
    stringBuffer.endLine();

    updateText(LCD_ROW_MENU_DEVICE_INFO_2, lcdtext_still, LCD_POSITION_MENU_DEVICE_INFO_2);
}

void LCD::displayFactoryResetConfirm()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(menuOption_factoryReset_title_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_TITLE, lcdtext_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_info_confirm_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_INFO_CONFIRM, lcdtext_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_pads_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_PADS, lcdtext_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_info_cancel_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_INFO_CANCEL, lcdtext_still, location);
}

void LCD::displayFactoryResetStart()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_start_1_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_1, lcdtext_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_start_2_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_2, lcdtext_still, location);
}

void LCD::displayFactoryResetEnd()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_end_1_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_1, lcdtext_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_end_2_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(LCD_ROW_FACTORY_RESET_PROGRESS_2, lcdtext_still, location);
}
