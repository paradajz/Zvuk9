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

#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#include "handlers/Handlers.h"

extern void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, int8_t steps);

///
/// \brief Default constructor.
///
Encoders::Encoders()
{
    
}

///
/// \brief Initializes Encoders object.
///
void Encoders::init()
{
    initHandlers_encoders();
}

///
/// \brief Continuously checks state of all encoders.
/// @param [in] If set to true, data from encoders won't be processed. Set to true by default.
///
void Encoders::update(bool process)
{
    int8_t steps;

    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        if (lastStepTime[i] < 255)
            lastStepTime[i]++;

        if (!board.encoderEnabled(i))
            continue;

        steps = board.getEncoderState(i);

        if (!process)
            continue;

        if (steps == 0)
            continue;

        if (lastStepTime[i] < SPEED_TIMEOUT)
        {
            encoderSpeed[i] += ENCODER_SPEED_CHANGE;
            steps = steps > 0 ? encoderSpeed[i] : -encoderSpeed[i];
        }
        else
        {
            encoderSpeed[i] = 0;
        }

        //allow only program and scale encoder while in menu
        //no message on display? maybe TO-DO
        if (menu.isMenuDisplayed())
        {
            if (!((i == PROGRAM_ENCODER) || (i == X_MAX_ENCODER) || (i == X_MIN_ENCODER) || (i == Y_MAX_ENCODER) || (i == Y_MIN_ENCODER)))
                continue;
        }

        if (encoderHandler[i] != NULL)
        {
            (*encoderHandler[i])(i, steps);
            lastStepTime[i] = 0;
        }
    }
}

///
/// \brief Flushes all data from encoders.
///
void Encoders::flush()
{
    update(false);
}

Encoders encoders;
