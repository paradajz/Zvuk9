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

    //we need to find out how many items are in current hierarchy level
    uint8_t items = 0;
    uint8_t indexes[10];
    char textBuffer[50];

    uint8_t currentDigits = display.getNumberOfDigits(menuHierarchyPosition);
    uint8_t menuMarker = menuHierarchyPosition % 10;
    uint8_t subtract = (menuHierarchyPosition - menuMarker) * (currentDigits > 1);

    #if MODE_SERIAL > 0
        printf("Subtract: %d\n\n", subtract);
    #endif

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

                    #if MODE_SERIAL > 0
                        printf("Selected: %d\n", menuItem[i].level);
                    #endif

                    indexes[items] = i;
                    items++;

                }

        }

    }

    uint8_t newMarkerPosition = menuMarker;
    direction ? newMarkerPosition++ : newMarkerPosition--;

    if (newMarkerPosition < 1) newMarkerPosition = 1;
    if (newMarkerPosition > items) newMarkerPosition--;

    menuHierarchyPosition = (menuHierarchyPosition-menuMarker)+newMarkerPosition;

    #if MODE_SERIAL > 0
        printf("Menu marker position: %d\n", menuHierarchyPosition%10);
        printf("Options:\n");
        for (int i=0; i<items; i++) {

            strcpy_P(textBuffer, menuItem[indexes[i]].stringPointer);
            printf(textBuffer);
            printf("\n");

        }
    #endif

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