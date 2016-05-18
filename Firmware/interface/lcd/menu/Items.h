#ifndef OPTIONS_H_
#define OPTIONS_H_

typedef enum {

    menuItem_calibration,
    menuItem_deviceInfo,
    menuItem_factoryReset,

    menuItem_calibrateX,
    menuItem_calibrateY,
    menuItem_calibratePressure,

    menuItem_calibrateXlower,
    menuItem_calibrateXupper,

    menuItem_calibrateYlower,
    menuItem_calibrateYupper,

    menuItem_calibratePressureLower,
    menuItem_calibratePressureUpper,

    menuItem_calibrateXlowerInteractive,
    menuItem_calibrateXlowerManual,

    menuItem_calibrateYlowerInteractive,
    menuItem_calibrateYlowerManual,

    menuItem_calibratePressureLowerInteractive,
    menuItem_calibratePressureLowerManual,

    menuItem_calibrateXupperInteractive,
    menuItem_calibrateXupperManual,

    menuItem_calibrateYupperInteractive,
    menuItem_calibrateYupperManual,

    menuItem_calibratePressureUpperInteractive,
    menuItem_calibratePressureUpperManual,

    menuItem_softwareInfo,
    menuItem_hardwareInfo,

    menuItem_partialReset,
    menuItem_fullReset,

    menuItem_confirmPartialReset,
    menuItem_rejectPartialReset,

    menuItem_confirmFullReset,
    menuItem_rejectFullReset,

    MENU_ITEMS

} menuItems_t;

#endif