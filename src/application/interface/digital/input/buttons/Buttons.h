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

///
/// \brief Button handling.
/// \defgroup interfaceButtons Buttons
/// \ingroup interfaceDigitalIn
/// @{

class Buttons
{
    public:
    Buttons();
    static void init();
    static void update();
    static void setButtonEnableState(int8_t buttonID, bool state);
    static bool getButtonEnableState(uint8_t buttonID);
    static void setTransportControlMode(transportControlMode_t type);
    static transportControlMode_t getTransportControlMode();
    static note_t getNoteFromButton(uint8_t buttonNumber);
    static bool getButtonState(uint8_t buttonID);
    static void setButtonState(uint8_t buttonID, bool state);

    private:
    static bool buttonDebounced(uint8_t buttonID, bool buttonState);
    static void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    static void handleTonicEvent(note_t note, bool state);
    static void handleOctaveEvent(bool direction, bool state);
    static void processButton(uint8_t button, uint8_t state);
};

///
/// \brief External definition of Buttons class instance.
///
extern Buttons buttons;

/// @}
