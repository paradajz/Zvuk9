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

#include "Board.h"

///
/// \ingroup boardAVR
/// @{

///
/// \brief Placeholder variable used only to reserve space in linker section.
///
const uint32_t appLength __attribute__ ((section (".applen"))) __attribute__((used)) = 0;

///
/// \brief Location at which size of application is written in flash.
///
#define APP_LENGTH_LOCATION         (uint32_t)0x00000098

/// @}

///
/// \brief Default constructor.
///
Board::Board()
{

}

///
/// \brief Performs software MCU reboot.
///
void Board::reboot()
{
    mcuReset();
}

///
/// \brief Checks if firmware has been updated.
/// Firmware file has written CRC in last two flash addresses. Application stores last read CRC in EEPROM. If EEPROM and flash CRC differ, firmware has been updated.
/// \returns True if firmware has been updated, false otherwise.
///
bool Board::checkNewRevision()
{
    //current app crc is written in ".applen" linker section
    //previous crc is stored into eeprom
    //if two differ, app has changed

    uint32_t flash_size = pgm_read_dword(APP_LENGTH_LOCATION);
    uint16_t crc_eeprom = eeprom_read_word((uint16_t*)SW_CRC_LOCATION_EEPROM);
    uint16_t crc_flash = pgm_read_word(flash_size);

    if (crc_eeprom != crc_flash)
    {
        eeprom_update_word((uint16_t*)SW_CRC_LOCATION_EEPROM, crc_flash);
        return true;
    }

    return false;
}

Board board;
