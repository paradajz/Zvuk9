#include "Menu.h"
#ifdef MENU_H_
#include "../strings/Strings.h"
#include "../../pads/Pads.h"
#include "../../buttons/Buttons.h"

Menu::Menu()    {

    //default constructor
    activeMenu = noMenu;
    activeOption = 0;
    menuHierarchyPosition = 0;
    functionRunning = false;

}

void Menu::init()   {


}

void Menu::displayMenu(menuType_t type) {

    activeMenu = type;

    switch(type)    {

        case serviceMenu:
        createServiceMenuLayout();
        break;

        case userMenu:
        createUserMenuLayout();
        break;

        case quickMenu:
        createQuickMenuLayout();
        break;

        default:
        break;

    }

    //always reset menu level when displaying menu
    menuHierarchyPosition = 1;
    activeOption = 0;
    functionRunning = false;

    setMenuTitle(true);
    menuSize = getMenuSize();
    getMenuItems();
    updateMenuScreen();

}

void Menu::setMenuTitle(bool rootTitle)   {

    uint8_t size = 0;
    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //set menu title, but only if current level isn't 1 (root)
    if (!rootTitle) {

        stringBuffer[0] = '<';
        stringBuffer[1] = '\0';
        strcpy_P(tempBuffer, menuItem[indexes[currentOptionIndex]].stringPointer);
        strcat(stringBuffer, tempBuffer);
        size = strlen_P(menuItem[indexes[currentOptionIndex]].stringPointer) + 1;
        updateDisplay(0, text, 0, true, size);

    }   else {

        switch(activeMenu)  {

            case userMenu:
            strcpy_P(stringBuffer, menuType_user);
            size = progmemCharArraySize(menuType_user);
            updateDisplay(0, text, 0, true, size);
            break;
            break;

            case serviceMenu:
            strcpy_P(stringBuffer, menuType_service);
            size = progmemCharArraySize(menuType_service);
            updateDisplay(0, text, 0, true, size);
            break;

            case quickMenu:
            strcpy_P(stringBuffer, menuType_quick);
            size = progmemCharArraySize(menuType_quick);
            updateDisplay(0, text, 0, true, size);
            break;

            default:
            break;

        }

    }

}

uint8_t Menu::getMenuSize() {

    uint8_t menuSize = 0;

    switch(activeMenu)  {

        case serviceMenu:
        menuSize = SERVICE_MENU_ITEMS;
        break;

        case userMenu:
        menuSize = USER_MENU_ITEMS;
        break;

        case quickMenu:
        if (pads.isUserScale(pads.getActivePreset()))
        menuSize = QUICK_MENU_ITEMS_USER_SCALE;
        else menuSize = QUICK_MENU_ITEMS_PREDEFINED_SCALE;
        break;

        default:
        break;

    }   return menuSize;

}

void Menu::getMenuItems()   {

    //reset current items
    items = 0;

    uint8_t currentDigits = display.getNumberOfDigits(menuHierarchyPosition);
    uint8_t currentOption = menuHierarchyPosition % 10;
    uint16_t subtract = (menuHierarchyPosition - currentOption) * (currentDigits > 1);

    for (int i=0; i<menuSize; i++) {

        if (display.getNumberOfDigits(menuItem[i].level) == currentDigits)  {

            int16_t result = menuItem[i].level - subtract;

            if ((display.getNumberOfDigits(result) == 1) && (result > 0)) {

                indexes[items] = i;
                items++;

            }

        }

    }

}

void Menu::updateMenuScreen()   {

    uint8_t size;

    uint8_t currentOption = menuHierarchyPosition % 10;

    //we can display up to three options/suboptions at the time
    //newSelectedOption needs to be subtracted by 1 since indexing uses 1 as starting point
    uint8_t markerOption = ((currentOption-1) > (NUMBER_OF_LCD_ROWS-2)) ? (NUMBER_OF_LCD_ROWS-2) : (currentOption-1);
    //position from which we start retrieving menu items
    uint8_t startPosition = ((currentOption-1) > (NUMBER_OF_LCD_ROWS-2)) ? currentOption-1-(NUMBER_OF_LCD_ROWS-2) : 0;
    uint8_t itemsIterate = items > (NUMBER_OF_LCD_ROWS-1) ? (NUMBER_OF_LCD_ROWS-1) : items;

    for (int i=0; i<itemsIterate; i++)    {

        //skipping first row since it's reserved for the menu title

        if (i == markerOption)  stringBuffer[0] = '>';
        else                    stringBuffer[0] = SPACE_CHAR;

        stringBuffer[1] = '\0';
        strcpy_P(tempBuffer, menuItem[indexes[i+startPosition]].stringPointer);
        strcat(stringBuffer, tempBuffer);
        size = 1 + strlen_P(menuItem[indexes[i+startPosition]].stringPointer);

        //check for checkable items
        if (menuItem[indexes[i+startPosition]].checkable)   {

            //checked and unchecked strings have the same size
            uint8_t checkMarkerSize = progmemCharArraySize(checked_string);
            uint8_t spaceFill = NUMBER_OF_LCD_COLUMNS - size - checkMarkerSize;
            addSpaceToCharArray(size, spaceFill);
            size += checkMarkerSize;

            //place checked/unchecked characters at the end of the screen line
            (menuItem[indexes[i+startPosition]].function(menuItem[indexes[i+startPosition]].argument)) ? strcpy_P(tempBuffer, checked_string) : strcpy_P(tempBuffer, unchecked_string);
            strcat(stringBuffer, tempBuffer);

        }

        updateDisplay(i+1, text, 0, true, size);

    }

    if (items < (NUMBER_OF_LCD_ROWS-1)) {

        //clear rows if needed

        strcpy_P(stringBuffer, emptyLine_string);
        size = progmemCharArraySize(emptyLine_string);

        for (int i=items+1; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true, size);

    }

    #if MODE_SERIAL > 0
    printf("\nmenuHierarchyPosition: %d\n", menuHierarchyPosition);
    #endif

}

void Menu::changeOption(bool direction) {

    if (functionRunning) return;

    uint8_t currentOption = menuHierarchyPosition % 10;

    //here we actually change selected option
    uint8_t newSelectedOption = currentOption;
    direction ? newSelectedOption++ : newSelectedOption--;

    //make sure we don't cross bounds
    if (newSelectedOption < 1) newSelectedOption = 1;
    if (newSelectedOption > items) newSelectedOption--;

    if (newSelectedOption != currentOption) {

        //we need to update global hierarchy position
        //to do that, first we get rid of current position and then add new one
        menuHierarchyPosition = (menuHierarchyPosition-currentOption)+newSelectedOption;

        updateMenuScreen();

    }

}

void Menu::exitMenu()   {

    #if MODE_SERIAL > 0
        printf("Exiting menu\n");
    #endif
    //exit menu and restore initial state
    display.displayProgramAndPreset(pads.getActiveProgram()+1, pads.getActivePreset());
    //disable calibration if active
    pads.setCalibrationMode(false);
    //re-enable buttons
    #ifdef MODULE_BUTTONS
    buttons.enable();
    #endif
    activeMenu = noMenu;
    functionRunning = false;

}

bool Menu::menuDisplayed()  {

    return (activeMenu != noMenu);

}

void Menu::confirmOption(bool confirm)  {

    if (confirm && functionRunning) return;
    if (!confirm && functionRunning) functionRunning = false;

    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument
    uint16_t newLevel = menuHierarchyPosition;

    confirm ? newLevel = (newLevel*10) + 1 : newLevel /= 10;

    if (newLevel < 1)   {   //exit menu

        exitMenu();
        return;

    }

    if (confirm)    {

        bool menuLevelPresent = false;

        //special check for jumping to user menu from quick actions
        if (activeMenu == quickMenu)    {

            if (
            (indexes[currentOptionIndex] == quickMenuItem_us_userMenu) ||
            (indexes[currentOptionIndex] == quickMenuItem_ps_userMenu)) {

                displayMenu(userMenu);
                return;

            }

        }

        //check if level has assigned function
        if (menuItem[indexes[currentOptionIndex]].function != NULL) {

            if (menuItem[indexes[currentOptionIndex]].conditionCheck)   {

                //run function without setting any flag or menu title
                if (menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument))   {

                    //do nothing

                } else return; //function returned false

            } else if (menuItem[indexes[currentOptionIndex]].checkable) {

                //set second argument to "true" value to switch to new option
                menuItem[indexes[currentOptionIndex]].argument.argument2 = true;
                menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument);
                //reset switch argument
                menuItem[indexes[currentOptionIndex]].argument.argument2 = false;
                //now refresh screen with changed arguments
                updateMenuScreen();

            }   else {

                if (!functionRunning) {

                    setMenuTitle(false);
                    functionRunning = true;
                    menuItem[indexes[currentOptionIndex]].function(menuItem[indexes[currentOptionIndex]].argument);

                }

                //update level regardless
                menuHierarchyPosition = newLevel;
                return;

            }

        }

        //level needs to be increased
        //we should first check it that level exists
        for (int i=0; i<menuSize; i++)
        if (menuItem[i].level == newLevel)  {

            menuLevelPresent = true;
            break;

        }   if (!menuLevelPresent) return; //no level

    }

    if (newLevel != menuHierarchyPosition)   {

        //if we got here, we should update menu

        if (confirm)    {

            if (newLevel > 10) setMenuTitle(false);
            else setMenuTitle(true);

            menuHierarchyPosition = newLevel;
            getMenuItems();

        }   else {

            //we need to get one level behind new level to find out menu title when going backwards
            menuHierarchyPosition = newLevel/10;

            getMenuItems();

            if (newLevel > 10) setMenuTitle(false);
            else setMenuTitle(true);

            menuHierarchyPosition = newLevel;
            getMenuItems();

        }

        //fill menu with items
        updateMenuScreen();

    }

}

void Menu::stopFunction()   {

    functionRunning = false;

}

Menu menu;
#endif