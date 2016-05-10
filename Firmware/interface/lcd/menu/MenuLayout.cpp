#include "Menu.h"
#ifdef MENU_H_
#include "Options.h"

void Menu::createLayout()   {

    {   //root screen

        rootScreen.options = progmemArraySize(service_menu_options);

        rootScreen.function[MENU_OPTION_PAD_CALIBRATION] = NULL;
        rootScreen.function[MENU_OPTION_DEVICE_INFO] = NULL;
        rootScreen.function[MENU_OPTION_FACTORY_RESET] = NULL;

        {   //pad calibration screen

            menuScreen[MENU_OPTION_PAD_CALIBRATION].options = progmemArraySize(calibration_menu_options);

            menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_X] = NULL;
            menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_Y] = NULL;
            menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_PRESSURE] = NULL;

            {   //calibration direction screen

                menuScreen[MENU_OPTION_PAD_CALIBRATION].options = progmemArraySize(calibrationDirection_options);

                menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_X] = NULL;
                menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_Y] = NULL;
                menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_PRESSURE] = NULL;

            }

        }

        {   //device info screen

            menuScreen[MENU_OPTION_DEVICE_INFO].options = progmemArraySize(deviceInfo_options);

            menuScreen[MENU_OPTION_DEVICE_INFO].function[MENU_OPTION_DEVICE_INFO_SW] = NULL;
            menuScreen[MENU_OPTION_DEVICE_INFO].function[MENU_OPTION_DEVICE_INFO_HW] = NULL;

        }

        {   //factory reset screen

            menuScreen[MENU_OPTION_FACTORY_RESET].options = progmemArraySize(factoryReset_options);

            menuScreen[MENU_OPTION_FACTORY_RESET].function[MENUSCREEN_FACTORY_RESET_FULL] = NULL;
            menuScreen[MENU_OPTION_FACTORY_RESET].function[MENUSCREEN_FACTORY_RESET_PARTIAL] = NULL;

        }

    }

}
#endif