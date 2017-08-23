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
