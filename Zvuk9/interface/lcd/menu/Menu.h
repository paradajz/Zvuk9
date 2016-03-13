#ifndef MENU_H_
#define MENU_H_

#include "../LCD.h"
#include "../../../Types.h"

class Menu  {

    public:
    Menu();
    void displayMenu(menuType_t type);
    bool menuDisplayed();
    void changeOption(bool direction);

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeSubOption;

};

extern Menu menu;

#endif /* MENU_H_ */