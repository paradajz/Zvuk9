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

const char presetNaturalMinor_string[] PROGMEM = "Natural Minor";
const char presetMelodicMinor_string[] PROGMEM = "Melodic Minor";
const char presetHarmonicMinor_string[] PROGMEM = "Harmonic Minor";
const char presetMajor_string[] PROGMEM = "Major";
const char presetHarmonicMajor_string[] PROGMEM = "Harmonic Major";
const char presetMinorPentatonic_string[] PROGMEM = "Minor Pentatonic";
const char presetMajorPentatonic_string[] PROGMEM = "Major Pentatonic";
const char presetUser_string[] PROGMEM = "User scale";

PGM_P const presetNameArray[] PROGMEM =
{
    presetNaturalMinor_string,
    presetMelodicMinor_string,
    presetHarmonicMinor_string,
    presetMajor_string,
    presetHarmonicMajor_string,
    presetMinorPentatonic_string,
    presetMajorPentatonic_string,
    presetUser_string
};

const uint8_t presetNameArray_sizes[] PROGMEM =
{
    ARRAY_SIZE_CHAR(presetNaturalMinor_string),
    ARRAY_SIZE_CHAR(presetMelodicMinor_string),
    ARRAY_SIZE_CHAR(presetHarmonicMinor_string),
    ARRAY_SIZE_CHAR(presetMajor_string),
    ARRAY_SIZE_CHAR(presetHarmonicMajor_string) ,
    ARRAY_SIZE_CHAR(presetMinorPentatonic_string),
    ARRAY_SIZE_CHAR(presetMajorPentatonic_string),
    ARRAY_SIZE_CHAR(presetUser_string)
};
