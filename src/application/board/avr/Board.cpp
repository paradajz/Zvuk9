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

#include <avr/wdt.h>
#include <avr/eeprom.h>
#include "../Board.h"
#include "constants/CRC.h"
#include "core/src/HAL/avr/reset/Reset.h"

///
/// \ingroup boardAVR
/// @{

///
/// \brief Placeholder variable used only to reserve space in linker section.
///
const uint32_t appLength __attribute__((section(".applen"))) __attribute__((used)) = 0;

///
/// \brief Location at which size of application is written in flash.
///
#define APP_LENGTH_LOCATION (uint32_t)0x00000098

/// @}

void Board::reboot()
{
    mcuReset();
}