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

    createLayout();

}

void Menu::displayMenu(menuType_t type) {

    activeMenu = type;
    //always reset menu level when displaying menu
    menuHierarchyPosition = 1;

    setMenuTitle(true);
    getMenuItems();
    updateMenuScreen();

}

void Menu::exitMenu()   {

    activeMenu = noMenu;

}

bool Menu::menuDisplayed()  {

    return (activeMenu != noMenu);

}

void Menu::getMenuItems()   {

    #if MODE_SERIAL > 0
        printf("Getting menu items\n");
    #endif

    //reset current items
    items = 0;

    uint8_t currentDigits = display.getNumberOfDigits(menuHierarchyPosition);
    uint8_t currentOption = menuHierarchyPosition % 10;
    uint16_t subtract = (menuHierarchyPosition - currentOption) * (currentDigits > 1);

    //we need to find out how many items are in current hierarchy level and their indexes

    for (int i=0; i<MENU_ITEMS; i++) {

        if (display.getNumberOfDigits(menuItem[i].level) == currentDigits)  {

            int16_t result = menuItem[i].level - subtract;

            if ((display.getNumberOfDigits(result) == 1) && (result > 0)) {

                #if MODE_SERIAL > 0
                printf("Indexes array[%d]: %d\n", items, i);
                #endif

                indexes[items] = i;
                items++;

            }

        }

    }

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

        #if MODE_SERIAL > 0
        printf("Selected option: %d\n", menuHierarchyPosition%10);
        printf("Options:\n");
        for (int i=0; i<items; i++) {

            strcpy_P(stringBuffer, menuItem[indexes[i]].stringPointer);
            printf(stringBuffer);
            printf("\n");

        }
        #endif

        updateMenuScreen();

    }

}

void Menu::updateMenuScreen()   {

    uint8_t size;

    uint8_t currentOption = menuHierarchyPosition % 10;

    #if MODE_SERIAL > 0
        printf("Updating menu screen\n\n");
    #endif

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

void Menu::confirmOption(bool confirm)  {

    if (confirm && functionRunning) return;

    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument
    uint16_t newLevel = menuHierarchyPosition;

    confirm ? newLevel = (newLevel*10) + 1 : newLevel /= 10;

    if (newLevel < 1)   {   //exit menu

        #if MODE_SERIAL > 0
        printf("Exiting menu\n");
        #endif
        //exit menu and restore initial state
        display.displayProgramAndPreset(pads.getActiveProgram()+1, pads.getActivePreset());
        //disable calibration if active
        pads.setCalibrationMode(false);
        //re-enable buttons
        buttons.enable();
        activeMenu = noMenu;
        return;

    }

    if (confirm)    {

        bool menuLevelPresent = false;

        //check if level has assigned function
        if (menuItem[indexes[currentOptionIndex]].function != NULL) {

            if (!functionRunning) {

                setMenuTitle(false);
                functionRunning = true;
                menuItem[indexes[currentOptionIndex]].function();

            }

            //update level regardless
            menuHierarchyPosition = newLevel;
            return;

        }

        //level needs to be increased
        //we should first check it that level exists
        for (int i=0; i<MENU_ITEMS; i++)
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

            //check for function
            if (menuItem[indexes[currentOptionIndex]].function != NULL) {

                if (functionRunning) {

                    functionRunning = false;

                }

            }

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

void Menu::setMenuTitle(bool rootTitle)   {

    uint8_t size = 0;
    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    //set menu title, but only if current level isn't 1 (root)
    if (!rootTitle) {

        strcpy_P(stringBuffer, menuItem[indexes[currentOptionIndex]].stringPointer);
        size = strlen_P(menuItem[indexes[currentOptionIndex]].stringPointer);
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

            default:
            break;

        }

    }

}

Menu menu;
#endif