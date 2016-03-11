/*
 * UserMenuStrings.h
 *
 *  Created on: Mar 11, 2016
 *      Author: igor
 */

#ifndef MENU_USERMENUSTRINGS_H_
#define MENU_USERMENUSTRINGS_H_


#include <avr/pgmspace.h>
#include "Arduino.h"


//user menu
const char PROGMEM serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char PROGMEM serviceMenuSubOption_padCalibration_x[] PROGMEM = "Calibrate X";
const char PROGMEM serviceMenuSubOption_padCalibration_y[] PROGMEM = "Calibrate Y";
const char PROGMEM serviceMenuSubOption_padCalibration_pressure[] PROGMEM = "Calibrate pressure";

const char PROGMEM serviceMenuOption_factoryReset[] PROGMEM = "Factory reset";

PGM_P const service_menu_options[] PROGMEM = {

	serviceMenuOption_padCalibration,
	serviceMenuOption_factoryReset,

};


#endif /* MENU_USERMENUSTRINGS_H_ */
