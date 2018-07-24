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

#include "Hardware.h"
#include "DataTypes.h"
#include "../constants/Analog.h"

///
/// \ingroup board
/// @{

///
/// \brief Holds currently active analog index which is being read.
/// Once all analog inputs are read, analog index is reset to 0.
///
extern uint8_t              analogIndex;

///
/// brief Holds currently active sample count.
/// Once all analog inputs are read, sample count is increased.
///
extern volatile uint8_t     analogSampleCounter;

///
/// \brief Holds press states for all pads.
///
extern volatile uint16_t    padPressed;

///
/// \brief Holds currently active coordinate reading for active pad.
///
extern uint8_t              padReadingIndex;

///
/// \brief Variable holding temporary pressure reading from first pad plate.
/// Pressure is read using two readings (X and Y plate). Difference between two
/// readings is proportional to resistance (pressure) between plates.
///
extern uint16_t             pressurePlate1;

///
/// \brief Array holding ADC samples for all three coordinates for all pads.
///
extern padData_t            analogInBuffer[ANALOG_IN_BUFFER_SIZE];

///
/// \brief Read only copy of ADC input buffer.
/// Used to avoid data overwrite from ISR.
///
extern padData_t            analogInBufferReadOnly;

///
/// \brief Holds currently active pad (pad which is being processed).
///
extern uint8_t              activePad;

///
/// \brief Holds "head" index position in ring buffer.
///
extern volatile uint8_t     aIn_head;

///
/// \brief Holds "tail" index position in ring buffer.
///
extern volatile uint8_t     aIn_tail;

///
/// \brief Holds current number of elements stored in ring buffer.
///
extern volatile uint8_t     aIn_count;

/// @}