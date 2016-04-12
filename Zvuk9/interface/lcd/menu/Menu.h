#include "../LCD.h"

#ifdef LCD_H_

#ifndef MENU_H_
#define MENU_H_

#include "../../../Types.h"
#include "../Macros.h"
#include "../strings/Strings.h"

#define MAX_MENU_LEVELS     10
#define MAX_MENU_OPTIONS    10

class Menu  {

    public:
    Menu();
    void displayMenu(menuType_t type);
    bool menuDisplayed();
    void changeOption(bool direction);
    void selectOption();
    void createLayout();

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeSubOption;
    typedef struct {

        uint8_t options;
        uint8_t levels;

    } menuLayout;

    menuLayout menuOption[MAX_MENU_OPTIONS];

};

extern Menu menu;

#endif
#endif