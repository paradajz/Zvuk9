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
#include "../../../display/menu/Menu.h"
#include "handlers/Handlers.h"
#include "pins/map/Encoders.h"


///
/// \ingroup interfaceEncoders
/// @{

///
/// \brief External definition of function pointers used as handlers for encoders.
///
extern void     (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, int8_t steps);

///
/// \brief When set to true, preset encoder is used to change MIDI channels instead of presets.
///
bool            midiChannelEncState;

///
/// \brief Array holding last time in milliseconds encoders were moved.
///
uint32_t        lastStepTime[MAX_NUMBER_OF_ENCODERS];

///
/// \brief Array holding current speed (in steps) for all encoders.
/// Set to 1 by default.
///
int8_t          encoderSpeed[MAX_NUMBER_OF_ENCODERS];

/// @}

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
///
void Encoders::update()
{
    int8_t steps;

    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        //process only enabled encoders
        if (!board.encoderEnabled(i))
            continue;

        steps = board.getEncoderState(i, pulsesPerStep[i]);

        if (steps == 0)
            continue;

        //when time difference between two movements is smaller than SPEED_TIMEOUT,
        //start accelerating
        if ((rTimeMs() - lastStepTime[i]) < SPEED_TIMEOUT)
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
            lastStepTime[i] = rTimeMs();
        }
    }

    //disable midi channel function when regular text gets shown on display
    if (midiChannelEncState && (display.getActiveTextType() == displayText_still))
        midiChannelEncState = false;
}

///
/// \brief Enables or disables MIDI channel changing mode for preset encoder.
/// @param[in] state    If set to true, preset encoder will be used to change MIDI channels.
///                     Otherwise, presets are changed using this encoder.
///
void Encoders::setMIDIchannelPresetEncMode(bool state)
{
    midiChannelEncState = state;
}

///
/// \brief Checks if MIDI channel changing mode for preset encoder is enabled.
/// \returns True if MIDI channel changing mode is enabled, false otherwise.
///
bool Encoders::getMIDIchannelPresetEncMode()
{
    return midiChannelEncState;
}

Encoders encoders;
