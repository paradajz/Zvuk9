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

#include "Menu.h"
#include "DataTypes.h"
#include "../strings/Strings.h"
#include "layout/Layout.h"

bool        itemFuncChangeVal;

///
/// \ingroup displayMenu
/// @{

///
/// \brief Currently active menu.
///
menuType_t  activeMenu;

///
/// \brief Currently active position in menu.
///
int16_t     menuHierarchyPosition;

///
/// \brief Total number of items in current menu layout.
///
uint8_t     menuSize;

///
/// \brief Flag indicating whether or not menu item function is running.
///
bool        functionRunning;

///
/// \brief Total number of items in current menu hierarchy.
///
uint8_t     menuHierarchyItems;

///
/// \brief Array holding indexes of items in current menu hierarchy.
/// Due to simple menu design, maximum number of indexes in hierarchy is 9.
///
uint8_t     itemIndex[9];

///
/// \brief Pointer to menu layout.
///
menuItem_t  *menuLayout;

/// @}

///
/// \brief Default constructor.
///
Menu::Menu()
{
    activeMenu = noMenu;
}

///
/// \brief Shows initial screen for specified menu type.
/// @param [in] type    Menu type (user or service). If type is set to noMenu,
///                     menu is cleared.
/// \returns True if menu setting was successful, false otherwise.
///
bool Menu::setMenuType(menuType_t type)
{
    switch(type)
    {
        case serviceMenu:
        menuLayout = serviceMenuLayout;
        menuSize = SERVICE_MENU_ITEMS;
        break;

        case userMenu:
        menuLayout = userMenuLayout;
        menuSize = USER_MENU_ITEMS;
        break;

        case noMenu:
        //nothing
        break;

        default:
        return false;
    }

    activeMenu = type;

    //always reset menu level when displaying menu
    menuHierarchyPosition = 1;
    functionRunning = false;

    if (type != noMenu)
    {
        setMenuTitle(true);
        updateMenuHierarchyItems();
        updateMenuScreen();
    }

    return true;
}

///
/// \brief Sets menu title.
/// @param [in] rootTitle   If set to true, menu name will be shown.
///                         Otherwise, string of first item in current menu hierarchy is shown.
///
void Menu::setMenuTitle(bool rootTitle)
{
    //make sure to clear the first line before setting new title
    display.clearRow(0);

    //set menu title, but only if current level isn't 1 (root)
    if (!rootTitle)
    {
        //go back one level to get menu title
        uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

        stringBuffer.startLine();
        stringBuffer.appendChar('<', 1);
        stringBuffer.appendText_P(menuLayout[itemIndex[currentOptionIndex]].stringPointer);
        stringBuffer.endLine();
        display.updateText(0, displayText_still, 0);
    }
    else
    {
        switch(activeMenu)
        {
            case userMenu:
            stringBuffer.startLine();
            stringBuffer.appendText_P(menuType_user_string);
            stringBuffer.endLine();

            display.updateText(0, displayText_still, 0);
            break;

            case serviceMenu:
            stringBuffer.startLine();
            stringBuffer.appendText_P(menuType_service_string);
            stringBuffer.endLine();

            display.updateText(0, displayText_still, 0);
            break;

            default:
            break;
        }
    }
}

///
/// \brief Retrieve number of items in current menu hierarchy and store their indexes.
///
void Menu::updateMenuHierarchyItems()
{
    //reset current items
    menuHierarchyItems = 0;

    //get number of digits for current menu hierarchy level
    uint8_t currentDigits = getNumberOfDigits(menuHierarchyPosition);
    //same hierarchy members are found by subtracting root index of hierarchy from level being checked
    //ie. if current level is 211 subtract 210 from all indexes and check the positive results with 1 digit
    int16_t subtract = menuHierarchyPosition - (menuHierarchyPosition % 10);
    uint8_t itemCount = 0;

    for (int i=0; i<menuSize; i++)
    {
        //check only items which match current amount of digits
        if (getNumberOfDigits(menuLayout[i].level) == currentDigits)
        {
            if (currentDigits != 1)
            {
                //check only items which match current hierarchy
                if ((getNumberOfDigits(menuLayout[i].level-subtract) == 1) && ((menuLayout[i].level-subtract) > 0))
                {
                    menuHierarchyItems++;
                    itemIndex[itemCount] = i;
                    itemCount++;
                }
            }
            else
            {
                //just count here, root items
                menuHierarchyItems++;
                itemIndex[itemCount] = i;
                itemCount++;
            }
        }
    }
}

///
/// \brief Displays all items for current menu hierarchy on display.
///
void Menu::updateMenuScreen()
{
    uint8_t currentOption = menuHierarchyPosition % 10;
    //display row in which marker should be shown
    uint8_t markerOption = ((currentOption-1) > (DISPLAY_HEIGHT-2)) ? (DISPLAY_HEIGHT-2) : (currentOption-1);
    //position from which items are being shown
    uint8_t startPosition = ((currentOption-1) > (DISPLAY_HEIGHT-2)) ? currentOption-1-(DISPLAY_HEIGHT-2) : 0;
    //limit items printout depending on display height
    uint8_t itemsIterate = menuHierarchyItems > (DISPLAY_HEIGHT-1) ? (DISPLAY_HEIGHT-1) : menuHierarchyItems;

    for (int i=0; i<itemsIterate; i++)
    {
        //skipping first row since it's reserved for the menu title
        display.clearRow(i+1);

        stringBuffer.startLine();

        if (i == markerOption)
            stringBuffer.appendChar('>', 1);
        else
            stringBuffer.appendChar(' ', 1);

        uint8_t index = itemIndex[i+startPosition];

        stringBuffer.appendText_P(menuLayout[index].stringPointer);

        //check for checkable items
        if (menuLayout[index].checkable && menuLayout[index].function != NULL)
        {
            //checked and unchecked strings have the same size
            uint8_t checkMarkerSize = ARRAY_SIZE_CHAR(checked_string);
            uint8_t spaceFill = DISPLAY_WIDTH - stringBuffer.getSize() - checkMarkerSize;
            stringBuffer.appendChar(' ', spaceFill);

            //place checked/unchecked characters at the end of the screen line
            (menuLayout[index].function(menuLayout[index].argument)) ? stringBuffer.appendText_P(checked_string) : stringBuffer.appendText_P(unchecked_string);
        }

        stringBuffer.endLine();

        display.updateText(i+1, displayText_still, 0);
    }
}

///
/// \brief Change menu option.
/// @param [in] direction   Flag indicating in which direction menu option marker should move.
///                         True means positive direction: increase marker index.
///                         False means negative direction: decrease marker index.
///
void Menu::changeOption(bool direction)
{
    //don't allow changing of options if menu item function is running
    if (functionRunning)
        return;

    uint8_t currentOption = menuHierarchyPosition % 10;

    //here we actually change selected option
    uint8_t newSelectedOption = currentOption;
    direction ? newSelectedOption++ : newSelectedOption--;

    //make sure we don't cross bounds
    if (newSelectedOption < 1)
        newSelectedOption = 1;

    if (newSelectedOption > menuHierarchyItems)
        newSelectedOption--;

    if (newSelectedOption != currentOption)
    {
        //we need to update global hierarchy position
        //to do that, first we get rid of current position and then add new one
        menuHierarchyPosition = (menuHierarchyPosition-currentOption)+newSelectedOption;
        updateMenuScreen();
    }
}

///
/// \brief Checks if menu is currently active.
/// \returns True if menu is displayed, false otherwise.
///
bool Menu::isMenuDisplayed()
{
    return (activeMenu != noMenu);
}

///
/// \brief Handles switching of menu screens when options are selected.
/// @param [in] confirm     If set to true, selected option is entered.
///                         Otherwise, menu switches one level back.
///
void Menu::confirmOption(bool confirm)
{
    if (confirm && functionRunning)
        return;

    if (!confirm && functionRunning)
        functionRunning = false;

    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument
    int16_t newLevel = menuHierarchyPosition;

    confirm ? newLevel = (newLevel*10) + 1 : newLevel /= 10;

    if (newLevel < 1)
    {
        //exit menu
        #ifdef DEBUG
        printf_P(PSTR("Exiting menu.\n"));
        #endif
        activeMenu = noMenu;
        return;
    }

    //clear all lines except for the first one
    for (int i=1; i<DISPLAY_HEIGHT; i++)
        display.clearRow(i);

    if (confirm)
    {
        bool menuLevelPresent = false;

        uint8_t index = itemIndex[currentOptionIndex];

        //check if item has assigned function
        if (menuLayout[index].function != NULL)
        {
            //check if item has checkable function which just returns true or false
            if (menuLayout[index].checkable)
            {
                //set itemFuncChangeVal to true to switch to new option
                itemFuncChangeVal = true;
                if (menuLayout[index].function(menuLayout[index].argument))
                {
                    //do nothing
                }
                else
                {
                    //function returned false
                    return;
                }

                //reset itemFuncChangeVal
                itemFuncChangeVal = false;

                //now refresh screen with changed arguments
                updateMenuScreen();
            }
            else
            {
                //regular function
                bool functionStatus = true;

                if (!functionRunning)
                {
                    functionStatus = menuLayout[index].function(menuLayout[index].argument);

                    if (functionStatus)
                    {
                        setMenuTitle(false);
                        functionRunning = true;
                        //update level regardless
                        menuHierarchyPosition = newLevel;
                    }
                    else
                    {
                        //non-regular exit
                        //in this case, increase menu level first so that we can decrease it properly
                        menuHierarchyPosition = newLevel;
                        confirmOption(false);
                    }
                }

                //exit
                return;
            }
        }

        //level needs to be increased
        //we should first check it that level exists
        for (int i=0; i<menuSize; i++)
        {
            if (menuLayout[i].level == newLevel)
            {
                menuLevelPresent = true;
                break;
            }
        }

           if (!menuLevelPresent)
            return; //no level
    }

    if (newLevel != menuHierarchyPosition)
    {
        //if we got here, we should update menu
        if (confirm)
        {
            if (newLevel > 10)
                setMenuTitle(false);
            else
                setMenuTitle(true);

            menuHierarchyPosition = newLevel;
            updateMenuHierarchyItems();
        }
        else
        {
            //we need to get one level behind new level to find out menu title when going backwards
            menuHierarchyPosition = newLevel/10;
            //update indexes so that correct index for menu title is retrieved
            updateMenuHierarchyItems();

            if (newLevel > 10)
                setMenuTitle(false);
            else
                setMenuTitle(true);

            menuHierarchyPosition = newLevel;
            updateMenuHierarchyItems();
        }

        //fill menu with items
        updateMenuScreen();
    }
}

Menu menu;
