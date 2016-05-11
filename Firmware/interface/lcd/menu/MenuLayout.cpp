#include "Menu.h"
#ifdef MENU_H_
#include "Options.h"

void Menu::createLayout()   {

    {   //root screen

        menu.rootScreen.options = progmemArraySize(service_menu_options);

        menu.rootScreen.function[MENU_OPTION_PAD_CALIBRATION] = NULL;
        menu. rootScreen.function[MENU_OPTION_DEVICE_INFO] = NULL;
        menu.rootScreen.function[MENU_OPTION_FACTORY_RESET] = NULL;

        {   //pad calibration screen

            menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].options = progmemArraySize(calibration_menu_options);

            menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_X] = NULL;
            menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_Y] = NULL;
            menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_PRESSURE] = NULL;

            {   //calibration direction screen

                menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].options = progmemArraySize(calibrationDirection_options);

                menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_X] = NULL;
                menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_Y] = NULL;
                menu.menuScreen[MENU_OPTION_PAD_CALIBRATION].function[MENU_OPTION_CALIBRATE_PRESSURE] = NULL;

            }

        }

        {   //device info screen

            menu.menuScreen[MENU_OPTION_DEVICE_INFO].options = progmemArraySize(deviceInfo_options);

            menu.menuScreen[MENU_OPTION_DEVICE_INFO].function[MENU_OPTION_DEVICE_INFO_SW] = NULL;
            menu.menuScreen[MENU_OPTION_DEVICE_INFO].function[MENU_OPTION_DEVICE_INFO_HW] = NULL;

        }

        {   //factory reset screen

            menu.menuScreen[MENU_OPTION_FACTORY_RESET].options = progmemArraySize(factoryReset_options);

            menu.menuScreen[MENU_OPTION_FACTORY_RESET].function[MENUSCREEN_FACTORY_RESET_FULL] = NULL;
            menu.menuScreen[MENU_OPTION_FACTORY_RESET].function[MENUSCREEN_FACTORY_RESET_PARTIAL] = NULL;

        }

    }

}
#endif