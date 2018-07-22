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
/// \ingroup board
/// @{

///
/// \brief Constant used to debounce button readings.
///
#define BUTTON_DEBOUNCE_COMPARE     0b11110000

///
/// \brief Total number of pads.
///
#define NUMBER_OF_PADS              9

///
/// \brief Total number of LED columns in LED matrix.
///
#define NUMBER_OF_LED_COLUMNS       8

///
/// \brief Total number of LED rows in LED matrix.
///
#define NUMBER_OF_LED_ROWS          3

///
/// \brief Total number of button columns in button matrix.
///
#define NUMBER_OF_BUTTON_COLUMNS    8

///
/// \brief Total number of button rows in button matrix.
///
#define NUMBER_OF_BUTTON_ROWS       8

///
/// \brief Maximum number of buttons.
///
#define MAX_NUMBER_OF_BUTTONS       (NUMBER_OF_BUTTON_COLUMNS*NUMBER_OF_BUTTON_ROWS)

///
/// \brief Maximum number of encoders.
/// Since encoders are also connected in button matrix and encoders need two
/// pins, total number of encoders is total number of buttons divided by two.
///
#define MAX_NUMBER_OF_ENCODERS      (MAX_NUMBER_OF_BUTTONS/2)

///
/// \brief Maximum number of LEDs.
///
#define MAX_NUMBER_OF_LEDS          (NUMBER_OF_LED_COLUMNS*NUMBER_OF_LED_ROWS)

/// @}
