#include "Functions.h"
#include "../DataTypes.h"

///
/// \brief Array holding pointers to functions for easier access.
///
uint8_t (*curveFunc[NUMBER_OF_CURVES]) (uint8_t value, uint8_t min, uint8_t max);

void initHandlers_functions()
{
    curveFunc[curve_linear_up_1] = linear_up_1;
    curveFunc[curve_linear_down] = linear_down;
    curveFunc[curve_linear_up_2] = linear_up_2;
    curveFunc[curve_linear_up_broken_1] = linear_up_broken_1;
    curveFunc[curve_linear_up_broken_2] = linear_up_broken_2;
    curveFunc[curve_flat_center_up] = flat_center_up;
    curveFunc[curve_log_up_1] = log_up_1;
    curveFunc[curve_log_up_2] = log_up_2;
    curveFunc[curve_exp_up] = exp_up;
    curveFunc[curve_switch] = switch_;
    curveFunc[curve_linear_down_broken] = linear_down_broken;
    curveFunc[curve_five_steps_up] = five_steps_up;
    curveFunc[curve_linear_up_down_broken] = linear_up_down_broken;
    curveFunc[curve_eight_waves_up_down] = eight_waves_up_down;
}