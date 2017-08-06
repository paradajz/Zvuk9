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