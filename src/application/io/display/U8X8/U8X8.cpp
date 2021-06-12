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

#include "U8X8.h"
#include "core/src/HAL/avr/spi/SPI.h"
#include "core/src/HAL/avr/PinManipulation.h"
#include "core/src/general/Timing.h"
#include "board/avr/pins/Pins.h"

namespace
{
    u8x8_t u8x8;
}

namespace U8X8
{
    bool initDisplay()
    {
        auto gpioDelay = [](u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr) -> uint8_t
        {
            switch(msg)
            {
                case U8X8_MSG_GPIO_AND_DELAY_INIT:
                //init display pins
                setOutput(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
                setOutput(DISPLAY_DC_PORT, DISPLAY_DC_PIN);
                break;

                case U8X8_MSG_DELAY_10MICRO:
                /* not used at the moment */
                break;

                case U8X8_MSG_DELAY_100NANO:
                /* not used at the moment */
                break;

                case U8X8_MSG_DELAY_MILLI:
                wait_ms(arg_int);
                break;

                case U8X8_MSG_GPIO_CS:
                arg_int ? setHigh(DISPLAY_CS_PORT, DISPLAY_CS_PIN) : setLow(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
                break;

                case U8X8_MSG_GPIO_DC:
                arg_int ? setHigh(DISPLAY_DC_PORT, DISPLAY_DC_PIN) : setLow(DISPLAY_DC_PORT, DISPLAY_DC_PIN);
                break;

                default:
                return 0;
                break;
            }

            return 1;
        };

        auto spiHWA = [](u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) -> uint8_t
        {
            uint8_t *data;
            SPI::mode_t internal_spi_mode;

            switch(msg)
            {
                case U8X8_MSG_BYTE_SEND:
                data = (uint8_t *)arg_ptr;

                while( arg_int > 0 )
                {
                    SPI::spiTransfer((uint8_t)*data);
                    data++;
                    arg_int--;
                }
                break;

                case U8X8_MSG_BYTE_INIT:
                /* disable chipselect */
                setHigh(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
                SPI::init();
                break;

                case U8X8_MSG_BYTE_SET_DC:
                //u8x8_gpio_SetDC(u8x8, arg_int);
                arg_int ? setHigh(DISPLAY_DC_PORT, DISPLAY_DC_PIN) : setLow(DISPLAY_DC_PORT, DISPLAY_DC_PIN);
                break;

                case U8X8_MSG_BYTE_START_TRANSFER:
                internal_spi_mode = SPI::mode_t::mode0;

                switch(u8x8->display_info->spi_mode)
                {
                    case 0:
                    internal_spi_mode = SPI::mode_t::mode0;
                    break;

                    case 1:
                    internal_spi_mode = SPI::mode_t::mode1;
                    break;

                    case 2:
                    internal_spi_mode = SPI::mode_t::mode2;
                    break;

                    case 3:
                    internal_spi_mode = SPI::mode_t::mode3;
                    break;
                }

                SPI::init();

                if (u8x8->display_info->sck_pulse_width_ns < 70)
                    SPI::setClockDivider(SPI::clockDiv_t::div2);
                else if (u8x8->display_info->sck_pulse_width_ns < 140)
                    SPI::setClockDivider(SPI::clockDiv_t::div4);
                else
                    SPI::setClockDivider(SPI::clockDiv_t::div8);

                SPI::setDataMode(internal_spi_mode);
                SPI::setBitOrder(SPI::bitOrder_t::msb);

                //enable chip select
                setLow(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
                u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
                break;

                case U8X8_MSG_BYTE_END_TRANSFER:
                u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
                //disable chip select
                setHigh(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
                break;

                default:
                return 0;
            }

            return 1;
        };

        //setup defaults
        u8x8_SetupDefaults(&u8x8);

        u8x8.display_cb = u8x8_d_ssd1322_nhd_256x64;
        u8x8.cad_cb = u8x8_cad_011;
        u8x8.byte_cb = spiHWA;
        u8x8.gpio_and_delay_cb = gpioDelay;

        /* setup display info */
        u8x8_SetupMemory(&u8x8);
        u8x8_InitDisplay(&u8x8);

        clearDisplay();
        u8x8_SetPowerSave(&u8x8, false);

        return true;
    }

    void clearDisplay()
    {
        u8x8_ClearDisplay(&u8x8);
    }

    void setPowerSave(uint8_t is_enable)
    {
        u8x8_SetPowerSave(&u8x8, is_enable);
    }

    void setFlipMode(uint8_t mode)
    {
        u8x8_SetFlipMode(&u8x8, mode);
    }

    void setFont(const uint8_t *font_8x8)
    {
        u8x8_SetFont(&u8x8, font_8x8);
    }

    void drawGlyph(uint8_t x, uint8_t y, uint8_t encoding)
    {
        u8x8_DrawGlyph(&u8x8, x, y, encoding);
    }
}