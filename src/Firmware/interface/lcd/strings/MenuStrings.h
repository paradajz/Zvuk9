#ifndef MENU_SERVICEMENUSTRINGS_H_
#define MENU_SERVICEMENUSTRINGS_H_

#include <avr/pgmspace.h>

const char menuOption_exit_string[] PROGMEM = "Exit";

//menu
const char menuType_user_string[] PROGMEM = "Settings";
const char menuType_service_string[] PROGMEM = "Service menu";

const char menuOption_padCalibration_string[] PROGMEM = "Pad calibration";
const char menuOption_deviceInfo_string[] PROGMEM = "Device info";
const char menuOption_factoryReset_string[] PROGMEM = "Factory reset";
const char menuOption_factoryReset_caps_string[] PROGMEM = "***FACTORY RESET***";
const char menuOption_pressureSettings_string[] PROGMEM = "Press. settings";
const char menuOption_pressureSensitivity_string[] PROGMEM = "Sensitivity";
const char menuOption_pressureCurve_string[] PROGMEM = "Curve";
const char menuOption_midiSettings_string[] PROGMEM = "MIDI settings";

//calibration menu options
const char calibration_x_string[] PROGMEM = "Calibrate X";
const char calibration_y_string[] PROGMEM = "Calibrate Y";
const char calibration_pressure_string[] PROGMEM = "Calibrate pressure";

//calibration direction options
const char calibrationDirection_lower_string[] PROGMEM = "Lower";
const char calibrationDirection_upper_string[] PROGMEM = "Upper";

//calibration mode
const char calibrationMode_interactive_string[] PROGMEM = "Interactive";
const char calibrationMode_manual_string[] PROGMEM = "Manual";

//device info options
const char deviceInfo_swVersion_string[] PROGMEM = "Software: ";
const char deviceInfo_hwVersion_string[] PROGMEM = "Hardware: ";

//factory reset options
const char factoryReset_partial_string[] PROGMEM = "Partial reset";
const char factoryReset_full_string[] PROGMEM = "Full reset";

const char confirm_string[] PROGMEM = "Confirm";
const char back_string[] PROGMEM = "Back";

const char pressure_sensitivity_1_string[] PROGMEM = "Soft";
const char pressure_sensitivity_2_string[] PROGMEM = "Medium";
const char pressure_sensitivity_3_string[] PROGMEM = "Hard";

const char pressure_curve_linear_string[] PROGMEM = "Linear";
const char pressure_curve_log_string[] PROGMEM = "Log.";
const char pressure_curve_inv_exp_string[] PROGMEM = "Exp.";

const char midiSettings_midiChannel_string[] PROGMEM = "Channel";
const char midiSettings_atouchType_string[] PROGMEM = "Aftertouch type";
const char midiSettings_runningStatus_string[] PROGMEM = "Running status";
const char midiSettings_noteOff_string[] PROGMEM = "Note off";

const char aftertouchType_channel_string[] PROGMEM = "Channel";
const char aftertouchType_poly_string[] PROGMEM = "Poly";

const char enabled_string_string[] PROGMEM = "Enabled";
const char disabled_string_string[] PROGMEM = "Disabled";

const char noteOff_zeroVel_string[] PROGMEM = "On channel/0 vel";
const char noteOff_offChannel_string[] PROGMEM = "Off channel";

const char checked_string[] PROGMEM = "[x]";
const char unchecked_string[] PROGMEM = "[ ]";

const char yes_string[] PROGMEM = "Yes";
const char no_string[] PROGMEM = "No";

//hardcoding...
#define FACTORY_RESET_STRING_PAD_1_LOCATION 4
#define FACTORY_RESET_STRING_PAD_7_LOCATION 9
#define FACTORY_RESET_STRING_PAD_9_LOCATION 14

const char factory_reset_warning_1_string[] PROGMEM = " Confirm with pads:";
const char factory_reset_warning_2_string[] PROGMEM = "  1[ ] 7[ ] 9[ ]   ";
const char factory_reset_warning_3_string[] PROGMEM = " Cancel: any other";

#endif