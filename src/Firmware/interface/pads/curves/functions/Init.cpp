#include "Functions.h"
#include "../DataTypes.h"

///
/// \brief Array holding pointers to functions for easier access.
///
uint8_t (*curveFunc[NUMBER_OF_CURVES]) (uint8_t value, uint8_t min, uint8_t max);

void initHandlers_functions()
{
    curveFunc[curve_linear_up_1] = linear_up_1;
    curveFunc[curve_linear_down_1] = linear_down_1;
    curveFunc[curve_linear_up_2] = linear_up_2;
    curveFunc[curve_linear_up_broken_1] = linear_up_broken_1;
    curveFunc[curve_linear_up_broken_4] = linear_up_broken_4;
    curveFunc[curve_flat_center_up_1] = flat_center_up_1;
    curveFunc[curve_log_up_1] = log_up_1;
    curveFunc[curve_log_up_3] = log_up_3;
    curveFunc[curve_exp_up_2] = exp_up_2;
    curveFunc[curve_switch_1] = switch_1;
    curveFunc[curve_linear_down_broken_2] = linear_down_broken_2;
    curveFunc[curve_five_steps_up] = five_steps_up;
    curveFunc[curve_linear_up_down_broken_1] = linear_up_down_broken_1;
    curveFunc[curve_eight_waves_up_down] = eight_waves_up_down;
}