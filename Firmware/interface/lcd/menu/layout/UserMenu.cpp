#include "../Menu.h"
#ifdef MENU_H_
#include "../items/UserMenu.h"

#include "../../../../version/Firmware.h"
#include "../../../../version/Hardware.h"
#include "../../../../eeprom/Configuration.h"
#include "../../../../init/Init.h"

#include "../functions/MenuFunctions.h"

void Menu::createUserMenuLayout()   {

    menuItem[userMenuItem_pressureSensitivity].stringPointer = menuOption_pressureSensitivity;
    menuItem[userMenuItem_pressureSensitivity].level = 1;
    menuItem[userMenuItem_pressureSensitivity].function = NULL;

    {

        menuItem[userMenuItem_pressureSensitivity_1].stringPointer = pressure_sensitivity_1;
        menuItem[userMenuItem_pressureSensitivity_1].level = 11;
        menuItem[userMenuItem_pressureSensitivity_1].checkable = true;
        menuItem[userMenuItem_pressureSensitivity_1].function = NULL;

        menuItem[userMenuItem_pressureSensitivity_2].stringPointer = pressure_sensitivity_2;
        menuItem[userMenuItem_pressureSensitivity_2].level = 12;
        menuItem[userMenuItem_pressureSensitivity_2].checkable = true;
        menuItem[userMenuItem_pressureSensitivity_2].function = NULL;

        menuItem[userMenuItem_pressureSensitivity_3].stringPointer = pressure_sensitivity_3;
        menuItem[userMenuItem_pressureSensitivity_3].level = 13;
        menuItem[userMenuItem_pressureSensitivity_3].checkable = true;
        menuItem[userMenuItem_pressureSensitivity_3].function = NULL;

    }

    menuItem[userMenuItem_pressureCurve].stringPointer = menuOption_pressureCurve;
    menuItem[userMenuItem_pressureCurve].level = 2;
    menuItem[userMenuItem_pressureCurve].function = NULL;

        {
            menuItem[userMenuItem_pressureCurve_linear].stringPointer = pressure_curve_lin;
            menuItem[userMenuItem_pressureCurve_linear].level = 21;
            menuItem[userMenuItem_pressureCurve_linear].checkable = true;
            menuItem[userMenuItem_pressureCurve_linear].function = NULL;

            menuItem[userMenuItem_pressureCurve_log].stringPointer = pressure_curve_log;
            menuItem[userMenuItem_pressureCurve_log].level = 22;
            menuItem[userMenuItem_pressureCurve_log].checkable = true;
            menuItem[userMenuItem_pressureCurve_log].function = NULL;

            menuItem[userMenuItem_pressureCurve_inv_log].stringPointer = pressure_curve_inv_log;
            menuItem[userMenuItem_pressureCurve_inv_log].level = 23;
            menuItem[userMenuItem_pressureCurve_inv_log].checkable = true;
            menuItem[userMenuItem_pressureCurve_inv_log].function = NULL;
        }

    menuItem[userMenuItem_midiSettings].stringPointer = menuOption_midiSettings;
    menuItem[userMenuItem_midiSettings].level = 3;
    menuItem[userMenuItem_midiSettings].function = NULL;

        {
            menuItem[userMenuItem_midiSettings_aftertouchType].stringPointer = midiSettings_atouchType;
            menuItem[userMenuItem_midiSettings_aftertouchType].level = 31;
            menuItem[userMenuItem_midiSettings_aftertouchType].function = NULL;

                {
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].stringPointer = aftertouchType_channel;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].level = 311;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_channel].function = NULL;

                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].stringPointer = aftertouchType_poly;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].level = 312;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].checkable = true;
                    menuItem[userMenuItem_midiSettings_aftertouchType_poly].function = NULL;
                }

            menuItem[userMenuItem_midiSettings_runningStatus].stringPointer = midiSettings_runningStatus;
            menuItem[userMenuItem_midiSettings_runningStatus].level = 32;
            menuItem[userMenuItem_midiSettings_runningStatus].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].stringPointer = enabled_string;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].level = 321;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_enabled].function = NULL;

                menuItem[userMenuItem_midiSettings_runningStatus_disabled].stringPointer = disabled_string;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].level = 322;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].checkable = true;
                menuItem[userMenuItem_midiSettings_runningStatus_disabled].function = NULL;
            }

            menuItem[userMenuItem_midiSettings_noteOff].stringPointer = midiSettings_noteOff;
            menuItem[userMenuItem_midiSettings_noteOff].level = 33;
            menuItem[userMenuItem_midiSettings_noteOff].function = NULL;

            {
                menuItem[userMenuItem_midiSettings_noteOff_channel].stringPointer = noteOff_offChannel;
                menuItem[userMenuItem_midiSettings_noteOff_channel].level = 331;
                menuItem[userMenuItem_midiSettings_noteOff_channel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_channel].function = NULL;

                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].stringPointer = noteOff_zeroVel;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].level = 332;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].checkable = true;
                menuItem[userMenuItem_midiSettings_noteOff_zero_vel].function = NULL;
            }
        }

    menuItem[userMenuItem_deviceInfo].stringPointer = menuOption_deviceInfo;
    menuItem[userMenuItem_deviceInfo].level = 4;
    menuItem[userMenuItem_deviceInfo].function = deviceInfo;

    menuItem[userMenuItem_factoryReset].stringPointer = menuOption_factoryReset;
    menuItem[userMenuItem_factoryReset].level = 5;
    menuItem[userMenuItem_factoryReset].function = NULL;

}
#endif