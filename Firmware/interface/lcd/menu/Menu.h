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
    void confirmOption(bool confirm);
    void createLayout();

    private:
    menuType_t activeMenu;
    int8_t activeOption;
    int8_t activeLevel;
    int8_t menuHierarchyPosition[MAX_MENU_LEVELS];
    uint8_t menuHierarchyIndex;

    typedef void (*actionPointer)(void);

    typedef struct {

        //denotes item level in menu hierarchy, ie. 1.2.3.1.
        uint8_t level[MAX_MENU_LEVELS];
        //pointer to function if any
        actionPointer function;
        //pointer to string to show item name
        char *stringPointer;

    } menuItem_t;

    typedef struct {

        menuItem_t calibration;
        menuItem_t deviceInfo;
        menuItem_t factoryReset;

        menuItem_t calibrateX;
        menuItem_t calibrateY;
        menuItem_t calibratePressure;

        menuItem_t calibrateXlower;
        menuItem_t calibrateXupper;

        menuItem_t calibrateYlower;
        menuItem_t calibrateYupper;

        menuItem_t calibratePressureLower;
        menuItem_t calibratePressureUpper;

        menuItem_t xCalibrationManual;
        menuItem_t xCalibrationInteractive;

        menuItem_t yCalibrationManual;
        menuItem_t yCalibrationInteractive;

        menuItem_t pressureCalibrationManual;
        menuItem_t pressureCalibrationInteractive;

        menuItem_t softwareInfo;
        menuItem_t hardwareInfo;

        menuItem_t partial;
        menuItem_t full;

    } menuItems_t;

    menuItems_t menu;


};

extern Menu menu;

#endif
#endif