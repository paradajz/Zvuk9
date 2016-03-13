#ifndef MENU_H_
#define MENU_H_

#include "../LCD.h"
#include "../../../Types.h"

class Menu  {

    public:
    Menu();
    void displayMenu(menuType_t type);
    bool menuDisplayed();

    private:
    menuType_t activeMenu;

};

extern Menu menu;

#endif /* MENU_H_ */