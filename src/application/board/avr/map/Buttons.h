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
/// \brief Button indexes.
/// \ingroup boardCommonMapAvr
/// @{

#define BUTTON_NOTE_C_SHARP         20
#define BUTTON_NOTE_D_SHARP         11
#define BUTTON_NOTE_F_SHARP         12
#define BUTTON_NOTE_G_SHARP         3
#define BUTTON_NOTE_A_SHARP         4

#define BUTTON_NOTE_C               59
#define BUTTON_NOTE_D               60
#define BUTTON_NOTE_E               51
#define BUTTON_NOTE_F               52
#define BUTTON_NOTE_G               43
#define BUTTON_NOTE_A               44
#define BUTTON_NOTE_B               35

#define BUTTON_ON_OFF_SPLIT         36
#define BUTTON_ON_OFF_X             13
#define BUTTON_ON_OFF_Y             14
#define BUTTON_ON_OFF_AFTERTOUCH    21
#define BUTTON_ON_OFF_NOTES         22

#define BUTTON_OCTAVE_DOWN          29
#define BUTTON_OCTAVE_UP            30

#define BUTTON_TRANSPORT_PLAY       27
#define BUTTON_TRANSPORT_STOP       28
#define BUTTON_TRANSPORT_RECORD     19

#define BUTTON_PROGRAM_ENC          6
#define BUTTON_PRESET_ENC           5

/// @}
