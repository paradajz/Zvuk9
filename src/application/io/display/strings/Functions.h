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

/// Display strings used for various interface functions.
/// \ingroup displayStrings
/// @{

const char function_midiChannel[] PROGMEM = "MIDI channel: ";
const char function_play[] PROGMEM        = "Play";
const char function_stop[] PROGMEM        = "Stop";
const char function_record[] PROGMEM      = "Record ";
const char function_tonic[] PROGMEM       = "Tonic: ";
const char function_split[] PROGMEM       = "Split ";
const char function_x[] PROGMEM           = "X ";
const char function_y[] PROGMEM           = "Y ";
const char function_aftertouch[] PROGMEM  = "Aftertouch ";
const char function_notes[] PROGMEM       = "Notes ";
const char function_octave[] PROGMEM      = "Octave: ";
const char function_noteShift[] PROGMEM   = "Note shift level: ";
const char function_xCC[] PROGMEM         = "X CC: ";
const char function_yCC[] PROGMEM         = "Y CC: ";
const char function_xPitchBend[] PROGMEM  = "X Pitch bend";
const char function_yPitchBend[] PROGMEM  = "Y Pitch bend";
const char function_xMin[] PROGMEM        = "X min: ";
const char function_xMax[] PROGMEM        = "X max: ";
const char function_yMin[] PROGMEM        = "Y min: ";
const char function_yMax[] PROGMEM        = "Y max: ";
const char function_xCurve[] PROGMEM      = "X curve: ";
const char function_yCurve[] PROGMEM      = "Y curve: ";

PGM_P const functionArray[] PROGMEM = {
    NULL,
    NULL,
    function_midiChannel,
    function_play,
    function_stop,
    function_record,
    function_tonic,
    function_split,
    function_x,
    function_y,
    function_aftertouch,
    function_notes,
    function_octave,
    function_noteShift,
    NULL,
    function_xCC,
    function_yCC,
    function_xPitchBend,
    function_yPitchBend,
    function_xMin,
    function_xMax,
    function_yMin,
    function_yMax,
    function_xCurve,
    function_yCurve,
    NULL
};

/// @}