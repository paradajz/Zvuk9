#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

const char serviceMenuOption_exit[] PROGMEM = "Exit";

//menu
const char menuType_user[] PROGMEM = "User menu";
const char menuType_service[] PROGMEM = "Service menu";

PGM_P const menu_types[] PROGMEM = {

    menuType_user,
    menuType_service

};

const uint8_t menu_types_sizes[] PROGMEM = {

    sizeof(menuType_user) - 1,
    sizeof(menuType_service) - 1

};


//service menu options
const char serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char serviceMenuOption_deviceInfo[] PROGMEM = "Device info";
const char serviceMenuOption_factoryReset[] PROGMEM = "Factory reset";

PGM_P const service_menu_options[] PROGMEM = {

    serviceMenuOption_padCalibration,
    serviceMenuOption_deviceInfo,
    serviceMenuOption_factoryReset

};

const uint8_t service_menu_options_sizes[] PROGMEM = {

    sizeof(serviceMenuOption_padCalibration) - 1,
    sizeof(serviceMenuOption_deviceInfo) - 1,
    sizeof(serviceMenuOption_factoryReset) - 1,
    sizeof(serviceMenuOption_exit) - 1

};


//calibration menu options
const char calibration_x[] PROGMEM = "Calibrate X";
const char calibration_y[] PROGMEM = "Calibrate Y";
const char calibration_pressure[] PROGMEM = "Calibrate pressure";

PGM_P const calibration_menu_options[] PROGMEM = {

    calibration_x,
    calibration_y,
    calibration_pressure

};

const uint8_t calibration_menu_options_sizes[] PROGMEM = {

    sizeof(calibration_x) - 1,
    sizeof(calibration_y) - 1,
    sizeof(calibration_pressure) - 1

};


//calibration direction options
const char calibrationDirection_lower[] PROGMEM = "Lower";
const char calibrationDirection_upper[] PROGMEM = "Upper";

PGM_P const calibrationDirection_options[] PROGMEM = {

    calibrationDirection_lower,
    calibrationDirection_upper

};

const uint8_t calibrationDirection_options_sizes[] PROGMEM = {

    sizeof(calibrationDirection_lower) - 1,
    sizeof(calibrationDirection_upper) - 1

};

//calibration mode
const char calibrationMode_interactive[] PROGMEM = "Interactive";
const char calibrationMode_manual[] PROGMEM = "Manual";

PGM_P const calibration_mode[] PROGMEM = {

    calibrationMode_interactive,
    calibrationMode_manual

};

const uint8_t calibrationMode_sizes[] PROGMEM = {

    sizeof(calibrationMode_interactive) - 1,
    sizeof(calibrationMode_manual) - 1

};


//device info options
const char deviceInfo_swVersion[] PROGMEM = "SW version";
const char deviceInfo_hwVersion[] PROGMEM = "HW version";

PGM_P const deviceInfo_options[] PROGMEM = {

    deviceInfo_swVersion,
    deviceInfo_hwVersion

};

const uint8_t deviceInfo_options_sizes[] PROGMEM = {

    sizeof(deviceInfo_swVersion) - 1,
    sizeof(deviceInfo_hwVersion) - 1

};


//factory reset options
const char factoryReset_partial[] PROGMEM = "Partial reset";
const char factoryReset_full[] PROGMEM = "Full reset";

PGM_P const factoryReset_options[] PROGMEM = {

    factoryReset_partial,
    factoryReset_full

};

const uint8_t factoryReset_options_sizes[] PROGMEM = {

    sizeof(factoryReset_partial) - 1,
    sizeof(factoryReset_full) - 1

};


//pointers


#endif