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

#include "Pads.h"
#include "curves/Curves.h"

Pads::Pads()
{
    for (int i = 0; i < NUMBER_OF_PADS; i++)
    {
        lastXCCvalue[i]        = DEFAULT_XY_AT_VALUE;
        lastYCCvalue[i]        = DEFAULT_XY_AT_VALUE;
        lastXPitchBendValue[i] = DEFAULT_PITCH_BEND_VALUE;
        lastYPitchBendValue[i] = DEFAULT_PITCH_BEND_VALUE;
        initialXposition[i]    = DEFAULT_INITIAL_XY_VALUE;
        initialYposition[i]    = DEFAULT_INITIAL_XY_VALUE;

        lastAftertouchValue[i] = DEFAULT_XY_AT_VALUE;

        for (int j = 0; j < NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;
    }

    activeProgram       = -1;
    activeScale         = -1;
    activePadEditOctave = DEFAULT_OCTAVE;
}

/// Initialize all parameters and settings.
void Pads::init()
{
    getConfiguration();
    curves.init();
    setActiveNoteLEDs(false, 0);
}