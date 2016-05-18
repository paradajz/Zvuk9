#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

const char serviceMenuOption_exit[] PROGMEM = "Exit";

//menu
const char menuType_user[] PROGMEM = "User menu";
const char menuType_service[] PROGMEM = "Service menu";

//service menu options
const char serviceMenuOption_padCalibration[] PROGMEM = "Pad calibration";
const char serviceMenuOption_deviceInfo[] PROGMEM = "Device info";
const char serviceMenuOption_factoryReset[] PROGMEM = "Factory reset";

//calibration menu options
const char calibration_x[] PROGMEM = "Calibrate X";
const char calibration_y[] PROGMEM = "Calibrate Y";
const char calibration_pressure[] PROGMEM = "Calibrate pressure";

//calibration direction options
const char calibrationDirection_lower[] PROGMEM = "Lower";
const char calibrationDirection_upper[] PROGMEM = "Upper";

//calibration mode
const char calibrationMode_interactive[] PROGMEM = "Interactive";
const char calibrationMode_manual[] PROGMEM = "Manual";

//device info options
const char deviceInfo_swVersion[] PROGMEM = "SW version";
const char deviceInfo_hwVersion[] PROGMEM = "HW version";

//factory reset options
const char factoryReset_partial_string[] PROGMEM = "Partial reset";
const char factoryReset_full_string[] PROGMEM = "Full reset";

const char yes_option[] PROGMEM = "Yes";
const char no_option[] PROGMEM = "No";

const char dummy_string[] PROGMEM = "dummy";
#endif