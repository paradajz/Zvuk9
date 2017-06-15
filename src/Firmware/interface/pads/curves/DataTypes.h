#pragma once

///
/// \brief List of all possible curves.
///
typedef enum
{
    curve_linear_up,
    curve_linear_down,
    curve_linear_up_broken_1,
    curve_linear_up_broken_2,
    curve_linear_up_down_broken,
    curve_linear_down_broken,
    curve_flat_center_up,
    curve_log_up_1,
    curve_log_up_2,
    curve_exp_up,
    curve_switch,
    curve_five_steps_up,
    curve_eight_waves_up_down,
    NUMBER_OF_CURVES
} curve_t;