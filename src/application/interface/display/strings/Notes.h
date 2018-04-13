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
/// \brief Display strings used to indicate notes and their parameters.
/// \ingroup displayStrings
/// @{

const char tonicC_string[] PROGMEM = "C";
const char tonicCSharp_string[] PROGMEM = "C#";
const char tonicD_string[] PROGMEM = "D";
const char tonicDSharp_string[] PROGMEM = "D#";
const char tonicE_string[] PROGMEM = "E";
const char tonicF_string[] PROGMEM = "F";
const char tonicFSharp_string[] PROGMEM = "F#";
const char tonicG_string[] PROGMEM = "G";
const char tonicGSharp_string[] PROGMEM = "G#";
const char tonicA_string[] PROGMEM = "A";
const char tonicASharp_string[] PROGMEM = "A#";
const char tonicB_strig[] PROGMEM = "B";

const char noNotes_string[] PROGMEM = "<no assigned notes>";

const char padEditOctave_string[] PROGMEM = "Octave ";
const char assignedNotes_string[] PROGMEM = "Assigned notes:";

const char velocity_string[] PROGMEM = "v: ";
const char aftertouch_string[] PROGMEM = "at: ";

const char noteShift_string[] PROGMEM = "n";

PGM_P const noteNameArray[] PROGMEM =
{
    tonicC_string,
    tonicCSharp_string,
    tonicD_string,
    tonicDSharp_string,
    tonicE_string,
    tonicF_string,
    tonicFSharp_string,
    tonicG_string,
    tonicGSharp_string,
    tonicA_string,
    tonicASharp_string,
    tonicB_strig
};

/// @}