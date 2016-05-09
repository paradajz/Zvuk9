#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

//menu names
const char menuType_user[] PROGMEM = "User menu";
const char menuType_service[] PROGMEM = "Service menu";

//service menu
const char serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char serviceMenuOption_factoryReset[] PROGMEM = "Factory reset";
const char serviceMenuOption_exit[] PROGMEM = "Exit";
const char serviceMenuSubOption_padCalibration_x[] PROGMEM = "Calibrate X";
const char serviceMenuSubOption_padCalibration_y[] PROGMEM = "Calibrate Y";
const char serviceMenuSubOption_padCalibration_pressure[] PROGMEM = "Calibrate pressure";

PGM_P const menu_types[] PROGMEM = {

    menuType_user,
    menuType_service

};

const uint8_t menu_types_sizes[] PROGMEM = {

    sizeof(menuType_user) - 1,
    sizeof(menuType_service) - 1

};

PGM_P const service_menu_options[] PROGMEM = {

    serviceMenuOption_padCalibration,
    serviceMenuOption_factoryReset,
    serviceMenuOption_exit

};

const uint8_t service_menu_options_sizes[] PROGMEM = {

    sizeof(serviceMenuOption_padCalibration) - 1,
    sizeof(serviceMenuOption_factoryReset) - 1,
    sizeof(serviceMenuOption_exit) - 1

};

PGM_P const menu_subOptions_padCalibration[] PROGMEM = {

    serviceMenuSubOption_padCalibration_x,
    serviceMenuSubOption_padCalibration_y,
    serviceMenuSubOption_padCalibration_pressure

};

#endif