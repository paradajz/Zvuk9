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

#pragma once

#include "DataTypes.h"
#include "../pads/Pads.h"
#include "../leds/LEDs.h"

class Buttons
{
    public:
    Buttons();
    void init();
    void update();
    void enable(int8_t buttonID = -1);
    void disable(int8_t buttonID = -1);
    bool getButtonEnableState(uint8_t buttonID);

    void setTransportControlType(transportControlType_t type);
    transportControlType_t getTransportControlType();

    //getters
    note_t getTonicFromButton(uint8_t buttonNumber);
    uint8_t getLastPressedButton();

    bool getButtonState(uint8_t buttonID);

    void setButtonState(uint8_t buttonID, bool state);
    void setMIDIchannelEnc(bool state);
    bool getMIDIchannelEnc();

    private:
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(note_t note, bool state);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);

    //button-to-led mapping
    void mapButtonsToNotes();

    void processButton(uint8_t button, uint8_t state);

    uint32_t                lastCheckTime;
    uint8_t                 buttonEnabled[MAX_NUMBER_OF_BUTTONS/8+1];
    bool                    processingEnabled;
    uint32_t                userMenuTimeout;
    transportControlType_t  transportControlType;
    uint8_t                 buttonToNoteArray[MAX_NUMBER_OF_BUTTONS];
    uint8_t                 lastPressedButton;
    uint8_t                 buttonPressed[MAX_NUMBER_OF_BUTTONS/8+1];
    bool                    midiChannelEncState;
};

extern Buttons buttons;
