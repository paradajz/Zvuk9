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

#include "core/src/general/IO.h"

#define BTLDR_BUTTON_PORT CORE_IO_PORT(C)
#define BTLDR_BUTTON_PIN  CORE_IO_PORT_INDEX(4)
#define SR_IN_DATA_PORT   CORE_IO_PORT(C)
#define SR_IN_DATA_PIN    CORE_IO_PORT_INDEX(14)
#define SR_IN_CLK_PORT    CORE_IO_PORT(C)
#define SR_IN_CLK_PIN     CORE_IO_PORT_INDEX(15)
#define SR_IN_LATCH_PORT  CORE_IO_PORT(C)
#define SR_IN_LATCH_PIN   CORE_IO_PORT_INDEX(13)
#define DEC_BM_PORT_A0    CORE_IO_PORT(C)
#define DEC_BM_PIN_A0     CORE_IO_PORT_INDEX(8)
#define DEC_BM_PORT_A1    CORE_IO_PORT(C)
#define DEC_BM_PIN_A1     CORE_IO_PORT_INDEX(7)
#define DEC_BM_PORT_A2    CORE_IO_PORT(C)
#define DEC_BM_PIN_A2     CORE_IO_PORT_INDEX(6)
#define DEC_LM_PORT_A0    CORE_IO_PORT(B)
#define DEC_LM_PIN_A0     CORE_IO_PORT_INDEX(9)
#define DEC_LM_PORT_A1    CORE_IO_PORT(B)
#define DEC_LM_PIN_A1     CORE_IO_PORT_INDEX(7)
#define DEC_LM_PORT_A2    CORE_IO_PORT(B)
#define DEC_LM_PIN_A2     CORE_IO_PORT_INDEX(6)
#define LED_ROW_PORT_0    CORE_IO_PORT(B)
#define LED_ROW_PIN_0     CORE_IO_PORT_INDEX(5)
#define LED_ROW_PORT_1    CORE_IO_PORT(B)
#define LED_ROW_PIN_1     CORE_IO_PORT_INDEX(4)
#define LED_ROW_PORT_2    CORE_IO_PORT(B)
#define LED_ROW_PIN_2     CORE_IO_PORT_INDEX(3)
#define MUX_PORT_INPUT_0  CORE_IO_PORT(A)
#define MUX_PIN_INPUT_0   CORE_IO_PORT_INDEX(3)
#define MUX_PORT_INPUT_1  CORE_IO_PORT(A)
#define MUX_PIN_INPUT_1   CORE_IO_PORT_INDEX(2)
#define MUX_PORT_INPUT_2  CORE_IO_PORT(A)
#define MUX_PIN_INPUT_2   CORE_IO_PORT_INDEX(1)
#define MUX_PORT_INPUT_3  CORE_IO_PORT(A)
#define MUX_PIN_INPUT_3   CORE_IO_PORT_INDEX(0)
#define MUX_PORT_S0       CORE_IO_PORT(B)
#define MUX_PIN_S0        CORE_IO_PORT_INDEX(12)
#define MUX_PORT_S1       CORE_IO_PORT(B)
#define MUX_PIN_S1        CORE_IO_PORT_INDEX(13)
#define MUX_PORT_S2       CORE_IO_PORT(B)
#define MUX_PIN_S2        CORE_IO_PORT_INDEX(14)
#define MUX_PORT_S3       CORE_IO_PORT(B)
#define MUX_PIN_S3        CORE_IO_PORT_INDEX(15)