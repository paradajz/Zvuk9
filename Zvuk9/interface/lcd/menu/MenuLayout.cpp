#include "Menu.h"
#include "Options.h"
#include "MenuStrings.h"

void Menu::createLayout()   {

    {
        menuOption[OPTION_PAD_CALIBRATION].options = progmemArraySize(service_menu_options);
    }

}