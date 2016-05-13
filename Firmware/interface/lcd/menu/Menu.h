#include "../LCD.h"

#ifdef LCD_H_

#ifndef MENU_H_
#define MENU_H_

#include "../../../Types.h"
#include "../Macros.h"
#include "../strings/Strings.h"
#include "Items.h"

#define MAX_MENU_OPTIONS    5

class Menu : LCD {

    public:
    Menu();
    void init();
    void displayMenu(menuType_t type);
    bool menuDisplayed();
    void changeOption(bool direction);
    void confirmOption(bool confirm);
    void createLayout();

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeLevel;
    uint16_t menuHierarchyPosition;

    typedef void (*actionPointer)(void);

    typedef struct {

        //denotes item level in menu hierarchy, ie. 1.2.3.1.
        uint16_t level;
        //pointer to function if any
        actionPointer function;
        //pointer to string to show item name
        const char *stringPointer;

    } menuItem_t;

    menuItem_t menuItem[MENU_ITEMS];

};

extern Menu menu;

#endif
#endif