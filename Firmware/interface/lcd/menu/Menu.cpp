#include "Menu.h"
#ifdef MENU_H_
#include "../strings/Strings.h"

Menu::Menu()    {

    //default constructor
    activeMenu = noMenu;
    activeOption = 0;
    menuHierarchyPosition = 0;

}

void Menu::init()   {

    createLayout();

}

void Menu::displayMenu(menuType_t type) {

    switch(type)    {

        case serviceMenu:
        display.displayServiceMenu();
        activeMenu = serviceMenu;
        menuHierarchyPosition = 1;
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

    uint8_t currentDigits = display.getNumberOfDigits(menuHierarchyPosition);
    uint8_t currentOption = menuHierarchyPosition % 10;
    uint8_t subtract = (menuHierarchyPosition - currentOption) * (currentDigits > 1);

    //first, we need to find out how many items are in current hierarchy level and their indexes
    uint8_t items = 0;
    uint8_t indexes[MAX_MENU_OPTIONS];

    for (int i=0; i<MENU_ITEMS; i++) {

        if (display.getNumberOfDigits(menuItem[i].level) == currentDigits)  {

            int16_t result = menuItem[i].level - subtract;

                if ((display.getNumberOfDigits(result) == 1) && (result > 0)) {

                    indexes[items] = i;
                    items++;

                }

        }

    }

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

        uint8_t size;

        //we can display up to three options/suboptions at the time
        //newSelectedOption needs to be subtracted by 1 since indexing uses 1 as starting point
        uint8_t markerOption = ((newSelectedOption-1) > (NUMBER_OF_LCD_ROWS-2)) ? (NUMBER_OF_LCD_ROWS-2) : (newSelectedOption-1);
        //position from which we start retrieving menu items
        uint8_t startPosition = ((newSelectedOption-1) > (NUMBER_OF_LCD_ROWS-2)) ? newSelectedOption-1-(NUMBER_OF_LCD_ROWS-2) : 0;

        switch(activeMenu)    {

            case serviceMenu:
            for (int i=0; i<(NUMBER_OF_LCD_ROWS-1); i++)    {

                //skipping first row since it's reserved for the menu title

                if (i == markerOption)  stringBuffer[0] = '>';
                else                    stringBuffer[0] = SPACE_CHAR;

                stringBuffer[1] = '\0';
                strcpy_P(tempBuffer, menuItem[indexes[i+startPosition]].stringPointer);
                strcat(stringBuffer, tempBuffer);
                size = 1 + strlen_P(menuItem[indexes[i+startPosition]].stringPointer);
                updateDisplay(i+1, text, 0, true, size);

            }
            break;

            case userMenu:
            break;

            case noMenu:
            break;

        }

        #if MODE_SERIAL > 0
            printf("menuHierarchyPosition: %d\n", menuHierarchyPosition);
        #endif

    }

}

void Menu::updateMenuScreen()   {

}

void Menu::confirmOption(bool confirm)  {

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

    if (menuLevelPresent && (newLevel != menuHierarchyPosition))   {

        #if MODE_SERIAL > 0
            printf("New level: %d\n", newLevel);
        #endif

        menuHierarchyPosition = newLevel;
        changeOption(true);

    }

}

Menu menu;
#endif