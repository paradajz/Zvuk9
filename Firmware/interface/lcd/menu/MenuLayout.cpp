#include "Menu.h"
#ifdef MENU_H_
#include "Options.h"

void Menu::createLayout()   {

    menuItem[menuItem_calibration].stringPointer = serviceMenuOption_padCalibration;
    menuItem[menuItem_calibration].level = 1;

        {

            menuItem[menuItem_calibrateX].stringPointer = calibration_x;
            menuItem[menuItem_calibrateX].level = 11;

            {
                menuItem[menuItem_calibrateXlower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibrateXlower].level = 111;

                    {
                        menuItem[menuItem_calibrateXlowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateXlowerInteractive].level = 1111;

                        menuItem[menuItem_calibrateXlowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateXlowerManual].level = 1112;
                    }

                menuItem[menuItem_calibrateXupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateXupper].level = 112;

                    {
                        menuItem[menuItem_calibrateXupperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateXupperInteractive].level = 1121;

                        menuItem[menuItem_calibrateXupperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateXupperManual].level = 1122;
                    }
            }

            menuItem[menuItem_calibrateY].stringPointer = calibration_y;
            menuItem[menuItem_calibrateY].level = 12;

            {
                menuItem[menuItem_calibrateYlower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibrateYlower].level = 121;

                    {
                        menuItem[menuItem_calibrateYlowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateYlowerInteractive].level = 1211;

                        menuItem[menuItem_calibrateYlowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateYlowerManual].level = 1212;
                    }

                menuItem[menuItem_calibrateYupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateYupper].level = 122;

                    {
                        menuItem[menuItem_calibrateYupperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateYupperInteractive].level = 1221;

                        menuItem[menuItem_calibrateYupperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateYupperManual].level = 1222;
                    }
            }

            menuItem[menuItem_calibratePressure].stringPointer = calibration_pressure;
            menuItem[menuItem_calibratePressure].level = 13;

            {
                menuItem[menuItem_calibratePressureLower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibratePressureLower].level = 131;

                    {
                        menuItem[menuItem_calibratePressureLowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureLowerInteractive].level = 1311;

                        menuItem[menuItem_calibratePressureLowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureLowerManual].level = 1312;
                    }

                menuItem[menuItem_calibratePressureUpper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibratePressureUpper].level = 132;

                    {
                        menuItem[menuItem_calibratePressureUpperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureUpperInteractive].level = 1321;

                        menuItem[menuItem_calibratePressureUpperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureUpperManual].level = 1322;
                    }
            }

        }

    menuItem[menuItem_deviceInfo].stringPointer = serviceMenuOption_deviceInfo;
    menuItem[menuItem_deviceInfo].level = 2;

        {
            menuItem[menuItem_softwareInfo].stringPointer = deviceInfo_swVersion;
            menuItem[menuItem_softwareInfo].level = 21;

            menuItem[menuItem_hardwareInfo].stringPointer = deviceInfo_hwVersion;
            menuItem[menuItem_hardwareInfo].level = 22;
        }

    menuItem[menuItem_factoryReset].stringPointer = serviceMenuOption_factoryReset;
    menuItem[menuItem_factoryReset].level = 3;

        {
            menuItem[menuItem_partialReset].stringPointer = factoryReset_partial;
            menuItem[menuItem_partialReset].level = 31;

            menuItem[menuItem_fullReset].stringPointer = factoryReset_full;
            menuItem[menuItem_fullReset].level = 32;
        }

}
#endif