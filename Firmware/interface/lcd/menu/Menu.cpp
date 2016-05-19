#include "Menu.h"
#ifdef MENU_H_
#include "../strings/Strings.h"
#include "../../pads/Pads.h"

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

    uint8_t size;

    switch(type)    {

        case serviceMenu:
        activeMenu = serviceMenu;
        menuHierarchyPosition = 1;
        strcpy_P(stringBuffer, menuType_service);
        size = progmemCharArraySize(menuType_service);
        updateDisplay(0, text, 0, true, size);
        getMenuItems();
        updateMenuScreen();
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

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

        printf(stringBuffer);
        printf("\n");

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

    //first, we should check if current item has defined function pointer
    //if it does, run function, but only on confirm (else we go back)
    //else, go to next menu level

    uint8_t currentOptionIndex = (menuHierarchyPosition % 10) - 1;

    if (menuItem[indexes[currentOptionIndex]].function != NULL)   {

        if (confirm)    {

            if (!functionRunning) {

                functionRunning = true;
                menuItem[indexes[currentOptionIndex]].function();

            }   return;

        } else {

            if (functionRunning) {

                functionRunning = false;
                updateMenuScreen();
                return;

            }

        }

    }

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument
    uint16_t newLevel = menuHierarchyPosition;

    confirm ? newLevel = (newLevel*10) + 1 : newLevel /= 10;

    if (newLevel < 1) newLevel = 1;

    bool menuLevelPresent = confirm ? false : true;

    if (confirm && !menuLevelPresent)    {

        //level needs to be increased
        //we should first check it that level exists
        for (int i=0; i<MENU_ITEMS; i++)
        if (menuItem[i].level == newLevel)  {

            menuLevelPresent = true;
            break;

        }

    }

    if (!confirm && newLevel == 1 && menuHierarchyPosition == 1)    {   //exit menu

        display.displayProgramAndPreset(pads.getActiveProgram()+1, pads.getActivePreset());
        activeMenu = noMenu;
        return;

    }

    if (menuLevelPresent && (newLevel != menuHierarchyPosition))   {

        menuHierarchyPosition = newLevel;
        getMenuItems();
        updateMenuScreen();

    }

}

Menu menu;
#endif