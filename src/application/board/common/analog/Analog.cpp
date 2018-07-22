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
volatile int16_t    samples[3][NUMBER_OF_PADS];
uint8_t             activePad;
uint8_t             padReadingIndex;

/// @}


bool Board::padDataAvailable()
{
    return (activePad == NUMBER_OF_PADS);
}

void Board::continuePadReadout()
{
    activePad = 0;
    startADCconversion();
}

int16_t Board::getPadX(uint8_t pad)
{
    if (samples[coordinateX][pad] != -1)
        return 1023 - samples[coordinateX][pad];
    else
        return -1;
}

int16_t Board::getPadY(uint8_t pad)
{
    if (samples[coordinateY][pad] != -1)
        return samples[coordinateY][pad];
    else
        return -1;
}

int16_t Board::getPadPressure(uint8_t pad)
{
    static uint8_t releaseDebounceCount[NUMBER_OF_PADS] = { 0 };

    if (samples[coordinateZ][pad] != -1)
    {
        if (samples[coordinateZ][pad] >= PRESSURE_VALUES)
            samples[coordinateZ][pad] = PRESSURE_VALUES-1;

        uint16_t cVal = samples[coordinateZ][pad];//pgm_read_word(&pressure_correction[samples[coordinateZ][pad]]);

        //if pad is already pressed, return zero value only if it's smaller
        //than PAD_RELEASE_PRESSURE
        if (cVal < PAD_PRESS_PRESSURE)
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
            cVal -= PAD_PRESS_PRESSURE;
        }

        if (!cVal)
        {
            releaseDebounceCount[pad]++;

            if (releaseDebounceCount[pad] == PAD_RELEASED_DEBOUNCE_COUNT)
            {
                //really released
                releaseDebounceCount[pad] = 0;
            }
            else
            {
                //not yet released
                cVal = 1;
            }
        }
        else
        {
            releaseDebounceCount[pad] = 0;
        }

        #ifdef DEBUG
        if (!pad)
        printf("pad %d pressure: %d raw: %d\nx: %d\ny: %d\n\n", pad, cVal, samples[coordinateZ][pad], getPadX(pad), getPadY(pad));
        #endif

        return cVal;
    }
    else
    {
        return -1;
    }
}