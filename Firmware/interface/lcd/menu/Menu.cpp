#include "Menu.h"
#ifdef MENU_H_
#include "../strings/Strings.h"

Menu::Menu()    {

    //default constructor
    activeMenu = noMenu;
    activeOption = 0;

    for (int i=0; i<MAX_MENU_LEVELS; i++)
        menuHierarchyPosition[i] = -1;

    menuHierarchyIndex = 0;

}

void Menu::displayMenu(menuType_t type) {

    switch(type)    {

        case serviceMenu:
        display.displayServiceMenu();
        activeMenu = serviceMenu;
        menuHierarchyPosition[menuHierarchyIndex] = 0;
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

    //this only changes index in current hierarchy level

    direction ? menuHierarchyIndex++ : menuHierarchyIndex--;

    if (menuHierarchyIndex < 0) activeOption = 0;
    if (activeOption >= MAX_MENU_LEVELS) activeOption--;
    menuHierarchyPosition[menuHierarchyIndex] = activeOption;

    display.changeMenuOption(activeMenu, menuHierarchyPosition);

}

void Menu::confirmOption(bool confirm)  {

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument

    switch(confirm) {

        case true:
        switch(activeMenu)  {

            case serviceMenu:
            if (menuHierarchyIndex < (MAX_MENU_LEVELS-1))   {

                activeOption = 0;
                menuHierarchyIndex++;

            }
            break;

        }
        break;

        case false:
        if (menuHierarchyIndex > 0) {

            menuHierarchyPosition[menuHierarchyIndex] = -1;
            menuHierarchyIndex--;
            activeOption = 0;

        }
        break;

    }

    menuHierarchyPosition[menuHierarchyIndex] = 0;

    #if MODE_SERIAL > 0
        printf("Current position in menu:\n");
        for (int i=0; i<MAX_MENU_LEVELS; i++)    {

            printf("%d.", menuHierarchyPosition[i]);

        } printf("\n");

        printf("Menu hierarchy index: %d\n", menuHierarchyIndex);
    #endif

    //display.changeMenuScreen(activeMenu, menuHierarchyPosition);

}

Menu menu;
#endif