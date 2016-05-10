#include "../LCD.h"

#ifdef LCD_H_

#ifndef MENU_H_
#define MENU_H_

#include "../../../Types.h"
#include "../Macros.h"
#include "../strings/Strings.h"

#define MAX_MENU_LEVELS     5
#define MAX_MENU_OPTIONS    5

class Menu  {

    public:
    Menu();
    void displayMenu(menuType_t type);
    bool menuDisplayed();
    void changeOption(bool direction);
    void createLayout();

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeLevel;
    int8_t activeSubOption;
    int8_t menuHierarchyPosition[10];

    typedef void (*actionPointer)(void);

    typedef struct {

        //pointer to function
        actionPointer function;

    } menuOption;

    typedef struct {

        uint8_t options;
        menuOption option[MAX_MENU_OPTIONS];
        //pointer to function
        actionPointer function[MAX_MENU_OPTIONS];
        //pointer to progmem char array


    } menuScreen_t;

    typedef struct {

        uint8_t options;
        actionPointer function[MAX_MENU_OPTIONS];

    } rootScreen_t;

    menuScreen_t menuScreen[MAX_MENU_OPTIONS];
    rootScreen_t rootScreen;

};

extern Menu menu;

#endif
#endif