#include "Menu.h"
#ifdef MENU_H_
#include "Items.h"

#include "../../../version/Firmware.h"
#include "../../../version/Hardware.h"
#include "../../../eeprom/EEPROMsettings.h"
#include "../../../init/Init.h"

#include "MenuFunctions.h"

void Menu::createLayout()   {

    menuItem[menuItem_calibration].stringPointer = serviceMenuOption_padCalibration;
    menuItem[menuItem_calibration].level = 1;
    menuItem[menuItem_calibration].function = NULL;

        {

            menuItem[menuItem_calibrateX].stringPointer = calibration_x;
            menuItem[menuItem_calibrateX].level = 11;
            menuItem[menuItem_calibrateX].function = enableCalibration_x;

            menuItem[menuItem_calibrateY].stringPointer = calibration_y;
            menuItem[menuItem_calibrateY].level = 12;
            menuItem[menuItem_calibrateY].function = enableCalibration_y;

            menuItem[menuItem_calibratePressure].stringPointer = calibration_pressure;
            menuItem[menuItem_calibratePressure].level = 13;
            menuItem[menuItem_calibratePressure].function = NULL;

            {
                menuItem[menuItem_calibratePressureLower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibratePressureLower].level = 131;
                menuItem[menuItem_calibratePressureLower].function = NULL;

                    {
                        menuItem[menuItem_calibratePressureLowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureLowerInteractive].level = 1311;
                        menuItem[menuItem_calibratePressureLowerInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibratePressureLowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureLowerManual].level = 1312;
                        menuItem[menuItem_calibratePressureLowerManual].function = runMenuFunction;
                    }

                menuItem[menuItem_calibratePressureUpper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibratePressureUpper].level = 132;
                menuItem[menuItem_calibratePressureUpper].function = NULL;

                    {
                        menuItem[menuItem_calibratePressureUpperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureUpperInteractive].level = 1321;
                        menuItem[menuItem_calibratePressureUpperInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibratePressureUpperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureUpperManual].level = 1322;
                        menuItem[menuItem_calibratePressureUpperManual].function = runMenuFunction;
                    }
            }

        }

    menuItem[menuItem_deviceInfo].stringPointer = serviceMenuOption_deviceInfo;
    menuItem[menuItem_deviceInfo].level = 2;
    menuItem[menuItem_deviceInfo].function = NULL;

        {
            menuItem[menuItem_softwareInfo].stringPointer = deviceInfo_swVersion;
            menuItem[menuItem_softwareInfo].level = 21;
            menuItem[menuItem_softwareInfo].function = displayFirmwareVersion;

            menuItem[menuItem_hardwareInfo].stringPointer = deviceInfo_hwVersion;
            menuItem[menuItem_hardwareInfo].level = 22;
            menuItem[menuItem_hardwareInfo].function = displayHardwareVersion;
        }

    menuItem[menuItem_factoryReset].stringPointer = serviceMenuOption_factoryReset;
    menuItem[menuItem_factoryReset].level = 3;
    menuItem[menuItem_factoryReset].function = NULL;

        {
            menuItem[menuItem_partialReset].stringPointer = factoryReset_partial_string;
            menuItem[menuItem_partialReset].level = 31;
            menuItem[menuItem_partialReset].function = NULL;

                {
                    menuItem[menuItem_confirmPartialReset].stringPointer = confirm_partial_reset;
                    menuItem[menuItem_confirmPartialReset].level = 311;
                    menuItem[menuItem_confirmPartialReset].function = partialReset;
                }

            menuItem[menuItem_fullReset].stringPointer = factoryReset_full_string;
            menuItem[menuItem_fullReset].level = 32;
            menuItem[menuItem_fullReset].function = NULL;

                {
                        menuItem[menuItem_confirmFullReset].stringPointer = confirm_full_reset;
                        menuItem[menuItem_confirmFullReset].level = 321;
                        menuItem[menuItem_confirmFullReset].function = fullReset;
                }
        }

}
#endif