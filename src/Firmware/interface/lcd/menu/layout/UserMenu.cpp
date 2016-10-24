#include "../Menu.h"
#ifdef MENU_H_
#include "../items/UserMenu.h"

#include "../../../../version/Firmware.h"
#include "../../../../version/Hardware.h"
#include "../../../../eeprom/Configuration.h"
#include "../../../../midi/DataTypes.h"
#include "../functions/MenuFunctions.h"

void Menu::createUserMenuLayout()   {

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
            menuItem[userMenuItem_pressureSensitivity_1].function = checkPressureSensitivity;
            menuItem[userMenuItem_pressureSensitivity_1].argument.argument1 = (uint8_t)pressure_soft;

            menuItem[userMenuItem_pressureSensitivity_2].stringPointer = pressure_sensitivity_2_string;
            menuItem[userMenuItem_pressureSensitivity_2].level = 112;
            menuItem[userMenuItem_pressureSensitivity_2].checkable = true;
            menuItem[userMenuItem_pressureSensitivity_2].function = checkPressureSensitivity;
            menuItem[userMenuItem_pressureSensitivity_2].argument.argument1 = (uint8_t)pressure_medium;

            menuItem[userMenuItem_pressureSensitivity_3].stringPointer = pressure_sensitivity_3_string;
            menuItem[userMenuItem_pressureSensitivity_3].level = 113;
            menuItem[userMenuItem_pressureSensitivity_3].checkable = true;
            menuItem[userMenuItem_pressureSensitivity_3].function = checkPressureSensitivity;
            menuItem[userMenuItem_pressureSensitivity_3].argument.argument1 = (uint8_t)pressure_hard;
        }

        menuItem[userMenuItem_pressureCurve].stringPointer = menuOption_pressureCurve_string;
        menuItem[userMenuItem_pressureCurve].level = 12;
        menuItem[userMenuItem_pressureCurve].function = NULL;

        {
            menuItem[userMenuItem_pressureCurve_linear].stringPointer = pressure_curve_linear_string;
            menuItem[userMenuItem_pressureCurve_linear].level = 121;
            menuItem[userMenuItem_pressureCurve_linear].checkable = true;
            menuItem[userMenuItem_pressureCurve_linear].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_linear].argument.argument1 = (uint8_t)pressureCurve_linear;

            menuItem[userMenuItem_pressureCurve_log].stringPointer = pressure_curve_log_string;
            menuItem[userMenuItem_pressureCurve_log].level = 122;
            menuItem[userMenuItem_pressureCurve_log].checkable = true;
            menuItem[userMenuItem_pressureCurve_log].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_log].argument.argument1 = (uint8_t)pressureCurve_log;

            menuItem[userMenuItem_pressureCurve_exp].stringPointer = pressure_curve_inv_exp_string;
            menuItem[userMenuItem_pressureCurve_exp].level = 123;
            menuItem[userMenuItem_pressureCurve_exp].checkable = true;
            menuItem[userMenuItem_pressureCurve_exp].function = checkPressureCurve;
            menuItem[userMenuItem_pressureCurve_exp].argument.argument1 = (uint8_t)pressureCurve_exp;
        }
    }

    menuItem[userMenuItem_midiSettings].stringPointer = menuOption_midiSettings_string;
    menuItem[userMenuItem_midiSettings].level = 3;
    menuItem[userMenuItem_midiSettings].function = NULL;

        {
            menuItem[userMenuItem_midiSettings_aftertouchType].stringPointer = midiSettings_atouchType_string;
            menuItem[userMenuItem_midiSettings_aftertouchType].level = 31;
            menuItem[userMenuItem_midiSettings_aftertouchType].function = NULL;

                {
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].stringPointer = aftertouchType_channel_string;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].level = 311;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].function = checkAftertouchType;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].argument.argument1 = (uint8_t)aftertouchChannel;

                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].stringPointer = aftertouchType_poly_string;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].level = 312;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].function = checkAftertouchType;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].argument.argument1 = (uint8_t)aftertouchPoly;
                }

            menuItem[userMenuItem_midiSettings_runningStatus].stringPointer = midiSettings_runningStatus_string;
            menuItem[userMenuItem_midiSettings_runningStatus].level = 32;
            menuItem[userMenuItem_midiSettings_runningStatus].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].stringPointer = enabled_string_string;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].level = 321;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].function = checkRunningStatus;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].argument.argument1 = 1;

                menuItem[userMenuItem_midiSettings_runningStatus_disabled].stringPointer = disabled_string_string;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].level = 322;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].function = checkRunningStatus;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].argument.argument1 = 0;
            }

            menuItem[userMenuItem_midiSettings_noteOff].stringPointer = midiSettings_noteOff_string;
            menuItem[userMenuItem_midiSettings_noteOff].level = 33;
            menuItem[userMenuItem_midiSettings_noteOff].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_noteOff_channel].stringPointer = noteOff_offChannel_string;
                menuItem[userMenuItem_midiSettings_noteOff_channel].level = 331;
                menuItem[userMenuItem_midiSettings_noteOff_channel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_channel].function = checkNoteOffStatus;
                menuItem[userMenuItem_midiSettings_noteOff_channel].argument.argument1 = (uint8_t)noteOffType_standardNoteOff;

                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].stringPointer = noteOff_zeroVel_string;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].level = 332;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].function = checkNoteOffStatus;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].argument.argument1 = (uint8_t)noteOffType_noteOnZeroVel;
            }
        }

    menuItem[userMenuItem_deviceInfo].stringPointer = menuOption_deviceInfo_string;
    menuItem[userMenuItem_deviceInfo].level = 4;
    menuItem[userMenuItem_deviceInfo].function = deviceInfo;

    menuItem[userMenuItem_factoryReset].stringPointer = menuOption_factoryReset_string;
    menuItem[userMenuItem_factoryReset].level = 5;
    menuItem[userMenuItem_factoryReset].function = factoryReset;
    menuItem[userMenuItem_factoryReset].argument.argument1 = (uint8_t)factoryReset_partial;

}
#endif