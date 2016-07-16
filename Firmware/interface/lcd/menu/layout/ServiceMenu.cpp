#include "../Menu.h"
#ifdef MENU_H_
#include "../items/ServiceMenu.h"

#include "../../../../version/Firmware.h"
#include "../../../../version/Hardware.h"
#include "../../../../eeprom/Configuration.h"
#include "../../../../init/Init.h"

#include "../functions/MenuFunctions.h"

void Menu::createServiceMenuLayout()   {

    menuItem[serviceMenuItem_calibration].stringPointer = menuOption_padCalibration;
    menuItem[serviceMenuItem_calibration].level = 1;
    menuItem[serviceMenuItem_calibration].function = checkCalibration;
    menuItem[serviceMenuItem_calibration].conditionCheck = true;

        {

            menuItem[serviceMenuItem_calibrateX].stringPointer = calibration_x;
            menuItem[serviceMenuItem_calibrateX].level = 11;
            menuItem[serviceMenuItem_calibrateX].function = enableCalibration;
            menuItem[serviceMenuItem_calibrateX].argument.argument1 = (uint8_t)coordinateX;

            menuItem[serviceMenuItem_calibrateY].stringPointer = calibration_y;
            menuItem[serviceMenuItem_calibrateY].level = 12;
            menuItem[serviceMenuItem_calibrateY].function = enableCalibration;
            menuItem[serviceMenuItem_calibrateY].argument.argument1 = (uint8_t)coordinateY;

            menuItem[serviceMenuItem_calibratePressure].stringPointer = calibration_pressure;
            menuItem[serviceMenuItem_calibratePressure].level = 13;
            menuItem[serviceMenuItem_calibratePressure].function = enableCalibration;
            menuItem[serviceMenuItem_calibratePressure].argument.argument1 = (uint8_t)coordinateZ;

        }

    menuItem[serviceMenuItem_deviceInfo].stringPointer = menuOption_deviceInfo;
    menuItem[serviceMenuItem_deviceInfo].level = 2;
    menuItem[serviceMenuItem_deviceInfo].function = deviceInfo;

    menuItem[serviceMenuItem_factoryReset].stringPointer = menuOption_factoryReset;
    menuItem[serviceMenuItem_factoryReset].level = 3;
    menuItem[serviceMenuItem_factoryReset].function = NULL;

        {
            menuItem[serviceMenuItem_partialReset].stringPointer = factoryReset_partial_string;
            menuItem[serviceMenuItem_partialReset].level = 31;
            menuItem[serviceMenuItem_partialReset].function = NULL;

                {
                    menuItem[serviceMenuItem_confirmPartialReset].stringPointer = confirm_partial_reset;
                    menuItem[serviceMenuItem_confirmPartialReset].level = 311;
                    menuItem[serviceMenuItem_confirmPartialReset].function = factoryReset;
                    menuItem[serviceMenuItem_confirmPartialReset].argument.argument1 = (uint8_t)factoryReset_partial;
                }

            menuItem[serviceMenuItem_fullReset].stringPointer = factoryReset_full_string;
            menuItem[serviceMenuItem_fullReset].level = 32;
            menuItem[serviceMenuItem_fullReset].function = NULL;

                {
                    menuItem[serviceMenuItem_confirmFullReset].stringPointer = confirm_full_reset;
                    menuItem[serviceMenuItem_confirmFullReset].level = 321;
                    menuItem[serviceMenuItem_confirmFullReset].function = factoryReset;
                    menuItem[serviceMenuItem_confirmFullReset].argument.argument1 = (uint8_t)factoryReset_wipeRestore;
                }
        }

}
#endif