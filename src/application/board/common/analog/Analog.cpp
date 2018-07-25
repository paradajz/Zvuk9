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

#include "board/Board.h"
#include "Variables.h"
#include "interface/analog/pads/DataTypes.h"
#include "constants/Pads.h"

///
/// \ingroup board
/// @{

volatile uint16_t   padPressed;
uint16_t            pressurePlate1;
padData_t           analogInBuffer[ANALOG_IN_BUFFER_SIZE];
padData_t           analogInBufferReadOnly;
uint8_t             activePad;
uint8_t             padReadingIndex;
volatile uint8_t    aIn_head;
volatile uint8_t    aIn_tail;
volatile uint8_t    aIn_count;

/// @}


bool Board::padDataAvailable()
{
    if (aIn_count)
    {
        #ifdef __AVR__
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        #endif
        {
            if (++aIn_tail == ANALOG_IN_BUFFER_SIZE)
                aIn_tail = 0;

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                analogInBufferReadOnly.zReading[i] = analogInBuffer[aIn_tail].zReading[i];
                analogInBufferReadOnly.xReading[i] = analogInBuffer[aIn_tail].xReading[i];
                analogInBufferReadOnly.yReading[i] = analogInBuffer[aIn_tail].yReading[i];
            }

            aIn_count--;
        }

        return true;
    }

    return false;
}

int16_t Board::getPadX(uint8_t pad)
{
    return 1023 - analogInBufferReadOnly.xReading[pad];
}

int16_t Board::getPadY(uint8_t pad)
{
    return analogInBufferReadOnly.yReading[pad];
}

int16_t Board::getPadPressure(uint8_t pad)
{
    static uint8_t releaseDebounceCount[NUMBER_OF_PADS] = { 0 };

    if (analogInBufferReadOnly.zReading[pad] >= PRESSURE_VALUES)
        analogInBufferReadOnly.zReading[pad] = PRESSURE_VALUES-1;

    uint16_t cVal = analogInBufferReadOnly.zReading[pad];

    //if pad is already pressed, return zero value only if it's smaller
    //or equal to PAD_RELEASE_PRESSURE
    if (cVal <= PAD_PRESS_PRESSURE)
    {
        if (BIT_READ(padPressed, pad))
        {
            if (cVal < PAD_RELEASE_PRESSURE)
                cVal = 0;
        }
        else
        {
            cVal = 0;
        }
    }
    else
    {
        //normalize - treat PAD_PRESS_PRESSURE value as 0
        cVal -= PAD_PRESS_PRESSURE;
    }

    if (!cVal)
    {
        if (releaseDebounceCount[pad] >= PAD_RELEASED_DEBOUNCE_COUNT)
        {
            //really released
        }
        else
        {
            //not yet released
            cVal = 1;
            releaseDebounceCount[pad]++;
        }
    }
    else
    {
        releaseDebounceCount[pad] = 0;
    }

    #ifdef DEBUG
    if (!pad)
        printf("pad %d pressure: %d raw: %d\nx: %d\ny: %d\n\n", pad, cVal, analogInBufferReadOnly.zReading[pad], getPadX(pad), getPadY(pad));
    #endif

    return cVal;
}