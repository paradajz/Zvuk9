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

#include "../../interface/analog/pads/DataTypes.h"

///
/// \defgroup dbScales Scales
/// \ingroup database
/// @{

///
/// \brief List of all sections in scale database block.
///
typedef enum
{
    scalePredefinedSection,
    scaleUserSection,
    SCALE_SECTIONS
} dbSection_userScale_t;

///
/// \brief Total number of user scales.
///
#define NUMBER_OF_USER_SCALES                       10

///
/// \brief Default octave used for all scales.
///
#define DEFAULT_OCTAVE                              4

///
/// \brief Maximum number of assigned notes pad pad (for user scales).
///
#define NOTES_PER_PAD                               7

///
/// \brief Value used as invalid (blank note).
///
#define BLANK_NOTE                                  128

///
/// \brief Default tonic assigned to predefined scales.
///
#define PREDEFINED_SCALE_TONIC                      C

///
/// \brief Default scale shift value for predefined scales.
///
#define PREDEFINED_SCALE_SHIFT                      0

///
/// \brief List of all elements in predefined scale database section.
///
typedef enum
{
    PREDEFINED_SCALE_OCTAVE_ID,
    PREDEFINED_SCALE_TONIC_ID,
    PREDEFINED_SCALE_SHIFT_ID,
    PREDEFINED_SCALE_PARAMETERS
} predefinedScaleParameters;

///
/// \brief Array of default (initial) values in predefined scale database section.
///
const uint8_t defaultPredefinedScaleParametersArray[PREDEFINED_SCALE_PARAMETERS] =
{
    DEFAULT_OCTAVE,
    PREDEFINED_SCALE_TONIC,
    PREDEFINED_SCALE_SHIFT
};

/// @}