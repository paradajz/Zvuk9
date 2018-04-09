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

#include "../Board.h"

void Board::initPins()
{
    //leds
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

    //pads
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

    //input shift register
    setInput(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN);
    setOutput(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);
    setOutput(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);

    //input matrix decoder
    setOutput(DECODER_IN_A0_PORT, DECODER_IN_A0_PIN);
    setOutput(DECODER_IN_A1_PORT, DECODER_IN_A1_PIN);
    setOutput(DECODER_IN_A2_PORT, DECODER_IN_A2_PIN);
}
