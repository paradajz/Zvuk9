/*
 * Strings.h
 *
 *  Created on: Mar 11, 2016
 *      Author: igor
 */

#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>
#include "Arduino.h"

//service menu
const char PROGMEM serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char PROGMEM serviceMenuOption_exit[] PROGMEM = "Exit";
const char PROGMEM serviceMenuSubOption_padCalibration_x[] PROGMEM = "Calibrate X";
const char PROGMEM serviceMenuSubOption_padCalibration_y[] PROGMEM = "Calibrate Y";
const char PROGMEM serviceMenuSubOption_padCalibration_pressure[] PROGMEM = "Calibrate pressure";
const char PROGMEM serviceMenuSubOption_padCalibration_flipAxis[] PROGMEM = "Flip X/Y axis";

PGM_P const service_menu_options[] PROGMEM = {

    serviceMenuOption_padCalibration,
    serviceMenuOption_exit

};

PGM_P const menu_subOptions_padCalibration[] PROGMEM = {

        serviceMenuSubOption_padCalibration_x,
        serviceMenuSubOption_padCalibration_y,
        serviceMenuSubOption_padCalibration_pressure,
        serviceMenuSubOption_padCalibration_flipAxis

};

#endif /* MENU_SERVICEMENUSTRINGS_H_ */
