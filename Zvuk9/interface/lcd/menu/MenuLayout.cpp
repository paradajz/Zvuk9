#include "Menu.h"
#include "Options.h"

void Menu::createLayout()   {

    {
        menuOption[OPTION_PAD_CALIBRATION].options = progmemArraySize(service_menu_options);
    }

}