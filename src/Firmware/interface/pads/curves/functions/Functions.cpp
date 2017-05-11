#include "Functions.h"

double linear_up_1(double value)
{
    return value;
}

double linear_down_1(double value)
{
    return 1-value;
}

double linear_up_2(double value)
{
    return value*0.5;
}

double linear_up_broken_1(double value)
{
    return value*0.99-0.3;
}

double linear_up_broken_4(double value)
{
    return 0.0;
}

double flat_center_up_1(double value)
{
    return 0.0;
}

double log_up_1(double value)
{
    return 0.0;
}

double log_up_3(double value)
{
    return 0.0;
}

double exp_up_2(double value)
{
    return 0.0;
}

double switch_1(double value)
{
    return 0.0;
}

double linear_down_broken_2(double value)
{
    return 1-value*1.2-0.2;
}

double five_steps_up(double value)
{
    return 0.0;
}

double linear_up_down_broken_1(double value)
{
    return 0.0;
}

double eight_waves_up_down(double value)
{
    return 0.0;
}
