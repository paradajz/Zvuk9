#include "Functions.h"
#include "../Curves.h"

uint8_t linear_up_1(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, 0, 127, min, max);
    else
        return value;
}

uint8_t linear_up_2(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&linear_up_2_values[0]), pgm_read_byte(&linear_up_2_values[127]), pgm_read_byte(&linear_up_2_values[min]), pgm_read_byte(&linear_up_2_values[max]));
    else
        return pgm_read_byte(&linear_up_2_values[value]);
}

uint8_t linear_down(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, 127, 0, min, max);
    else
        return value;
}

uint8_t linear_up_broken_1(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&linear_up_broken_1_values[0]), pgm_read_byte(&linear_up_broken_1_values[127]), pgm_read_byte(&linear_up_broken_1_values[min]), pgm_read_byte(&linear_up_broken_1_values[max]));
    else
        return pgm_read_byte(&linear_up_broken_1_values[value]);
}

uint8_t linear_up_broken_2(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&linear_up_broken_2_values[0]), pgm_read_byte(&linear_up_broken_2_values[127]), pgm_read_byte(&linear_up_broken_2_values[min]), pgm_read_byte(&linear_up_broken_2_values[max]));
    else
        return pgm_read_byte(&linear_up_broken_2_values[value]);
}

uint8_t linear_up_down_broken(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&linear_up_down_broken_values[0]), pgm_read_byte(&linear_up_down_broken_values[127]), pgm_read_byte(&linear_up_down_broken_values[min]), pgm_read_byte(&linear_up_down_broken_values[max]));
    else
        return pgm_read_byte(&linear_up_down_broken_values[value]);
}

uint8_t linear_down_broken(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&linear_down_broken_values[0]), pgm_read_byte(&linear_down_broken_values[127]), pgm_read_byte(&linear_down_broken_values[min]), pgm_read_byte(&linear_down_broken_values[max]));
    else
        return pgm_read_byte(&linear_down_broken_values[value]);
}

uint8_t flat_center_up(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&flat_center_up_values[0]), pgm_read_byte(&flat_center_up_values[127]), pgm_read_byte(&flat_center_up_values[min]), pgm_read_byte(&flat_center_up_values[max]));
    else
        return pgm_read_byte(&flat_center_up_values[value]);
}

uint8_t log_up_1(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&log_up_1_values[0]), pgm_read_byte(&log_up_1_values[127]), pgm_read_byte(&log_up_1_values[min]), pgm_read_byte(&log_up_1_values[max]));
    else
        return pgm_read_byte(&log_up_1_values[value]);
}

uint8_t log_up_2(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&log_up_2_values[0]), pgm_read_byte(&log_up_2_values[127]), pgm_read_byte(&log_up_2_values[min]), pgm_read_byte(&log_up_2_values[max]));
    else
        return pgm_read_byte(&log_up_2_values[value]);
}

uint8_t exp_up(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&exp_up_values[0]), pgm_read_byte(&exp_up_values[127]), pgm_read_byte(&exp_up_values[min]), pgm_read_byte(&exp_up_values[max]));
    else
        return pgm_read_byte(&exp_up_values[value]);
}

uint8_t switch_(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&switch_values[0]), pgm_read_byte(&switch_values[127]), pgm_read_byte(&switch_values[min]), pgm_read_byte(&switch_values[max]));
    else
        return pgm_read_byte(&switch_values[value]);
}

uint8_t five_steps_up(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&five_steps_up_values[0]), pgm_read_byte(&five_steps_up_values[127]), pgm_read_byte(&five_steps_up_values[min]), pgm_read_byte(&five_steps_up_values[max]));
    else
        return pgm_read_byte(&five_steps_up_values[value]);
}

uint8_t eight_waves_up_down(uint8_t value, uint8_t min, uint8_t max)
{
    if (min || (max != 127))
        return curves.map_uint8(value, pgm_read_byte(&eight_waves_up_down_values[0]), pgm_read_byte(&eight_waves_up_down_values[127]), pgm_read_byte(&eight_waves_up_down_values[min]), pgm_read_byte(&eight_waves_up_down_values[max]));
    else
        return pgm_read_byte(&eight_waves_up_down_values[value]);
}
