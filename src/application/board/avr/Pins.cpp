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
/// \brief Initializes all pins to correct states.
///
void Board::initPins()
{
    setOutput(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setOutput(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setOutput(LED_ROW_3_PORT, LED_ROW_3_PIN);

    #ifdef LED_INVERT
    setHigh(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setHigh(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setHigh(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #else
    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #endif

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);

    setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);

    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setOutput(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    setOutput(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    setOutput(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    setOutput(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    setHigh(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setInput(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN);
    setOutput(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);
    setOutput(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);

    setOutput(DECODER_IN_A0_PORT, DECODER_IN_A0_PIN);
    setOutput(DECODER_IN_A1_PORT, DECODER_IN_A1_PIN);
    setOutput(DECODER_IN_A2_PORT, DECODER_IN_A2_PIN);

    setInput(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN);
    setHigh(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN);

    //init unused pins - output, low
    setOutput(PORTA, 0);
    setOutput(PORTA, 1);
    setOutput(PORTA, 2);
    setOutput(PORTA, 6);
    setOutput(PORTA, 7);

    setLow(PORTA, 0);
    setLow(PORTA, 1);
    setLow(PORTA, 2);
    setLow(PORTA, 6);
    setLow(PORTA, 7);

    setOutput(PORTB, 0);
    setOutput(PORTB, 3);
    setOutput(PORTB, 7);

    setLow(PORTB, 0);
    setLow(PORTB, 3);
    setLow(PORTB, 7);

    setOutput(PORTC, 6);
    setOutput(PORTC, 7);

    setLow(PORTC, 6);
    setLow(PORTC, 7);

    setOutput(PORTD, 0);
    setOutput(PORTD, 1);
    setOutput(PORTD, 2);
    setOutput(PORTD, 4);

    setLow(PORTD, 0);
    setLow(PORTD, 1);
    setLow(PORTD, 2);
    setLow(PORTD, 4);

    setOutput(PORTE, 0);
    setOutput(PORTE, 1);
    setOutput(PORTE, 3);
    setOutput(PORTE, 4);
    setOutput(PORTE, 5);
    setOutput(PORTE, 6);
    setOutput(PORTE, 7);

    setLow(PORTE, 0);
    setLow(PORTE, 1);
    setLow(PORTE, 3);
    setLow(PORTE, 4);
    setLow(PORTE, 5);
    setLow(PORTE, 6);
    setLow(PORTE, 7);
}