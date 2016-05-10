#include "Menu.h"
#ifdef MENU_H_
#include "../strings/Strings.h"

Menu::Menu()    {

    //default constructor
    activeMenu = noMenu;
    activeOption = 0;
    activeSubOption = 0;

    for (int i=0; i<10; i++)
        menuHierarchyPosition[i] = -1;

}

void Menu::displayMenu(menuType_t type) {

    switch(type)    {

        case serviceMenu:
        display.displayServiceMenu();
        activeMenu = serviceMenu;
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

}

bool Menu::menuDisplayed()  {

    return (activeMenu != noMenu);

}

void Menu::changeOption(bool direction) {

    direction ? activeOption++ : activeOption--;

    switch(activeMenu)  {

        case serviceMenu:
        if (activeOption < 0) activeOption = 0;
        if (activeOption >= (int8_t)(progmemArraySize(service_menu_options))) activeOption--;
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

    display.changeMenuOption(activeMenu, activeOption, activeSubOption);

}

Menu menu;
#endif