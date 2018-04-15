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

#include "u8g2/csrc/u8x8.h"

///
/// \brief C++ U8x8 library wrapper.
/// Based on original wrapper for Arduino boards.
/// \defgroup boardDisplay Display
/// \ingroup board
/// @{
///

class U8X8
{
    public:
    U8X8();
    void initDisplay();
    void clearDisplay();
    void setPowerSave(bool state);
    void setFlipMode(bool state);
    void setFont(const uint8_t *font_8x8);
    void drawGlyph(uint8_t x, uint8_t y, char character);

    private:
    ///
    /// \brief Main U8x8 data structure.
    ///
    u8x8_t u8x8;
};

///
/// \brief External definitions for low-level display control.
/// These functions must be implemented in order to access display.
/// @{

extern uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
extern uint8_t u8x8_byte_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/// @}

///
/// \brief External definition of U8X8 class instance.
///
extern U8X8 display_hw;

/// @}
