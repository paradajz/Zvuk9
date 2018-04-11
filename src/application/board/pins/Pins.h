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

//mux
//common
//original
//#define MUX_COMMON_PIN_0_PORT       PORTF
//#define MUX_COMMON_PIN_0_PIN        7
//
//#define MUX_COMMON_PIN_1_PORT       PORTF
//#define MUX_COMMON_PIN_1_PIN        5
//
//#define MUX_COMMON_PIN_2_PORT       PORTF
//#define MUX_COMMON_PIN_2_PIN        6
//
//#define MUX_COMMON_PIN_3_PORT       PORTF
//#define MUX_COMMON_PIN_3_PIN        4

//problem moves to upper left
#define MUX_COMMON_PIN_0_PORT       PORTF
#define MUX_COMMON_PIN_0_PIN        4

#define MUX_COMMON_PIN_1_PORT       PORTF
#define MUX_COMMON_PIN_1_PIN        6

#define MUX_COMMON_PIN_2_PORT       PORTF
#define MUX_COMMON_PIN_2_PIN        5

#define MUX_COMMON_PIN_3_PORT       PORTF
#define MUX_COMMON_PIN_3_PIN        7

//select
#define MUX_SELECT_PIN_0_PORT       PORTF
#define MUX_SELECT_PIN_0_PIN        1

#define MUX_SELECT_PIN_1_PORT       PORTF
#define MUX_SELECT_PIN_1_PIN        0

#define MUX_SELECT_PIN_2_PORT       PORTF
#define MUX_SELECT_PIN_2_PIN        2

#define MUX_SELECT_PIN_3_PORT       PORTF
#define MUX_SELECT_PIN_3_PIN        3

//LEDs

#define LED_ROW_1_PORT              PORTB
#define LED_ROW_1_PIN               4

#define LED_ROW_2_PORT              PORTB
#define LED_ROW_2_PIN               5

#define LED_ROW_3_PORT              PORTB
#define LED_ROW_3_PIN               6

#define DECODER_OUT_1_PORT          PORTC
#define DECODER_OUT_1_PIN           0

#define DECODER_OUT_2_PORT          PORTC
#define DECODER_OUT_2_PIN           1

#define DECODER_OUT_3_PORT          PORTC
#define DECODER_OUT_3_PIN           2

#define DECODER_OUT_PORT            PORTC

//used to clear all select bits in encoder (valid only if all three pins are on same port)
#define DECODER_OUT_CLEAR_MASK      0b11111000

const uint8_t decoderOutOrderArray[] =
{
    0b00000000,
    0b00000001,
    0b00000010,
    0b00000011,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111
};

//encoders/buttons

#define DECODER_IN_A0_PORT          PORTC
#define DECODER_IN_A0_PIN           3

#define DECODER_IN_A1_PORT          PORTC
#define DECODER_IN_A1_PIN           4

#define DECODER_IN_A2_PORT          PORTC
#define DECODER_IN_A2_PIN           5

#define DECODER_IN_PORT             PORTC

//used to clear all select bits in encoder (valid only if all three pins are on same port)
#define DECODER_IN_CLEAR_MASK       0b11000111

const uint8_t decoderInOrderArray[] =
{
    0b00000000,
    0b00001000,
    0b00010000,
    0b00011000,
    0b00100000,
    0b00101000,
    0b00110000,
    0b00111000
};

#define INPUT_SHIFT_REG_IN_PORT     PORTD
#define INPUT_SHIFT_REG_IN_PIN      5

#define INPUT_SHIFT_REG_CLOCK_PORT  PORTD
#define INPUT_SHIFT_REG_CLOCK_PIN   6

#define INPUT_SHIFT_REG_LATCH_PORT  PORTD
#define INPUT_SHIFT_REG_LATCH_PIN   7

//display
#define DISPLAY_DC_PORT                 PORTA
#define DISPLAY_DC_PIN                  3

#define DISPLAY_CS_PORT                 PORTA
#define DISPLAY_CS_PIN                  4

#define DISPLAY_RESET_PORT              PORTA
#define DISPLAY_RESET_PIN               5

//SPI
#define SPI_CLOCK_PORT              PORTB
#define SPI_CLOCK_PIN               1

#define SPI_MOSI_PORT               PORTB
#define SPI_MOSI_PIN                2

#define SPI_SS_PORT                 PORTB
#define SPI_SS_PIN                  0

//bootloader button
#define BTLDR_BUTTON_PORT           PORTE
#define BTLDR_BUTTON_PIN            2
