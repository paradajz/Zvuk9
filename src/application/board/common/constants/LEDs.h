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
/// \ingroup boardCommon
/// @{

///
/// \brief Total number of states between fully off and fully on for LEDs.
///
#define NUMBER_OF_LED_TRANSITIONS           120

///
/// \brief Array holding values needed to achieve more natural LED transition between states.
///
const uint8_t ledTransitionScale[NUMBER_OF_LED_TRANSITIONS] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    5,
    5,
    5,
    6,
    6,
    6,
    7,
    7,
    7,
    8,
    8,
    9,
    9,
    10,
    10,
    11,
    11,
    12,
    13,
    13,
    14,
    15,
    16,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    25,
    26,
    27,
    29,
    30,
    31,
    33,
    35,
    36,
    38,
    40,
    42,
    44,
    46,
    49,
    51,
    54,
    56,
    59,
    62,
    65,
    68,
    71,
    75,
    78,
    82,
    86,
    90,
    95,
    99,
    104,
    109,
    114,
    120,
    126,
    132,
    138,
    145,
    152,
    159,
    167,
    175,
    183,
    192,
    201,
    211,
    221,
    232,
    243,
    255
};

/// @}