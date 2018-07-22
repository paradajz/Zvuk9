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
#include "board/common/digital/input/Variables.h"
#include "board/common/constants/Encoders.h"
#include "pins/map/Encoders.h"

///
/// \ingroup board
/// @{

///
/// \brief Array holding last two readings from encoder pins.
///
uint8_t encoderData[MAX_NUMBER_OF_ENCODERS];

///
/// \brief Array holding current amount of pulses for all encoders.
///
int8_t  encoderPulses[MAX_NUMBER_OF_ENCODERS];

/// @}


int8_t Board::readEncoder(uint8_t encoderID, uint8_t pairState, uint8_t pulsesPerStep)
{
    int8_t returnValue = 0;

    //add new data
    encoderData[encoderID] <<= 2;
    encoderData[encoderID] |= pairState;
    encoderData[encoderID] &= 0x0F;

    encoderPulses[encoderID] += encoderLookUpTable[encoderData[encoderID]];

    if (abs(encoderPulses[encoderID]) >= pulsesPerStep)
    {
        returnValue = (encoderPulses[encoderID] > 0) ? 1 : -1;
        //reset count
        encoderPulses[encoderID] = 0;
    }

    return returnValue;
}

int8_t Board::getEncoderState(uint8_t encoderID, uint8_t pulsesPerStep)
{
    uint8_t column = encoderID % NUMBER_OF_BUTTON_COLUMNS;
    uint8_t row  = (encoderID/NUMBER_OF_BUTTON_COLUMNS)*2;
    uint8_t pairState = (digitalInBufferReadOnly[column] >> row) & 0x03;

    return readEncoder(encoderID, pairState, pulsesPerStep);
}

bool Board::encoderEnabled(uint8_t encoderNumber)
{
    return encoderEnabledMap[encoderNumber];
}