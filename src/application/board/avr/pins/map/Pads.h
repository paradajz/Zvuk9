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

#include "../Pins.h"
#include "board/common/DataTypes.h"

///
/// \brief Pad indexes.
/// \ingroup board
/// @{

#define PAD_0       0
#define PAD_1       1
#define PAD_2       2
#define PAD_3       3
#define PAD_4       4
#define PAD_5       5
#define PAD_6       6
#define PAD_7       7
#define PAD_8       8

///
/// \brief Array holding pad indexes for easier access.
///
const uint8_t padIDArray[NUMBER_OF_PADS] =
{
    PAD_0,
    PAD_1,
    PAD_2,
    PAD_3,
    PAD_4,
    PAD_5,
    PAD_6,
    PAD_7,
    PAD_8
};

///
/// \brief Array holding pins which should be read for specific coordinate.
/// Matched with padReadOrder_t enumeration.
///
const uint8_t coordinateAnalogInput[PAD_READINGS] =
{
    PAD_PLATE_X_PLUS_PIN,
    PAD_PLATE_Y_PLUS_PIN,
    PAD_PLATE_Y_MINUS_PIN,
    PAD_PLATE_X_MINUS_PIN,
    PAD_PLATE_Y_PLUS_PIN,
    PAD_PLATE_X_PLUS_PIN
};

/// @}