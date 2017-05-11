#pragma once

///
/// \brief List of all possible curves.
///
typedef enum
{
    curve_linear_up_1,
    curve_linear_down_1,
    curve_linear_up_2,
    curve_linear_up_broken_1,
    curve_linear_up_broken_4,
    curve_flat_center_up_1,
    curve_log_up_1,
    curve_log_up_3,
    curve_exp_up_2,
    curve_switch_1,
    curve_linear_down_broken_2,
    curve_five_steps_up,
    curve_linear_up_down_broken_1,
    curve_eight_waves_up_down,
    NUMBER_OF_CURVES
} curve_t;