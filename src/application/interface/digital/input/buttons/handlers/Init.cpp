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
#include "../../../../../board/common/Common.h"

void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

///
/// \brief Initializes handlers for all buttons.
///
void initHandlers_buttons()
{
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
        buttonHandler[i] = NULL;

    //transport controls
    buttonHandler[BUTTON_TRANSPORT_PLAY] = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_STOP] = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_RECORD] = handleTransportControl;

    //on/ff
    buttonHandler[BUTTON_ON_OFF_NOTES] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_AFTERTOUCH] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_X] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_Y] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_SPLIT] = handleOnOff;

    //up/down
    buttonHandler[BUTTON_OCTAVE_DOWN] = handleUpDown;
    buttonHandler[BUTTON_OCTAVE_UP] = handleUpDown;

    //notes
    buttonHandler[BUTTON_NOTE_C_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_D_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_F_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_G_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_A_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_C] = handleTonic;
    buttonHandler[BUTTON_NOTE_D] = handleTonic;
    buttonHandler[BUTTON_NOTE_E] = handleTonic;
    buttonHandler[BUTTON_NOTE_F] = handleTonic;
    buttonHandler[BUTTON_NOTE_G] = handleTonic;
    buttonHandler[BUTTON_NOTE_A] = handleTonic;
    buttonHandler[BUTTON_NOTE_B] = handleTonic;

    buttonHandler[BUTTON_PROGRAM_ENC] = handleProgramEncButton;
    buttonHandler[BUTTON_PRESET_ENC] = handlePresetEncButton;
}
