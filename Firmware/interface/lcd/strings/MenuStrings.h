#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

//menu names
const char PROGMEM menuType_user[] PROGMEM = "User menu";
const char PROGMEM menuType_service[] PROGMEM = "Service menu";

//service menu
const char PROGMEM serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char PROGMEM serviceMenuOption_exit[] PROGMEM = "Exit";
const char PROGMEM serviceMenuSubOption_padCalibration_x[] PROGMEM = "Calibrate X";
const char PROGMEM serviceMenuSubOption_padCalibration_y[] PROGMEM = "Calibrate Y";
const char PROGMEM serviceMenuSubOption_padCalibration_pressure[] PROGMEM = "Calibrate pressure";
const char PROGMEM serviceMenuSubOption_padCalibration_flipAxis[] PROGMEM = "Flip X/Y axis";

PGM_P const menu_types[] PROGMEM = {

    menuType_user,
    menuType_service

};

const uint8_t menu_types_sizes[] PROGMEM = {

    sizeof(menuType_user),
    sizeof(menuType_service)

};

PGM_P const service_menu_options[] PROGMEM = {

    serviceMenuOption_padCalibration,
    serviceMenuOption_exit

};

const uint8_t service_menu_options_sizes[] PROGMEM = {

    sizeof(serviceMenuOption_padCalibration),
    sizeof(serviceMenuOption_exit)

};

PGM_P const menu_subOptions_padCalibration[] PROGMEM = {

    serviceMenuSubOption_padCalibration_x,
    serviceMenuSubOption_padCalibration_y,
    serviceMenuSubOption_padCalibration_pressure,
    serviceMenuSubOption_padCalibration_flipAxis

};

#endif