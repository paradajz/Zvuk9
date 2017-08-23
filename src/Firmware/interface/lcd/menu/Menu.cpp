#include "Menu.h"
#include "../strings/Strings.h"
#include "../../pads/Pads.h"
#include "../../buttons/Buttons.h"

Menu::Menu()
{
    activeMenu = noMenu;
}

void Menu::show(menuType_t type)
{
    activeMenu = type;

    switch(type)
    {
        case serviceMenu:
        createServiceMenuLayout();
        break;

        case userMenu:
        createUserMenuLayout();
        break;

        default:
        break;
    }

    //always reset menu level when displaying menu
    menuHierarchyPosition = 1;
    activeOption = 0;
    functionRunning = false;

    //clear entire display
    display.clearAll();

    setMenuTitle(true);
    menuSize = getMenuSize();
    getMenuItems();
    updateMenuScreen();
}

void Menu::setMenuTitle(bool rootTitle)
{
    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //make sure to clear the first line before setting new title
    display.clearRow(0);

    //set menu title, but only if current level isn't 1 (root)
    if (!rootTitle)
    {
        stringBuffer[0] = '<';
        stringBuffer[1] = '\0';
        strcpy_P(tempBuffer, menuItem[indexes[currentOptionIndex]].stringPointer);
        strcat(stringBuffer, tempBuffer);
        display.updateText(0, lcdtext_still, 0);
    }
    else
    {
        switch(activeMenu)
        {
            case userMenu:
            strcpy_P(stringBuffer, menuType_user_string);
            display.updateText(0, lcdtext_still, 0);
            break;

            case serviceMenu:
            strcpy_P(stringBuffer, menuType_service_string);
            display.updateText(0, lcdtext_still, 0);
            break;

            default:
            break;
        }
    }
}

uint8_t Menu::getMenuSize()
{
    uint8_t menuSize = 0;

    switch(activeMenu)
    {
        case serviceMenu:
        menuSize = SERVICE_MENU_ITEMS;
        break;

        case userMenu:
        menuSize = USER_MENU_ITEMS;
        break;

        default:
        break;
    }

    return menuSize;
}

void Menu::getMenuItems()
{
    //reset current items
    items = 0;

    uint8_t currentDigits = getNumberOfDigits(menuHierarchyPosition);
    uint8_t currentOption = menuHierarchyPosition % 10;
    uint16_t subtract = (menuHierarchyPosition - currentOption) * (currentDigits > 1);

    for (int i=0; i<menuSize; i++)
    {
        if (getNumberOfDigits(menuItem[i].level) == currentDigits)
        {
            int16_t result = menuItem[i].level - subtract;

            if ((getNumberOfDigits(result) == 1) && (result > 0))
            {
                indexes[items] = i;
                items++;
            }
        }
    }
}

void Menu::updateMenuScreen()
{
    uint8_t size;

    uint8_t currentOption = menuHierarchyPosition % 10;

    //we can display up to three options/suboptions at the time
    //newSelectedOption needs to be subtracted by 1 since indexing uses 1 as starting point
    uint8_t markerOption = ((currentOption-1) > (LCD_HEIGHT-2)) ? (LCD_HEIGHT-2) : (currentOption-1);
    //position from which we start retrieving menu items
    uint8_t startPosition = ((currentOption-1) > (LCD_HEIGHT-2)) ? currentOption-1-(LCD_HEIGHT-2) : 0;
    uint8_t itemsIterate = items > (LCD_HEIGHT-1) ? (LCD_HEIGHT-1) : items;

    for (int i=0; i<itemsIterate; i++)
    {
        //clear line first
        display.clearRow(i+1);

        //skipping first row since it's reserved for the menu title
        if (i == markerOption)
            stringBuffer[0] = '>';
        else
            stringBuffer[0] = ' ';

        size = 1;

        stringBuffer[1] = '\0';
        strcpy_P(tempBuffer, menuItem[indexes[i+startPosition]].stringPointer);
        size += strlen(tempBuffer);
        strcat(stringBuffer, tempBuffer);

        //check for checkable items
        if (menuItem[indexes[i+startPosition]].checkable && menuItem[indexes[i+startPosition]].function != NULL)
        {
            //checked and unchecked strings have the same size
            uint8_t checkMarkerSize = ARRAY_SIZE_CHAR(checked_string);
            uint8_t spaceFill = LCD_WIDTH - size - checkMarkerSize;
            addSpaceToCharArray(spaceFill, size);
            size += checkMarkerSize;

            //place checked/unchecked characters at the end of the screen line
            (menuItem[indexes[i+startPosition]].function(menuItem[indexes[i+startPosition]].argument)) ? strcpy_P(tempBuffer, checked_string) : strcpy_P(tempBuffer, unchecked_string);
            strcat(stringBuffer, tempBuffer);
        }

        display.updateText(i+1, lcdtext_still, 0);
    }
}

void Menu::changeOption(bool direction)
{
    if (functionRunning)
        return;
    uint8_t currentOption = menuHierarchyPosition % 10;

    //here we actually change selected option
    uint8_t newSelectedOption = currentOption;
    direction ? newSelectedOption++ : newSelectedOption--;

    //make sure we don't cross bounds
    if (newSelectedOption < 1)
        newSelectedOption = 1;

    if (newSelectedOption > items)
        newSelectedOption--;

    if (newSelectedOption != currentOption)
    {
        //we need to update global hierarchy position
        //to do that, first we get rid of current position and then add new one
        menuHierarchyPosition = (menuHierarchyPosition-currentOption)+newSelectedOption;

        updateMenuScreen();
    }
}

void Menu::exitMenu()
{
    #ifdef DEBUG
    printf_P(PSTR("Exiting menu\n"));
    #endif

    //disable calibration if active
    pads.setCalibrationMode(false);
    //exit menu and restore initial state
    display.setupHomeScreen();
    //re-enable buttons
    buttons.enable();
    activeMenu = noMenu;
    functionRunning = false;
}

bool Menu::isMenuDisplayed()
{
    return (activeMenu != noMenu);
}

void Menu::confirmOption(bool confirm)
{
    if (confirm && functionRunning)
        return;

    if (!confirm && functionRunning)
        functionRunning = false;

    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument
    uint16_t newLevel = menuHierarchyPosition;

    confirm ? newLevel = (newLevel*10) + 1 : newLevel /= 10;

    if (newLevel < 1)
    {
        //exit menu
        exitMenu();
        return;
    }

    //clear all lines except for the first one
    for (int i=1; i<LCD_HEIGHT; i++)
        display.clearRow(i);

    if (confirm)
    {
        bool menuLevelPresent = false;

        //check if level has assigned function
        if (menuItem[indexes[currentOptionIndex]].function != NULL)
        {
            if (menuItem[indexes[currentOptionIndex]].checkable)
            {
                //set second argument to "true" value to switch to new option
                menuItem[indexes[currentOptionIndex]].argument.argument2 = true;
                //menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument);
                if (menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument))
                {
                    //do nothing
                }
                else
                {
                    //function returned false
                    return;
                }

                //reset switch argument
                menuItem[indexes[currentOptionIndex]].argument.argument2 = false;

                //now refresh screen with changed arguments
                updateMenuScreen();
            }
            else
            {
                //regular function
                bool functionStatus = true;

                if (!functionRunning)
                {
                    functionStatus = menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument);

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
            if (menuItem[i].level == newLevel)
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
            getMenuItems();
        }
        else
        {
            //we need to get one level behind new level to find out menu title when going backwards
            menuHierarchyPosition = newLevel/10;
            getMenuItems();

            if (newLevel > 10)
                setMenuTitle(false);
            else
                setMenuTitle(true);

            menuHierarchyPosition = newLevel;
            getMenuItems();
        }

        //fill menu with items
        updateMenuScreen();
    }
}

void Menu::stopFunction()
{
    functionRunning = false;
}

Menu menu;
