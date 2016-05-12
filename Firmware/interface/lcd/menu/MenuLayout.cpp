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
                    }

                menuItem[menuItem_calibrateXupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateXupper].level = 112;
            }

            menuItem[menuItem_calibrateY].stringPointer = calibration_y;
            menuItem[menuItem_calibrateY].level = 12;

            {
                menuItem[menuItem_calibrateYlower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibrateYlower].level = 121;

                menuItem[menuItem_calibrateYupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateYupper].level = 122;
            }

            menuItem[menuItem_calibratePressure].stringPointer = calibration_pressure;
            menuItem[menuItem_calibratePressure].level = 13;

            {
                menuItem[menuItem_calibratePressureLower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibratePressureLower].level = 131;

                menuItem[menuItem_calibratePressureUpper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibratePressureUpper].level = 132;
            }

        }

}
#endif