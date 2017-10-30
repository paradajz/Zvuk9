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

/*

  U8x8lib.cpp
  
  Arduino specific low level functions


  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include "U8x8lib.h"

#ifndef __AVR_ARCH__
#define __AVR_ARCH__ 0
#endif

size_t U8X8::write(uint8_t v)
{
    if ( v == '\n' )
    {
        ty++;
        tx=0;
    }
    else
    {
        u8x8_DrawGlyph(&u8x8, tx, ty, v);
        tx++;
    }

    return 1;
}

extern "C" uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
        //init lcd pins
        setOutput(LCD_CS_PORT, LCD_CS_PIN);
        setOutput(LCD_DC_PORT, LCD_DC_PIN);
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
        arg_int ? setHigh(LCD_CS_PORT, LCD_CS_PIN) : setLow(LCD_CS_PORT, LCD_CS_PIN);
        break;

        case U8X8_MSG_GPIO_DC:
        arg_int ? setHigh(LCD_DC_PORT, LCD_DC_PIN) : setLow(LCD_DC_PORT, LCD_DC_PIN);
        break;

        default:
        return 0;
        break;
    }

    return 1;
}

extern "C" uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
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
        setHigh(LCD_CS_PORT, LCD_CS_PIN);
        spiInit();
        break;

        case U8X8_MSG_BYTE_SET_DC:
        //u8x8_gpio_SetDC(u8x8, arg_int);
        arg_int ? setHigh(LCD_DC_PORT, LCD_DC_PIN) : setLow(LCD_DC_PORT, LCD_DC_PIN);
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
        setLow(LCD_CS_PORT, LCD_CS_PIN);
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
        break;

        case U8X8_MSG_BYTE_END_TRANSFER:
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
        //disable chip select
        setHigh(LCD_CS_PORT, LCD_CS_PIN);
        break;

        default:
        return 0;
    }

    return 1;
}
