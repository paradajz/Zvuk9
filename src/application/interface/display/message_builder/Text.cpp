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

#include "../Display.h"
#include "../Variables.h"
#include "../../analog/pads/Pads.h"
#include "../../../database/blocks/Scales.h"
#include "../../../Version.h"

///
/// \brief Displays home screen.
///
void Display::setupHomeScreen()
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
    updateText(DISPLAY_ROW_PRESS_INFO_X, displayText_still, DISPLAY_POSITION_PRESS_INFO_X);

    stringBuffer.startLine();
    stringBuffer.appendText("Y | ");
    stringBuffer.endLine();
    updateText(DISPLAY_ROW_PRESS_INFO_Y, displayText_still, DISPLAY_POSITION_PRESS_INFO_Y);

    //delimiter between velocity and aftertouch
    stringBuffer.startLine();
    stringBuffer.appendText("| ");
    stringBuffer.endLine();
    updateText(DISPLAY_ROW_PRESS_INFO_DELIMITER_1, displayText_still, DISPLAY_POSITION_PRESS_INFO_DELIMITER_1);

    //delimiter between xy and message type
    updateText(DISPLAY_ROW_PRESS_INFO_X, displayText_still, DISPLAY_POSITION_PRESS_INFO_X_CC_POS_DELIMITER);
    updateText(DISPLAY_ROW_PRESS_INFO_Y, displayText_still, DISPLAY_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER);
}

///
/// \brief Displays pad edit screen.
/// @param [in] pad             Pad for which pad edit screen should be set.
/// @param [in] octave          Currently active octave in pad edit mode.
/// @param [in] forceRefresh    If set to true, entire screen will be cleared first.
///                             Otherwise, only pad and its notes will be refreshed.
///
void Display::setupPadEditScreen(uint8_t pad, uint8_t octave, bool forceRefresh)
{
    if (forceRefresh)
    {
        clearAll();

        stringBuffer.startLine();
        stringBuffer.appendText_P(assignedNotes_string);
        stringBuffer.endLine();

        updateText(DISPLAY_ROW_PAD_EDIT_ASSIGNED_NOTES_INFO, displayText_still, getTextCenter(stringBuffer.getSize()));
    }

    stringBuffer.startLine();
    stringBuffer.appendText_P(padEditMode_string);
    stringBuffer.appendText(" | pad ");
    stringBuffer.appendInt(pad);
    stringBuffer.appendText(" | ");
    stringBuffer.appendText_P(padEditOctave_string);
    stringBuffer.appendInt(normalizeOctave(octave));

    //make sure entire row is filled
    while (stringBuffer.getSize() < DISPLAY_WIDTH)
        stringBuffer.appendChar(' ', 1);

    stringBuffer.endLine();

    updateText(DISPLAY_ROW_PAD_EDIT_PAD_NUMBER, displayText_still, getTextCenter(stringBuffer.getSize()));

    displayActivePadNotes();
}

///
/// \brief Displays pad calibration screen.
/// @param [in] coordinate  Coordinate for which pad calibration screen should be set up.
/// @param [in] intFlag     Interactive calibration flag. If set to true, text indicating that
///                         user should scroll on pad to actually calibrate it will be displayed,
///                         but only for X and Y coordinate. If Z coordinate is specified,
///                         message indicating that user should hold pad pressed for several seconds
///                         is displayed instead.
///
void Display::setupCalibrationScreen(padCoordinate_t coordinate, bool intFlag)
{
    clearRow(DISPLAY_ROW_CALIBRATION_VALUES);
    clearRow(DISPLAY_ROW_CALIBRATION_SCROLL_INFO);
    clearRow(DISPLAY_ROW_PRESS_INFO_PAD_NUMBER);

    displayPad();

    stringBuffer.startLine();
    stringBuffer.appendText_P(calibration_rawValue_string);
    stringBuffer.endLine();
    updateText(DISPLAY_ROW_CALIBRATION_VALUES, displayText_still, DISPLAY_POSITION_CALIBRATION_RAW_VALUE_TEXT);

    stringBuffer.startLine();
    stringBuffer.appendText_P(calibration_midiValue_string);
    stringBuffer.endLine();
    updateText(DISPLAY_ROW_CALIBRATION_VALUES, displayText_still, DISPLAY_POSITION_CALIBRATION_MIDI_VALUE_TEXT);

    displayCalibrationStatus(coordinate, intFlag);
}

///
/// \brief Displays current status of pad calibration.
/// @param [in] coordinate  Coordinate for which pad calibration status is displayed.
/// @param [in] intFlag     Interactive calibration flag. If set to true, text indicating that
///                         user should scroll on pad to actually calibrate it will be displayed,
///                         but only for X and Y coordinate. If Z coordinate is specified,
///                         message indicating that user should hold pad pressed for several seconds
///                         is displayed instead.
///
void Display::displayCalibrationStatus(padCoordinate_t coordinate, bool intFlag)
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

    clearRow(DISPLAY_ROW_CALIBRATION_SCROLL_INFO);

    stringBuffer.startLine();

    switch(intFlag)
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

    updateText(DISPLAY_ROW_CALIBRATION_SCROLL_INFO, displayText_still, getTextCenter(stringBuffer.getSize()));
}

///
/// \brief Used to display remaining time before applied pressure is saved as calibration value.
/// @param [in] seconds     Number of seconds left.
/// @param [in] zone        Active pressure zone.
/// @param [in] done        When set to true, message indicating that calibration is done will
///                         be displayed. Otherwise, remaining amount of seconds will be displayed instead.
///
void Display::displayPressureCalibrationTime(uint8_t seconds, uint8_t zone, bool done)
{
    clearRow(DISPLAY_ROW_CALIBRATION_SCROLL_INFO);

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

    updateText(DISPLAY_ROW_CALIBRATION_SCROLL_INFO, displayText_still, getTextCenter(stringBuffer.getSize()));
}

///
/// \brief Displays program, scale, tonic and scale shift value stored in requested program on home screen.
/// @param [in] program     Program which should be displayed.
/// @param [in] scale       Active scale.
/// @param [in] tonic       Active tonic.
/// @param [in] scaleShift  Active value of scale shift. If scale isn't shifted, this value isn't displayed.
///
void Display::displayProgramInfo(uint8_t program, uint8_t scale, note_t tonic, int8_t scaleShift)
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(program_string);
    stringBuffer.appendInt(program);

    while (stringBuffer.getSize() < DISPLAY_POSITION_PROGRAM_INFO_DELIMITER_1)
        stringBuffer.appendChar(' ', 1);

    stringBuffer.appendText("| ");

    if ((scale >= 0) && (scale < PREDEFINED_SCALES))
    {
        stringBuffer.appendText_P((char*)pgm_read_word(&(presetNameArray[scale])));

        while (stringBuffer.getSize() < DISPLAY_POSITION_PROGRAM_INFO_DELIMITER_2)
            stringBuffer.appendChar(' ', 1);

        stringBuffer.appendText("| ");
        stringBuffer.appendText_P((char*)pgm_read_word(&(noteNameArray[tonic])));

        if (scaleShift != 0)
        {
            while (stringBuffer.getSize() < DISPLAY_POSITION_PROGRAM_INFO_DELIMITER_3)
                stringBuffer.appendChar(' ', 1);

            stringBuffer.appendText("| ");

            if (scaleShift > 0)
                stringBuffer.appendText("+");

            stringBuffer.appendInt(scaleShift);
        }
        else
        {
            while (stringBuffer.getSize() < DISPLAY_WIDTH)
                stringBuffer.appendChar(' ', 1);
        }
    }
    else
    {
        stringBuffer.appendText_P((char*)pgm_read_word(&(presetNameArray[PREDEFINED_SCALES])));
        stringBuffer.appendChar(' ', 1);
        stringBuffer.appendInt((scale - PREDEFINED_SCALES + 1));

        //make sure the rest of the line is cleared
        while (stringBuffer.getSize() < DISPLAY_WIDTH)
            stringBuffer.appendChar(' ', 1);
    }

    stringBuffer.endLine();

    updateText(DISPLAY_ROW_PROGRAM_INFO_PROGRAM, displayText_still, DISPLAY_POSITION_PROGRAM_INFO_PROGRAM);
}

///
/// \brief Displays pad number on home screen.
/// @param [in] pad Pad number. If pad 255 is specified (default value)
///                 pad number will be cleared.
///
void Display::displayPad(uint8_t pad)
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

    updateText(DISPLAY_ROW_PRESS_INFO_PAD_NUMBER, displayText_still, DISPLAY_POSITION_PRESS_INFO_PAD_NUMBER);
}

///
/// \brief Displays all assigned notes for last pressed pad in home screen.
/// @param [in] showNotes   If set to true, assigned notes will be displayed,
///                         otherwise, notes will be cleared instead.
///
void Display::displayActivePadNotes(bool showNotes)
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
    if ((stringBuffer.getSize()+DISPLAY_POSITION_PRESS_INFO_NOTES) < (STRING_BUFFER_SIZE-2))
        stringBuffer.appendChar(' ', STRING_BUFFER_SIZE-2-DISPLAY_POSITION_PRESS_INFO_NOTES-stringBuffer.getSize());

    stringBuffer.endLine();

    padEditMode ? updateText(DISPLAY_ROW_PAD_EDIT_NOTES, displayText_still, DISPLAY_POSITION_PAD_EDIT_NOTES) : updateText(DISPLAY_ROW_PRESS_INFO_NOTES, displayText_still, DISPLAY_POSITION_PRESS_INFO_NOTES);
}

///
/// \brief Displays velocity.
/// When called during calibration, both MIDI and raw values are displayed so that
/// it's easier to calibrate pressure.
/// @param [in] midiVelocity    MIDI velocity value.
/// @param [in] rawPressure     Raw velocity value as retrieved from board ADC.
///
void Display::displayVelocity(uint8_t midiVelocity, int16_t rawPressure)
{
    if (pads.isCalibrationEnabled())
    {
        stringBuffer.startLine();

        if (midiVelocity != 255)
            stringBuffer.appendInt(midiVelocity);

        stringBuffer.appendInt(3-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(DISPLAY_ROW_CALIBRATION_VALUES, displayText_still, DISPLAY_POSITION_CALIBRATION_MIDI_VALUE_VALUE);

        stringBuffer.startLine();

        if (midiVelocity != 255)
            stringBuffer.appendInt(rawPressure);

        stringBuffer.appendChar(' ', 4-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(DISPLAY_ROW_CALIBRATION_VALUES, displayText_still, DISPLAY_POSITION_CALIBRATION_RAW_VALUE_VALUE);
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

        updateText(DISPLAY_ROW_PRESS_INFO_VELOCITY, displayText_still, DISPLAY_POSITION_PRESS_INFO_VELOCITY);
    }
}

///
/// \brief Displays aftertouch value for last pressed pad in home screen.
/// @param [in] aftertouch  Aftertouch value. If value 255 is specified (default),
///                         aftertouch value will be cleared.
///
void Display::displayAftertouch(uint8_t aftertouch)
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

    updateText(DISPLAY_ROW_PRESS_INFO_ATOUCH, displayText_still, DISPLAY_POSITION_PRESS_INFO_ATOUCH);
}

///
/// \brief Displays X and Y values.
/// @param [in] type        Coordinate which should be updated (X or Y).
/// @param [in] messageType Type of X/Y value (pitch bend or CC, see midiMessageType_t).
/// @param [in] value1      In user mode, used as CC number or pitch bend value. In calibration mode, used as
///                         X/Y MIDI value.
/// @param [in] value2      In user mode, used as CC value. When pitch bend is used, this value is ignored.
///                         In calibration mode this value is used as raw ADC value for X/Y.
///
void Display::displayXYvalue(padCoordinate_t type, midiMessageType_t messageType, int16_t value1, int16_t value2)
{
    uint8_t displayCoordinate = 0, displayRow = 0;

    if (pads.isCalibrationEnabled())
    {
        //pad calibration screen is already set, only update values
        displayRow = DISPLAY_ROW_CALIBRATION_VALUES;

        stringBuffer.startLine();
        displayCoordinate = DISPLAY_POSITION_CALIBRATION_RAW_VALUE_VALUE;

        if (value1 != 10000)
            stringBuffer.appendInt(value1);

        stringBuffer.appendChar(' ', 4-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(displayRow, displayText_still, displayCoordinate);

        stringBuffer.startLine();
        displayCoordinate = DISPLAY_POSITION_CALIBRATION_MIDI_VALUE_VALUE;

        if (value1 != 10000)
            stringBuffer.appendInt(value2);

        stringBuffer.appendChar(' ', 3-stringBuffer.getSize());
        stringBuffer.endLine();

        updateText(displayRow, displayText_still, displayCoordinate);
    }
    else
    {
        switch(type)
        {
            case coordinateX:
            displayCoordinate = DISPLAY_POSITION_PRESS_INFO_X_VALUE;
            displayRow = DISPLAY_ROW_PRESS_INFO_X;
            break;

            case coordinateY:
            displayCoordinate = DISPLAY_POSITION_PRESS_INFO_Y_VALUE;
            displayRow = DISPLAY_ROW_PRESS_INFO_Y;
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

        stringBuffer.appendChar(' ', DISPLAY_WIDTH-displayCoordinate-stringBuffer.getSize());
        stringBuffer.endLine();
        updateText(displayRow, displayText_still, displayCoordinate);
    }
}

///
/// \brief Displays MIDI channel on home screen.
/// @param [in] channel     MIDI channel to display.
///                         If channel is set to 255 (set by default) MIDI channel is cleared.
///
void Display::displayMIDIchannel(uint8_t channel)
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

    updateText(DISPLAY_ROW_PRESS_INFO_MIDI_CHANNEL, displayText_still, DISPLAY_POSITION_PRESS_INFO_MIDI_CHANNEL);
}

///
/// \brief Clears all data on display related to pad press.
/// Pad number, notes, aftertouch, MIDI channel, velocity, X and Y data is cleared.
///
void Display::clearPadPressData()
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

///
/// \brief Clears specified row on display.
/// @param [in] row     Row on display to clear.
///
void Display::clearRow(uint8_t row)
{
    stringBuffer.startLine();
    stringBuffer.appendChar(' ', DISPLAY_WIDTH);
    stringBuffer.endLine();
    updateText(row, displayText_still, 0);
}

///
/// \brief Clears entire display.
///
void Display::clearAll()
{
    //clear entire display
    for (int i=0; i<DISPLAY_HEIGHT; i++)
        clearRow(i);
}

///
/// \brief Displays firmware and hardware version in user and service menu.
///
void Display::displayDeviceInfo()
{
    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_swVersion_string);
    stringBuffer.appendInt(SW_VERSION_MAJOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(SW_VERSION_MINOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(SW_VERSION_REVISION);
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MENU_DEVICE_INFO_1, displayText_still, DISPLAY_POSITION_MENU_DEVICE_INFO_1);

    stringBuffer.startLine();
    stringBuffer.appendText_P(deviceInfo_hwVersion_string);
    stringBuffer.appendInt(HARDWARE_VERSION_MAJOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(HARDWARE_VERSION_MINOR);
    stringBuffer.appendText(".");
    stringBuffer.appendInt(HARDWARE_VERSION_REVISION);
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_MENU_DEVICE_INFO_2, displayText_still, DISPLAY_POSITION_MENU_DEVICE_INFO_2);
}

///
/// \brief Displays confirmation screen before performing factory reset.
///
void Display::displayFactoryResetConfirm()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(menuOption_factoryReset_title_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_TITLE, displayText_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_info_confirm_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_INFO_CONFIRM, displayText_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_pads_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_PADS, displayText_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_info_cancel_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_INFO_CANCEL, displayText_still, location);
}

///
/// \brief Displays initial factory reset phase message.
///
void Display::displayFactoryResetStart()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_start_1_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_PROGRESS_1, displayText_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_start_2_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_PROGRESS_2, displayText_still, location);
}

///
/// \brief Displays final factory reset phase message.
///
void Display::displayFactoryResetEnd()
{
    uint8_t location;

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_end_1_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_PROGRESS_1, displayText_still, location);

    stringBuffer.startLine();
    stringBuffer.appendText_P(factory_reset_end_2_string);
    location = getTextCenter(stringBuffer.getSize());
    stringBuffer.endLine();

    updateText(DISPLAY_ROW_FACTORY_RESET_PROGRESS_2, displayText_still, location);
}
