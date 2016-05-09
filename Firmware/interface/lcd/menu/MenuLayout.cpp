#include "Menu.h"
#ifdef MENU_H_
#include "Options.h"

void Menu::createLayout()   {

    {
        menuOption[OPTION_PAD_CALIBRATION].options = progmemCharArraySize(service_menu_options);
    }

}
#endif