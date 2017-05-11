#pragma once

const char curveCoordinateX_string[] PROGMEM = "X curve=";
const char curveCoordinateY_string[] PROGMEM = "Y curve=";

PGM_P const curveCoordinateArray[] PROGMEM =
{
    //match order with coordinateType_t enum
    curveCoordinateX_string,
    curveCoordinateY_string
};

const uint8_t curveCoordinateArray_sizes[] PROGMEM =
{
    //match order with coordinateType_t enum
    sizeof(curveCoordinateX_string) - 1,
    sizeof(curveCoordinateY_string) - 1
};

const char curve_linear_up_1_string[] PROGMEM = "Linear up 1";
const char curve_linear_down_1_string[] PROGMEM = "Linear down 1";
const char curve_linear_up_2_string[] PROGMEM = "Linear up 2";
const char curve_linear_up_broken_1_string[] PROGMEM = "Linear up broken 1";
const char curve_linear_up_broken_4_string[] PROGMEM = "Linear up broken 4";
const char curve_flat_center_up_1_string[] PROGMEM = "Flat center up 1";
const char curve_log_up_1_string[] PROGMEM = "Log up 1";
const char curve_log_up_3_string[] PROGMEM = "Log up 3";
const char curve_exp_up_2_string[] PROGMEM = "Log exp up 2";
const char curve_switch_1_string[] PROGMEM = "Switch 1";
const char curve_linear_down_broken_2_string[] PROGMEM = "Linear down broken 2";
const char curve_five_steps_up_string[] PROGMEM = "Five steps up";
const char curve_linear_up_down_broken_1_string[] PROGMEM = "Linear up down broken 1";
const char curve_eight_waves_up_down_string[] PROGMEM = "Eight waves up down";

PGM_P const curveNameArray[] PROGMEM =
{
    curve_linear_up_1_string,
    curve_linear_down_1_string,
    curve_linear_up_2_string,
    curve_linear_up_broken_1_string,
    curve_linear_up_broken_4_string,
    curve_flat_center_up_1_string,
    curve_log_up_1_string,
    curve_log_up_3_string,
    curve_exp_up_2_string,
    curve_switch_1_string,
    curve_linear_down_broken_2_string,
    curve_five_steps_up_string,
    curve_linear_up_down_broken_1_string,
    curve_eight_waves_up_down_string
};

const uint8_t curveNameArray_sizes[] PROGMEM =
{
    //match order with coordinateType_t enum
    sizeof(curve_linear_up_1_string) - 1,
    sizeof(curve_linear_down_1_string) - 1,
    sizeof(curve_linear_up_2_string) - 1,
    sizeof(curve_linear_up_broken_1_string) - 1,
    sizeof(curve_linear_up_broken_4_string) - 1,
    sizeof(curve_flat_center_up_1_string) - 1,
    sizeof(curve_log_up_1_string) - 1,
    sizeof(curve_log_up_3_string) - 1,
    sizeof(curve_exp_up_2_string) - 1,
    sizeof(curve_switch_1_string) - 1,
    sizeof(curve_linear_down_broken_2_string) - 1,
    sizeof(curve_five_steps_up_string) - 1,
    sizeof(curve_linear_up_down_broken_1_string) - 1,
    sizeof(curve_eight_waves_up_down_string) - 1
};