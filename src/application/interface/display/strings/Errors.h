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

#pragma once

///
/// \brief Display strings used to indicate various errors.
/// \ingroup displayStrings
/// @{

const char error_changeProgram_string[] PROGMEM = "Program change error:";
const char error_changeScale_string[] PROGMEM = "Scale change error:";
const char error_changeMIDIchannel_string[] PROGMEM = "MIDI channel change error";
const char error_play_string[] PROGMEM = "Play error:";
const char error_stop_string[] PROGMEM = "Stop error:";
const char error_record_string[] PROGMEM = "Record error:";
const char error_noteChange_string[] PROGMEM = "Note change error:";
const char error_splitMode_string[] PROGMEM = "Split mode change error:";
const char error_sendX_string[] PROGMEM = "X send mode change error:";
const char error_sendY_string[] PROGMEM = "Y send mode change error:";
const char error_sendAftertouch_string[] PROGMEM = "Aftertouch send mode change error:";
const char error_sendNotes_string[] PROGMEM = "Note send mode change error:";
const char error_octaveChange_string[] PROGMEM = "Octave change error:";
const char error_noteShift_string[] PROGMEM = "Note shift change error:";
const char error_padEdit_string[] PROGMEM = "Pad edit mode entry error:";
const char error_ccX_string[] PROGMEM = "X CC change error:";
const char error_ccY_string[] PROGMEM = "Y CC change error:";
const char error_xMin_string[] PROGMEM = "X min change error:";
const char error_xMax_string[] PROGMEM = "X max change error:";
const char error_yMin_string[] PROGMEM = "Y min change error:";
const char error_yMax_string[] PROGMEM = "Y max change error:";
const char error_xCurve_string[] PROGMEM = "X curve change error:";
const char error_yCurve_string[] PROGMEM = "Y curve change error:";
const char error_menu_string[] PROGMEM = "Menu entry error:";

PGM_P const functionErrorArray[] PROGMEM =
{
    error_changeProgram_string,
    error_changeScale_string,
    error_changeMIDIchannel_string,
    error_play_string,
    error_stop_string,
    error_record_string,
    error_noteChange_string,
    error_splitMode_string,
    error_sendX_string,
    error_sendY_string,
    error_sendAftertouch_string,
    error_sendNotes_string,
    error_octaveChange_string,
    error_noteShift_string,
    error_padEdit_string,
    error_ccX_string,
    error_ccY_string,
    error_xMin_string,
    error_xMax_string,
    error_yMin_string,
    error_yMax_string,
    error_xCurve_string,
    error_yCurve_string,
    error_menu_string
};

const uint8_t functionErrorArray_sizes[] PROGMEM =
{
    ARRAY_SIZE_CHAR(error_changeProgram_string),
    ARRAY_SIZE_CHAR(error_changeScale_string),
    ARRAY_SIZE_CHAR(error_changeMIDIchannel_string),
    ARRAY_SIZE_CHAR(error_play_string),
    ARRAY_SIZE_CHAR(error_stop_string),
    ARRAY_SIZE_CHAR(error_record_string),
    ARRAY_SIZE_CHAR(error_noteChange_string),
    ARRAY_SIZE_CHAR(error_splitMode_string),
    ARRAY_SIZE_CHAR(error_sendX_string),
    ARRAY_SIZE_CHAR(error_sendY_string),
    ARRAY_SIZE_CHAR(error_sendAftertouch_string),
    ARRAY_SIZE_CHAR(error_sendNotes_string),
    ARRAY_SIZE_CHAR(error_octaveChange_string),
    ARRAY_SIZE_CHAR(error_noteShift_string),
    ARRAY_SIZE_CHAR(error_padEdit_string),
    ARRAY_SIZE_CHAR(error_ccX_string),
    ARRAY_SIZE_CHAR(error_ccY_string),
    ARRAY_SIZE_CHAR(error_xMin_string),
    ARRAY_SIZE_CHAR(error_xMax_string),
    ARRAY_SIZE_CHAR(error_yMin_string),
    ARRAY_SIZE_CHAR(error_yMax_string),
    ARRAY_SIZE_CHAR(error_xCurve_string),
    ARRAY_SIZE_CHAR(error_yCurve_string),
    ARRAY_SIZE_CHAR(error_menu_string)
};

/// @}