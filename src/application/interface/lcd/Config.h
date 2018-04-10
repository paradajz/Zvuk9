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

#include "../analog/pads/Config.h"

///
/// \brief Length of temporary (message) text on display in milliseconds.
///
#define LCD_MESSAGE_DURATION                1500

///
/// \brief Time in milliseconds after text on display is being refreshed.
///
#define LCD_REFRESH_TIME                    PAD_NOTE_SEND_DELAY

///
/// \brief Time in milliseconds after which scrolling text moves on display.
///
#define LCD_SCROLL_TIME                     1000

///
/// \brief Maximum amount of characters displayed in single LCD row.
///
#define LCD_WIDTH                           32

///
/// \brief Number of LCD rows.
///
#define LCD_HEIGHT                          4

///
/// \brief Value by which actual octave is being subtracted when showing octave on display.
///
#define OCTAVE_NORMALIZATION                -2

///
/// \brief Array holding remapped values of LCD rows.
/// LCD actually contains 8 rows, however, to increase readability every second row is used.
///
const uint8_t rowMap[LCD_HEIGHT] =
{
    0,
    2,
    4,
    6
};
