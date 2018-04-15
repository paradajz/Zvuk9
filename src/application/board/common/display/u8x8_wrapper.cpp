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

#include "u8x8_wrapper.h"

///
/// \brief Default constructor.
///
U8X8::U8X8()
{

}

///
/// \brief Performs display initialization.
///
void U8X8::initDisplay()
{
    //setup defaults
    u8x8_SetupDefaults(&u8x8);

    u8x8.display_cb = u8x8_d_ssd1322_nhd_256x64;
    u8x8.cad_cb = u8x8_cad_011;
    u8x8.byte_cb = u8x8_byte_hw_spi;
    u8x8.gpio_and_delay_cb = u8x8_gpio_and_delay;

    /* setup display info */
    u8x8_SetupMemory(&u8x8);
    u8x8_InitDisplay(&u8x8);

    clearDisplay();
    u8x8_SetPowerSave(&u8x8, false);
}

///
/// \brief Clears all characters from display.
///
void U8X8::clearDisplay()
{
    u8x8_ClearDisplay(&u8x8);
}

///
/// \brief Enables or disables display power-saving.
/// @param [in] state State of power saving (true/enabled, false/disabled).
///
void U8X8::setPowerSave(bool state)
{
    u8x8_SetPowerSave(&u8x8, state);
}

///
/// \brief Used to rotate display characters by 180 degrees.
/// @param [in] state If set to true, display characters are rotated by 180 degrees.
///
void U8X8::setFlipMode(bool state)
{
    u8x8_SetFlipMode(&u8x8, state);
}

///
/// \brief Used to set display font.
/// @param [in] font_8x8    Pointer to display font.
///                         See https://github.com/olikraus/u8g2/wiki/fntlist8x8.
///
void U8X8::setFont(const uint8_t *font_8x8)
{
    u8x8_SetFont(&u8x8, font_8x8);
}

///
/// \brief Draws single character on display.
/// @param [in] x           X coordinate (column) of character.
/// @param [in] y           Y coordinate (row) of character.
/// @param [in] character   Character to display.
///
void U8X8::drawGlyph(uint8_t x, uint8_t y, char character)
{
    u8x8_DrawGlyph(&u8x8, x, y, character);
}

U8X8 display_hw;