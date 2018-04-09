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

#include "../../LCD.h"
#include "../../../pads/Pads.h"
#include "../../../encoders/Encoders.h"
#include "../../../../midi/src/MIDI.h"
#include "../Menu.h"
#include <avr/boot.h>

//uint16_t dev_signature[20];
//
//static inline void USB_Device_GetSerialString_test()    //testing only
//{
//
    //uint8_t curr_int = SREG;
    //cli();
//
    //uint8_t SigReadAddress = 0x0E;
//
    //printf_P("Device serial number: ");
//
    //for (uint8_t SerialCharNum = 0; SerialCharNum < 20; SerialCharNum++)
    //{
        //uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);
//
        //if (SerialCharNum & 0x01)
        //{
            //SerialByte >>= 4;
            //SigReadAddress++;
        //}
//
        //SerialByte &= 0x0F;
//
        //dev_signature[SerialCharNum] = /*cpu_to_le16*/((SerialByte >= 10) ?
        //(('A' - 10) + SerialByte) : ('0' + SerialByte));
//
        //printf_P("%d", dev_signature[SerialCharNum]);
    //}   printf_P("\n");
//
    //SREG = curr_int;
//}

//action functions

bool factoryReset(functionArgument argument);

bool deviceInfo(functionArgument argument);

bool enableCalibration(functionArgument argument);

//checkable items functions
bool checkRunningStatus(functionArgument argument);

bool checkTransportCC(functionArgument argument);

bool checkPitchBendType(functionArgument argument);

bool checkVelocitySensitivity(functionArgument argument);

bool checkPressureCurve(functionArgument argument);

bool checkAftertouchType(functionArgument argument);

bool checkNoteOffStatus(functionArgument argument);
