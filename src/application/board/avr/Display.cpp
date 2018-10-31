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

#include "pins/Pins.h"
#include "../common/display/u8x8_wrapper.h"
#include "core/src/HAL/avr/spi/SPI.h"
#include "core/src/HAL/avr/PinManipulation.h"
#include "core/src/general/Timing.h"

///
/// \ingroup board
/// @{

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
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
}

uint8_t u8x8_byte_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;
    uint8_t internal_spi_mode;

    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;

        while( arg_int > 0 )
        {
            spiTransfer((uint8_t)*data);
            data++;
            arg_int--;
        }
        break;

        case U8X8_MSG_BYTE_INIT:
        /* disable chipselect */
        setHigh(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
        spiInit();
        break;

        case U8X8_MSG_BYTE_SET_DC:
        //u8x8_gpio_SetDC(u8x8, arg_int);
        arg_int ? setHigh(DISPLAY_DC_PORT, DISPLAY_DC_PIN) : setLow(DISPLAY_DC_PORT, DISPLAY_DC_PIN);
        break;

        case U8X8_MSG_BYTE_START_TRANSFER:
        internal_spi_mode = 0;

        switch(u8x8->display_info->spi_mode)
        {
            case 0:
            internal_spi_mode = SPI_MODE0;
            break;

            case 1:
            internal_spi_mode = SPI_MODE1;
            break;

            case 2:
            internal_spi_mode = SPI_MODE2;
            break;
            
            case 3:
            internal_spi_mode = SPI_MODE3;
            break;
        }

        spiInit();

        if (u8x8->display_info->sck_pulse_width_ns < 70)
            setClockDivider(SPI_CLOCK_DIV2);
        else if (u8x8->display_info->sck_pulse_width_ns < 140)
            setClockDivider(SPI_CLOCK_DIV4);
        else
            setClockDivider(SPI_CLOCK_DIV8);

        setDataMode(internal_spi_mode);
        setBitOrder(MSBFIRST);

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
}

/// @}