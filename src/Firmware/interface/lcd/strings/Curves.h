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

const char curve_linear_up_string[] PROGMEM = "Linear up";
const char curve_linear_down_string[] PROGMEM = "Linear down";
const char curve_linear_up_broken_1_string[] PROGMEM = "Linear up broken 1";
const char curve_linear_up_broken_2_string[] PROGMEM = "Linear up broken 2";
const char curve_linear_up_down_broken_string[] PROGMEM = "Linear up down broken";
const char curve_linear_down_broken_string[] PROGMEM = "Linear down broken";
const char curve_flat_center_up_string[] PROGMEM = "Flat center up";
const char curve_log_up_1_string[] PROGMEM = "Log up 1";
const char curve_log_up_2_string[] PROGMEM = "Log up 2";
const char curve_exp_up_string[] PROGMEM = "Log exp up";
const char curve_switch_string[] PROGMEM = "Switch";
const char curve_five_steps_up_string[] PROGMEM = "Five steps up";
const char curve_eight_waves_up_down_string[] PROGMEM = "Eight waves up down";

//match with curve_t
PGM_P const curveNameArray[] PROGMEM =
{
    curve_linear_up_string,
    curve_linear_down_string,
    curve_linear_up_broken_1_string,
    curve_linear_up_broken_2_string,
    curve_linear_up_down_broken_string,
    curve_linear_down_broken_string,
    curve_flat_center_up_string,
    curve_log_up_1_string,
    curve_log_up_2_string,
    curve_exp_up_string,
    curve_switch_string,
    curve_five_steps_up_string,
    curve_eight_waves_up_down_string
};

//match with curve_t
const uint8_t curveNameArray_sizes[] PROGMEM =
{
    //match order with coordinateType_t enum
    ARRAY_SIZE_CHAR(curve_linear_up_string),
    ARRAY_SIZE_CHAR(curve_linear_down_string),
    ARRAY_SIZE_CHAR(curve_linear_up_broken_1_string),
    ARRAY_SIZE_CHAR(curve_linear_up_broken_2_string),
    ARRAY_SIZE_CHAR(curve_linear_up_down_broken_string),
    ARRAY_SIZE_CHAR(curve_linear_down_broken_string),
    ARRAY_SIZE_CHAR(curve_flat_center_up_string),
    ARRAY_SIZE_CHAR(curve_log_up_1_string),
    ARRAY_SIZE_CHAR(curve_log_up_2_string),
    ARRAY_SIZE_CHAR(curve_exp_up_string),
    ARRAY_SIZE_CHAR(curve_switch_string),
    ARRAY_SIZE_CHAR(curve_five_steps_up_string),
    ARRAY_SIZE_CHAR(curve_eight_waves_up_down_string)
};
