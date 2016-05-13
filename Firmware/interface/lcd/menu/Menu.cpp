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

    //#if MODE_SERIAL > 0
        //printf("Subtract: %d\n\n", subtract);
    //#endif

    //first, we need to find out how many items are in current hierarchy level and their indexes
    uint8_t items = 0;
    uint8_t indexes[MAX_MENU_OPTIONS];

    for (int i=0; i<MENU_ITEMS; i++) {

        if (display.getNumberOfDigits(menuItem[i].level) == currentDigits)  {

             //we need to pick specific indexes here
             //#if MODE_SERIAL > 0
                //printf("Candidate: %d\n", menuItem[i].level);
            //#endif

            int16_t result = menuItem[i].level - subtract;

            //#if MODE_SERIAL > 0
                //printf("Result: %d\n", result);
            //#endif

                if ((display.getNumberOfDigits(result) == 1) && (result > 0)) {

                    //#if MODE_SERIAL > 0
                        //printf("Selected: %d\n", menuItem[i].level);
                    //#endif

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
    uint8_t markerOption = (newSelectedOption > (NUMBER_OF_LCD_ROWS-2)) ? (NUMBER_OF_LCD_ROWS-2) : newSelectedOption;
    //position from which we start retrieving menu items
    uint8_t startPosition = (newSelectedOption > (NUMBER_OF_LCD_ROWS-2)) ? newSelectedOption-(NUMBER_OF_LCD_ROWS-2) : 0;

    switch(activeMenu)    {

        case serviceMenu:
        for (int i=0; i<(NUMBER_OF_LCD_ROWS-1); i++)    {

            if (i == markerOption)  stringBuffer[0] = '>';
            else                    stringBuffer[0] = SPACE_CHAR;

            stringBuffer[1] = '\0';
            //strcpy_P(display.tempBuffer, (char*)pgm_read_word(&(service_menu_options[i+startPosition])));
            strcpy_P(tempBuffer, menuItem[indexes[i]].stringPointer);
            strcat(stringBuffer, tempBuffer);
            size = 1 + pgm_read_byte(&service_menu_options_sizes[i+startPosition]);
            updateDisplay(i+1, text, 0, true, size);

        }
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

    //display.changeMenuOption(activeMenu, direction);

}

void Menu::confirmOption(bool confirm)  {

    //this confirms current hierarchy level and moves to next one,
    //or it deletes current level and switches to previous, depending on received argument

    //switch(confirm) {
//
        //case true:
        //switch(activeMenu)  {
//
            //case serviceMenu:
            //if (menuHierarchyIndex < (MAX_MENU_LEVELS-1))   {
//
                //activeOption = 0;
                //menuHierarchyIndex++;
//
            //}
            //break;
//
        //}
        //break;
//
        //case false:
        //if (menuHierarchyIndex > 0) {
//
            //menuHierarchyPosition[menuHierarchyIndex] = -1;
            //menuHierarchyIndex--;
            //activeOption = 0;
//
        //}
        //break;
//
    //}
//
    //menuHierarchyPosition[menuHierarchyIndex] = 0;
//
    //#if MODE_SERIAL > 0
        //printf("Current position in menu:\n");
        //for (int i=0; i<MAX_MENU_LEVELS; i++)    {
//
            //printf("%d.", menuHierarchyPosition[i]);
//
        //} printf("\n");
//
        //printf("Menu hierarchy index: %d\n", menuHierarchyIndex);
    //#endif

    //display.changeMenuScreen(activeMenu, menuHierarchyPosition);

}

Menu menu;
#endif