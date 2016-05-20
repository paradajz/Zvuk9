#ifndef OPTIONS_H_
#define OPTIONS_H_

typedef enum {

    menuItem_calibration,
    menuItem_deviceInfo,
    menuItem_factoryReset,

    menuItem_calibrateX,
    menuItem_calibrateY,
    menuItem_calibratePressure,

    menuItem_calibratePressureLower,
    menuItem_calibratePressureUpper,

    menuItem_calibratePressureLowerInteractive,
    menuItem_calibratePressureLowerManual,

    menuItem_calibratePressureUpperInteractive,
    menuItem_calibratePressureUpperManual,

    menuItem_softwareInfo,
    menuItem_hardwareInfo,

    menuItem_partialReset,
    menuItem_fullReset,

    menuItem_confirmPartialReset,
    menuItem_confirmFullReset,

    MENU_ITEMS

} menuItems_t;

#endif