#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

const char menuOption_exit[] PROGMEM = "Exit";

//menu
const char menuType_user[] PROGMEM = "Settings";
const char menuType_service[] PROGMEM = "Service menu";
const char menuType_quick[] PROGMEM = "Quick actions";

const char menuOption_padCalibration[] PROGMEM = "Pad calibration";
const char menuOption_deviceInfo[] PROGMEM = "Device info";
const char menuOption_factoryReset[] PROGMEM = "Factory reset";
const char menuOption_pressureSensitivity[] PROGMEM = "Press. sensitivity";
const char menuOption_pressureCurve[] PROGMEM = "Press. curve";
const char menuOption_midiSettings[] PROGMEM = "MIDI settings";

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
const char deviceInfo_swVersion[] PROGMEM = "Software: ";
const char deviceInfo_hwVersion[] PROGMEM = "Hardware: ";

//factory reset options
const char factoryReset_partial_string[] PROGMEM = "Partial reset";
const char factoryReset_full_string[] PROGMEM = "Full reset";

const char confirm_partial_reset[] PROGMEM = "Confirm partial rst";
const char confirm_full_reset[] PROGMEM = "Confirm full rst";
const char back_option[] PROGMEM = "Back";

const char pressure_sensitivity_1[] PROGMEM = "Level 1";
const char pressure_sensitivity_2[] PROGMEM = "Level 2";
const char pressure_sensitivity_3[] PROGMEM = "Level 3";
const char pressure_sensitivity_4[] PROGMEM = "Level 4";

const char pressure_curve_lin[] PROGMEM = "Linear";
const char pressure_curve_log[] PROGMEM = "Logarithmic";
const char pressure_curve_inv_log[] PROGMEM = "Inv. logarithmic";

const char midiSettings_midiChannel[] PROGMEM = "Channel";
const char midiSettings_atouchType[] PROGMEM = "Aftertouch type";
const char midiSettings_runningStatus[] PROGMEM = "Running status";
const char midiSettings_noteOff[] PROGMEM = "Note off";

const char aftertouchType_channel[] PROGMEM = "Channel";
const char aftertouchType_poly[] PROGMEM = "Poly";

const char enabled_string[] PROGMEM = "Enabled";
const char disabled_string[] PROGMEM = "Disabled";

const char noteOff_zeroVel[] PROGMEM = "On channel, 0 vel";
const char noteOff_offChannel[] PROGMEM = "Off channel";

#endif