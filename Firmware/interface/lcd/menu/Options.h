#ifndef OPTIONS_H_
#define OPTIONS_H_

typedef enum {

    MENU_OPTION_PAD_CALIBRATION,
    MENU_OPTION_DEVICE_INFO,
    MENU_OPTION_FACTORY_RESET

} menuScreen_root;

typedef enum {

    MENU_OPTION_CALIBRATE_X,
    MENU_OPTION_CALIBRATE_Y,
    MENU_OPTION_CALIBRATE_PRESSURE

} menuScreen_calibration;

typedef enum {

    MENU_OPTION_CALIBRATION_DIRECTION_LOWER,
    MENU_OPTION_CALIBRATION_DIRECTION_UPPER

} menuScreen_calibrationDirection;

typedef enum {

    MENU_OPTION_DEVICE_INFO_SW,
    MENU_OPTION_DEVICE_INFO_HW

} menuScreen_deviceInfo;

typedef enum {

    MENUSCREEN_FACTORY_RESET_FULL,
    MENUSCREEN_FACTORY_RESET_PARTIAL

} menuScreen_factoryReset;


#endif