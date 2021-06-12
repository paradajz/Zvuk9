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

/// Various sanity checks defined as macros for easier input argument validation.

#define PAD_CHECK(pad)            (((pad < 0) || (pad >= NUMBER_OF_PADS)) ? 0 : 1)
#define NOTE_ASSIGN_CHECK(note)   (((note < 0) || (note >= NOTES_PER_PAD)) ? 0 : 1)
#define MIDI_VALUE_CHECK(note)    (((note < 0) || (note > 127)) ? 0 : 1)
#define SCALE_CHECK(scale)        ((scale < 0) || (scale >= (PREDEFINED_SCALES + NUMBER_OF_USER_SCALES) ? 0 : 1))
#define PROGRAM_CHECK(program)    (((program < 0) || (program >= NUMBER_OF_PROGRAMS)) ? 0 : 1)
#define OCTAVE_CHECK(octave)      (((octave < 0) || (octave >= MIDI_NOTES)) ? 0 : 1)
#define XY_RAW_VALUE_CHECK(value) (((value < 85) || (value > 950)) ? 0 : 1)