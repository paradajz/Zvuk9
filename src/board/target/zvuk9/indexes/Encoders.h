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

#define PROGRAM_ENCODER 0
#define X_CURVE_ENCODER 1
#define Y_CURVE_ENCODER 2
#define PRESET_ENCODER  8
#define X_MAX_ENCODER   9
#define Y_MAX_ENCODER   10
#define X_CC_ENCODER    16
#define Y_CC_ENCODER    17
#define X_MIN_ENCODER   24
#define Y_MIN_ENCODER   25

// /// Array holding information whether or not specific encoder is enabled.
// const bool encoderEnabledMap[MAX_NUMBER_OF_ENCODERS] = {
//     1,    //PROGRAM_ENCODER
//     1,    //X_CURVE_ENCODER
//     1,    //Y_CURVE_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     1,    //PRESET_ENCODER
//     1,    //X_MAX_ENCODER
//     1,    //Y_MAX_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     1,    //X_CC_ENCODER
//     1,    //Y_CC_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     0,
//     1,    //X_MIN_ENCODER
//     1,    //Y_MIN_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     0,
// };

// /// Array holding information on how much pulses specific encoder generates per step.
// const uint8_t encoderPulsesPerStepMap[MAX_NUMBER_OF_ENCODERS] = {
//     2,    //PROGRAM_ENCODER
//     4,    //X_CURVE_ENCODER
//     4,    //Y_CURVE_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     2,    //PRESET_ENCODER
//     4,    //X_MAX_ENCODER
//     4,    //Y_MAX_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     4,    //X_CC_ENCODER
//     4,    //Y_CC_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     0,
//     4,    //X_MIN_ENCODER
//     4,    //Y_MIN_ENCODER
//     0,
//     0,
//     0,
//     0,
//     0,
//     0,
// };