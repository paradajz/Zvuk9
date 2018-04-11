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

#include "Variables.h"
#include "map/Map.h"
#include "display/u8g2_wrapper.h"
#include "pins/Pins.h"
#include "DataTypes.h"
#include "../interface/digital/output/leds/Variables.h"

///
/// \brief Hardcoded board revision.
/// @{
///

#define HARDWARE_VERSION_MAJOR      3
#define HARDWARE_VERSION_MINOR      0
#define HARDWARE_VERSION_REVISION   0

/// @}

class Board
{
    public:
    Board();
    void init();
    bool encoderEnabled(uint8_t encoderNumber);
    uint16_t getPadPressure(uint8_t pad);
    int16_t getPadX(uint8_t pad);
    int16_t getPadY(uint8_t pad);
    void reboot(rebootType_t type);
    static bool checkNewRevision();

    //digital in
    static bool digitalInputDataAvailable();
    static void continueDigitalInReadout();

    //buttons
    static bool getButtonState(uint8_t buttonIndex);

    //encoders
    static int8_t getEncoderState(uint8_t encoderID);

    private:
    static void initPins();
    static void initAnalog();
    static void initEncoders();
    static void initUSB_MIDI();
    static void initUART_MIDI(uint32_t baudRate, bool reInit);
    static void initTimers();
    static void initPads();
    static int8_t readEncoder(uint8_t encoderID, uint8_t pairState);
};

extern Board board;
