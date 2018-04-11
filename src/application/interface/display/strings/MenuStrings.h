/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#pragma once

///
/// \brief Display strings used in user and service menus.
/// \ingroup displayStrings
/// @{

const char menuOption_exit_string[] PROGMEM = "Exit";

//menu
const char menuType_user_string[] PROGMEM = "Settings";
const char menuType_service_string[] PROGMEM = "Service menu";

const char menuOption_padCalibration_string[] PROGMEM = "Pad calibration";
const char menuOption_deviceInfo_string[] PROGMEM = "Device info";
const char menuOption_factoryReset_string[] PROGMEM = "Factory reset";
const char menuOption_factoryReset_title_string[] PROGMEM = "***FACTORY RESET***";
const char menuOption_velocitySettings_string[] PROGMEM = "Velocity settings";
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
const char pressure_curve_log_string[] PROGMEM = "Logarithmic";
const char pressure_curve_inv_exp_string[] PROGMEM = "Exponential";

const char midiSettings_midiChannel_string[] PROGMEM = "Channel";
const char midiSettings_atouchType_string[] PROGMEM = "Aftertouch type";
const char midiSettings_runningStatus_string[] PROGMEM = "Running status";
const char midiSettings_noteOff_string[] PROGMEM = "Note off type";
const char midiSettings_transport_string[] PROGMEM = "Transport controls";
const char midiSettings_pitchBend_string[] PROGMEM = "Pitch bend type";

const char midiSettings_transport_MMC_string[] PROGMEM = "MMC";
const char midiSettings_transport_CC_string[] PROGMEM = "CC";
const char midiSettings_transport_MMC_CC_string[] PROGMEM = "MMC+CC";

const char midiSettings_pitchBend_1_string[] PROGMEM = "Type 1";
const char midiSettings_pitchBend_2_string[] PROGMEM = "Type 2";

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

const char factory_reset_info_confirm_string[] PROGMEM = " Confirm with pads:";
const char factory_reset_pads_string[] PROGMEM = " 1[ ] 7[ ] 9[ ]";
const char factory_reset_info_cancel_string[] PROGMEM = " Cancel: any other";

const char factory_reset_start_1_string[] PROGMEM = "Performing factory reset.";
const char factory_reset_start_2_string[] PROGMEM = "Please wait.";
const char factory_reset_end_1_string[] PROGMEM = "Factory reset finished.";
const char factory_reset_end_2_string[] PROGMEM = "Initiating reboot.";

const char aftertouchPoly_string[] PROGMEM = "Key aftertouch";
const char aftertouchChannel_string[] PROGMEM = "Channel aftertouch";

/// @}