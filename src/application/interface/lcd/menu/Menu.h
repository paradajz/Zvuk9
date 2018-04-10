/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#pragma once

#include "../LCD.h"
#include "../Macros.h"
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
