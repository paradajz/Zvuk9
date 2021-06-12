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

/// \ingroup interfaceDisplay
/// @{

/// List of all possible menu types.
typedef enum
{
    noMenu,
    userMenu,
    serviceMenu,
} menuType_t;

/// List of all possible text types on display.
typedef enum
{
    displayText_still,
    displayText_temp
} displayTextType_t;

/// List of possible setting types which are being displayed when changing various parameters.
typedef enum
{
    globalSetting,
    singlePadSetting,
    allPadsSetting
} settingType_t;

/// List of all possible text scrolling directions.
typedef enum
{
    scroll_ltr,
    scroll_rtl
} scrollDirection_t;

/// Structure holding data for scrolling event on display for single row.
typedef struct
{
    uint8_t           size;
    uint8_t           startIndex;
    int8_t            currentIndex;
    scrollDirection_t direction;
} scrollEvent_t;

/// @}
