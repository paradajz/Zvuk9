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
#include "../items/UserMenu.h"
#include "../../../../database/Database.h"
#include "../functions/MenuFunctions.h"

void Menu::createUserMenuLayout()
{
    menuItem[userMenuItem_pressureSettings].stringPointer = menuOption_pressureSettings_string;
    menuItem[userMenuItem_pressureSettings].level = 1;
    menuItem[userMenuItem_pressureSettings].function = NULL;

    {
        menuItem[userMenuItem_pressureSensitivity].stringPointer = menuOption_pressureSensitivity_string;
        menuItem[userMenuItem_pressureSensitivity].level = 11;
        menuItem[userMenuItem_pressureSensitivity].function = NULL;

        {
            menuItem[userMenuItem_pressureSensitivity_1].stringPointer = pressure_sensitivity_1_string;
            menuItem[userMenuItem_pressureSensitivity_1].level = 111;
            menuItem[userMenuItem_pressureSensitivity_1].checkable = true;
            menuItem[userMenuItem_pressureSensitivity_1].function = checkVelocitySensitivity;
            menuItem[userMenuItem_pressureSensitivity_1].argument.argument1 = (uint8_t)velocity_soft;

            menuItem[userMenuItem_pressureSensitivity_2].stringPointer = pressure_sensitivity_2_string;
            menuItem[userMenuItem_pressureSensitivity_2].level = 112;
            menuItem[userMenuItem_pressureSensitivity_2].checkable = true;
            menuItem[userMenuItem_pressureSensitivity_2].function = checkVelocitySensitivity;
            menuItem[userMenuItem_pressureSensitivity_2].argument.argument1 = (uint8_t)velocity_medium;

            menuItem[userMenuItem_pressureSensitivity_3].stringPointer = pressure_sensitivity_3_string;
            menuItem[userMenuItem_pressureSensitivity_3].level = 113;
            menuItem[userMenuItem_pressureSensitivity_3].checkable = true;
            menuItem[userMenuItem_pressureSensitivity_3].function = checkVelocitySensitivity;
            menuItem[userMenuItem_pressureSensitivity_3].argument.argument1 = (uint8_t)velocity_hard;
        }

        menuItem[userMenuItem_pressureCurve].stringPointer = menuOption_pressureCurve_string;
        menuItem[userMenuItem_pressureCurve].level = 12;
        menuItem[userMenuItem_pressureCurve].function = NULL;

        {
            menuItem[userMenuItem_pressureCurve_linear].stringPointer = pressure_curve_linear_string;
            menuItem[userMenuItem_pressureCurve_linear].level = 121;
            menuItem[userMenuItem_pressureCurve_linear].checkable = true;
            menuItem[userMenuItem_pressureCurve_linear].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_linear].argument.argument1 = (uint8_t)curve_linear_up;

            menuItem[userMenuItem_pressureCurve_log].stringPointer = pressure_curve_log_string;
            menuItem[userMenuItem_pressureCurve_log].level = 122;
            menuItem[userMenuItem_pressureCurve_log].checkable = true;
            menuItem[userMenuItem_pressureCurve_log].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_log].argument.argument1 = (uint8_t)curve_log_up_1;

            menuItem[userMenuItem_pressureCurve_exp].stringPointer = pressure_curve_inv_exp_string;
            menuItem[userMenuItem_pressureCurve_exp].level = 123;
            menuItem[userMenuItem_pressureCurve_exp].checkable = true;
            menuItem[userMenuItem_pressureCurve_exp].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_exp].argument.argument1 = (uint8_t)curve_exp_up;
        }
    }

    menuItem[userMenuItem_midiSettings].stringPointer = menuOption_midiSettings_string;
    menuItem[userMenuItem_midiSettings].level = 2;
    menuItem[userMenuItem_midiSettings].function = NULL;

        {
            menuItem[userMenuItem_midiSettings_aftertouchType].stringPointer = midiSettings_atouchType_string;
            menuItem[userMenuItem_midiSettings_aftertouchType].level = 21;
            menuItem[userMenuItem_midiSettings_aftertouchType].function = NULL;

                {
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].stringPointer = aftertouchType_channel_string;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].level = 211;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].function = checkAftertouchType;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].argument.argument1 = (uint8_t)aftertouchChannel;

                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].stringPointer = aftertouchType_poly_string;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].level = 212;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].function = checkAftertouchType;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].argument.argument1 = (uint8_t)aftertouchPoly;
                }

            menuItem[userMenuItem_midiSettings_runningStatus].stringPointer = midiSettings_runningStatus_string;
            menuItem[userMenuItem_midiSettings_runningStatus].level = 22;
            menuItem[userMenuItem_midiSettings_runningStatus].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].stringPointer = enabled_string_string;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].level = 221;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].function = checkRunningStatus;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].argument.argument1 = 1;

                menuItem[userMenuItem_midiSettings_runningStatus_disabled].stringPointer = disabled_string_string;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].level = 222;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].function = checkRunningStatus;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].argument.argument1 = 0;
            }

            menuItem[userMenuItem_midiSettings_noteOff].stringPointer = midiSettings_noteOff_string;
            menuItem[userMenuItem_midiSettings_noteOff].level = 23;
            menuItem[userMenuItem_midiSettings_noteOff].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_noteOff_channel].stringPointer = noteOff_offChannel_string;
                menuItem[userMenuItem_midiSettings_noteOff_channel].level = 231;
                menuItem[userMenuItem_midiSettings_noteOff_channel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_channel].function = checkNoteOffStatus;
                menuItem[userMenuItem_midiSettings_noteOff_channel].argument.argument1 = noteOffType_standardNoteOff;

                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].stringPointer = noteOff_zeroVel_string;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].level = 232;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].function = checkNoteOffStatus;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].argument.argument1 = noteOffType_noteOnZeroVel;
            }

            menuItem[userMenuItem_midiSettings_transport].stringPointer = midiSettings_transport_string;
            menuItem[userMenuItem_midiSettings_transport].level = 24;
            menuItem[userMenuItem_midiSettings_transport].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_transport_mmc].stringPointer = midiSettings_transport_MMC_string;
                menuItem[userMenuItem_midiSettings_transport_mmc].level = 241;
                menuItem[userMenuItem_midiSettings_transport_mmc].checkable = true;
                menuItem[userMenuItem_midiSettings_transport_mmc].function = checkTransportCC;
                menuItem[userMenuItem_midiSettings_transport_mmc].argument.argument1 = transportMMC;

                menuItem[userMenuItem_midiSettings_transport_cc].stringPointer = midiSettings_transport_CC_string;
                menuItem[userMenuItem_midiSettings_transport_cc].level = 242;
                menuItem[userMenuItem_midiSettings_transport_cc].checkable = true;
                menuItem[userMenuItem_midiSettings_transport_cc].function = checkTransportCC;
                menuItem[userMenuItem_midiSettings_transport_cc].argument.argument1 = transportCC;

                menuItem[userMenuItem_midiSettings_transport_mmc_cc].stringPointer = midiSettings_transport_MMC_CC_string;
                menuItem[userMenuItem_midiSettings_transport_mmc_cc].level = 243;
                menuItem[userMenuItem_midiSettings_transport_mmc_cc].checkable = true;
                menuItem[userMenuItem_midiSettings_transport_mmc_cc].function = checkTransportCC;
                menuItem[userMenuItem_midiSettings_transport_mmc_cc].argument.argument1 = transportMMC_CC;
            }

            menuItem[userMenuItem_midiSettings_pitchBend].stringPointer = midiSettings_pitchBend_string;
            menuItem[userMenuItem_midiSettings_pitchBend].level = 25;
            menuItem[userMenuItem_midiSettings_pitchBend].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_pitchBend_1].stringPointer = midiSettings_pitchBend_1_string;
                menuItem[userMenuItem_midiSettings_pitchBend_1].level = 251;
                menuItem[userMenuItem_midiSettings_pitchBend_1].checkable = true;
                menuItem[userMenuItem_midiSettings_pitchBend_1].function = checkPitchBendType;
                menuItem[userMenuItem_midiSettings_pitchBend_1].argument.argument1 = pitchBend1;

                menuItem[userMenuItem_midiSettings_pitchBend_2].stringPointer = midiSettings_pitchBend_2_string;
                menuItem[userMenuItem_midiSettings_pitchBend_2].level = 252;
                menuItem[userMenuItem_midiSettings_pitchBend_2].checkable = true;
                menuItem[userMenuItem_midiSettings_pitchBend_2].function = checkPitchBendType;
                menuItem[userMenuItem_midiSettings_pitchBend_2].argument.argument1 = pitchBend2;
            }
        }

    menuItem[userMenuItem_deviceInfo].stringPointer = menuOption_deviceInfo_string;
    menuItem[userMenuItem_deviceInfo].level = 3;
    menuItem[userMenuItem_deviceInfo].function = deviceInfo;

    menuItem[userMenuItem_factoryReset].stringPointer = menuOption_factoryReset_string;
    menuItem[userMenuItem_factoryReset].level = 4;
    menuItem[userMenuItem_factoryReset].function = factoryReset;
    menuItem[userMenuItem_factoryReset].argument.argument1 = (uint8_t)initPartial;
}