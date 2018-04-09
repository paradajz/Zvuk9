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

#include "../Menu.h"
#include "../items/ServiceMenu.h"
#include "../../../../database/Database.h"

#include "../functions/MenuFunctions.h"

void Menu::createServiceMenuLayout()
{
    menuItem[serviceMenuItem_calibration].stringPointer = menuOption_padCalibration_string;
    menuItem[serviceMenuItem_calibration].level = 1;
    menuItem[serviceMenuItem_calibration].function = NULL;

        {

            menuItem[serviceMenuItem_calibrateX].stringPointer = calibration_x_string;
            menuItem[serviceMenuItem_calibrateX].level = 11;
            menuItem[serviceMenuItem_calibrateX].function = enableCalibration;
            menuItem[serviceMenuItem_calibrateX].argument.argument1 = (uint8_t)coordinateX;

            menuItem[serviceMenuItem_calibrateY].stringPointer = calibration_y_string;
            menuItem[serviceMenuItem_calibrateY].level = 12;
            menuItem[serviceMenuItem_calibrateY].function = enableCalibration;
            menuItem[serviceMenuItem_calibrateY].argument.argument1 = (uint8_t)coordinateY;

            menuItem[serviceMenuItem_calibratePressure].stringPointer = calibration_pressure_string;
            menuItem[serviceMenuItem_calibratePressure].level = 13;
            menuItem[serviceMenuItem_calibratePressure].function = enableCalibration;
            menuItem[serviceMenuItem_calibratePressure].argument.argument1 = (uint8_t)coordinateZ;

        }

    menuItem[serviceMenuItem_deviceInfo].stringPointer = menuOption_deviceInfo_string;
    menuItem[serviceMenuItem_deviceInfo].level = 2;
    menuItem[serviceMenuItem_deviceInfo].function = deviceInfo;

    menuItem[serviceMenuItem_factoryReset].stringPointer = menuOption_factoryReset_string;
    menuItem[serviceMenuItem_factoryReset].level = 3;
    menuItem[serviceMenuItem_factoryReset].function = factoryReset;
    menuItem[serviceMenuItem_factoryReset].argument.argument1 = (uint8_t)initWipe;
}
