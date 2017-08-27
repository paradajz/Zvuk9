#include "Curves.h"
#include "../Pads.h"
#include "Functions.h"

///
/// \brief Default constructor.
///
Curves::Curves()
{
    
}

///
/// \brief Initialize default values and function handlers.
///
void Curves::init()
{
    //get min/max for each curve
    for (int i=0; i<NUMBER_OF_CURVES; i++)
    {
        uint8_t min_val = 255;
        uint8_t max_val = 0;

        for (int j=0; j<128; j++)
        {
            uint8_t value = pgm_read_byte(&(curveArray[i][j]));

            if (value < min_val)
                min_val = value;

            if (value > max_val)
                max_val = value;
        }

        curveMin[i] = min_val;
        curveMax[i] = max_val;
    }
}

///
/// \brief Maps value from one range to another.
/// @param [in] x Input value.
/// @param [in] in_min  Lowest possible input value.
/// @param [in] in_max  Largest possible input value.
/// @param [in] out_min Lowest possible output value.
/// @param [in] out_max Largest possible output value.
/// \returns Mapped value (uint32_t).
///
int32_t Curves::map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

///
/// \brief Calculates inverted value based on input range.
/// @param [in] value   Input value.
/// @param [in] min     Lowest possible input value.
/// @param [in] max     Largest possible input value.
/// \returns Inverted value.
///
uint32_t Curves::invertRange(uint32_t value, uint32_t min, uint32_t max)
{
    return (max + min) - value;
}

///
/// \brief Returns value for wanted curve and curve index.
/// @param [in] curve   Wanted curve.
/// @param [in] value   Wanted curve index.
/// @param [in] min     Lowest possible output value.
/// @param [in] max     Largest possible output value.
/// \returns Curve value.
///
uint8_t Curves::getCurveValue(curve_t curve, uint8_t value, uint8_t min, uint8_t max)
{
    uint8_t out_min = min < curveMin[curve] ? curveMin[curve] : min;
    uint8_t out_max = max > curveMax[curve] ? curveMax[curve] : max;

    if (out_min || (out_max != 127))
         return map(pgm_read_byte(&(curveArray[curve][value])), curveMin[curve], curveMax[curve], out_min, out_max);
    else
        return pgm_read_byte(&(curveArray[curve][value]));
}

Curves curves;
