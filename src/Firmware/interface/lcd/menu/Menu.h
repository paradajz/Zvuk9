#pragma once

#include "../LCD.h"

#include "../Macros.h"
#include "../strings/Strings.h"
#include "items/ServiceMenu.h"
#include "items/UserMenu.h"

#define MAX_MENU_OPTIONS    USER_MENU_ITEMS //more items in user menu than service

typedef struct
{
    uint8_t argument1;
    uint8_t argument2;
} functionArgument;

class Menu
{
    public:
    Menu();
    void exitMenu();
    void show(menuType_t type);
    bool isMenuDisplayed();
    void changeOption(bool direction);
    void confirmOption(bool confirm);
    void createServiceMenuLayout();
    void createUserMenuLayout();
    void createQuickMenuLayout();
    void stopFunction();

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeLevel;
    uint16_t menuHierarchyPosition;
    uint8_t items;
    uint8_t indexes[MAX_MENU_OPTIONS];
    bool functionRunning;

    void setMenuTitle(bool rootTitle);
    void updateMenuScreen();
    void getMenuItems();
    uint8_t getMenuSize();

    typedef bool (*actionPointer)(functionArgument argument);

    typedef struct
    {
        //denotes item level in menu hierarchy, ie. 1.2.3.1.
        uint16_t level;
        //pointer to function if any
        actionPointer function;
        //pointer to string to show item name
        const char *stringPointer;
        bool checkable;
        functionArgument argument;
    } menuItem_t;

    menuItem_t menuItem[MAX_MENU_OPTIONS];
    uint8_t menuSize;
};

extern Menu menu;
