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

#include "Handlers.h"
#include "../../../../../board/constants/Hardware.h"
#include "../../../../../board/map/Encoders.h"

void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, int8_t steps);

///
/// \brief Initializes handlers for all encoders.
///
void initHandlers_encoders()
{
    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
        encoderHandler[i] = NULL;

    encoderHandler[PROGRAM_ENCODER] = handleProgram;
    encoderHandler[PRESET_ENCODER] = handleScale;

    encoderHandler[X_CC_ENCODER] = handleCC;
    encoderHandler[Y_CC_ENCODER] = handleCC;

    encoderHandler[X_MAX_ENCODER] = handleLimit;
    encoderHandler[X_MIN_ENCODER] = handleLimit;
    encoderHandler[Y_MAX_ENCODER] = handleLimit;
    encoderHandler[Y_MIN_ENCODER] = handleLimit;

    encoderHandler[X_CURVE_ENCODER] = handleCurve;
    encoderHandler[Y_CURVE_ENCODER] = handleCurve;
}
