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
/// \ingroup boardAVR
/// @{

///
/// \brief Array holding processed data from encoders.
///
uint16_t  encoderData[MAX_NUMBER_OF_ENCODERS];

/// @}

///
/// \brief Initializes encoder values to default state.
///
void Board::initEncoders()
{
    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        encoderData[i] |= ((uint16_t)0 << 8);
        //set number of pulses to 8
        encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);
    }
}

///
/// \brief Checks state of requested encoder.
/// Internal function.
/// @param [in] encoderID   Encoder which is being checked.
/// @param [in] pairState   A and B signal readings from encoder placed into bits 0 and 1.
/// \returns 0 if encoder hasn't been moved, 1 if it's moving in positive and -1 if it's
/// moving in negative direction.
///
int8_t Board::readEncoder(uint8_t encoderID, uint8_t pairState)
{
    //add new data
    uint8_t newPairData = 0;
    newPairData |= (((encoderData[encoderID] << 2) & 0x000F) | (uint16_t)pairState);

    //remove old data
    encoderData[encoderID] &= ENCODER_CLEAR_TEMP_STATE_MASK;

    //shift in new data
    encoderData[encoderID] |= (uint16_t)newPairData;

    int8_t encRead = encoderLookUpTable[newPairData];

    if (!encRead)
        return 0;

    bool newEncoderDirection = encRead > 0;
    //get current number of pulses from encoderData
    int8_t currentPulses = (encoderData[encoderID] >> 4) & 0x000F;
    currentPulses += encRead;
    //clear current pulses
    encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;
    //shift in new pulse count
    encoderData[encoderID] |= (uint16_t)(currentPulses << 4);
    //get last encoder direction
    bool lastEncoderDirection = BIT_READ(encoderData[encoderID], ENCODER_DIRECTION_BIT);
    //write new encoder direction
    BIT_WRITE(encoderData[encoderID], ENCODER_DIRECTION_BIT, newEncoderDirection);

    if (lastEncoderDirection != newEncoderDirection)
        return 0;

    if (currentPulses % pulsesPerStep[encoderID])
        return 0;

    //clear current pulses
    encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;

    //set default pulse count
    encoderData[encoderID] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);

    if (!encInverted[encoderID])
    {
        if (newEncoderDirection)
            return 1;
        else
            return -1;
    }
    else
    {
        if (newEncoderDirection)
            return -1;
        else
            return 1;
    }
}

///
/// \brief Checks state of requested encoder.
/// @param [in] encoderID   Encoder which is being checked.
/// \returns 0 if encoder hasn't been moved, 1 if it's moving in positive and -1 if it's
/// moving in negative direction.
///
int8_t Board::getEncoderState(uint8_t encoderID)
{
    uint8_t column = encoderID % NUMBER_OF_BUTTON_COLUMNS;
    uint8_t row  = (encoderID/NUMBER_OF_BUTTON_COLUMNS)*2;
    uint8_t pairState = (digitalInBuffer[column] >> row) & 0x03;

    return readEncoder(encoderID, pairState);
}

///
/// \brief Checks if requested encoder ID is enabled.
/// @param [in] encoderNumber Encoder which is being checked.
/// \returns True if encoder is enabled, false otherwise.
///
bool Board::encoderEnabled(uint8_t encoderNumber)
{
    return encoderEnabledMap[encoderNumber];
}
