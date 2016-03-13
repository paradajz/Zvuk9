#include "Menu.h"

Menu::Menu()    {

    //default constructor
    activeMenu = noMenu;

}

void Menu::displayMenu(menuType_t type) {

    switch(type)    {

        case serviceMenu:
        lcDisplay.displayServiceMenu();
        activeMenu = serviceMenu;
        break;

    }

}

bool Menu::menuDisplayed()  {

    return (activeMenu != noMenu);

}

Menu menu;