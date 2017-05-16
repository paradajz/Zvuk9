#include "Curves.h"
#include "../Pads.h"
#include "functions/Functions.h"

extern uint8_t (*curveFunc[NUMBER_OF_CURVES]) (uint8_t value, uint8_t min, uint8_t max);

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
    //for (int i=0; i<PAD_COORDINATES; i++)
    //{
        //lastCurve[i] = curve_linear_up_1;
        //lastMin[i] = 0;
        //lastMax[i] = 127;
//
        //for (int j=0; j<128; j++)
            //scale[i][j] = j;
    //}

    initHandlers_functions();
}

///
/// \brief Maps value from one range to another.
/// @param [in] x Input value.
/// @param [in] in_min  Lowest possible input value.
/// @param [in] in_max  Largest possible input value.
/// @param [in] out_min Lowest possible output value.
/// @param [in] out_max Largest possible output value.
/// \return Mapped value (uint16_t).
///
uint16_t Curves::map_uint16(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

///
/// \brief Maps value from one range to another.
/// @param [in] x Input value.
/// @param [in] in_min  Lowest possible input value.
/// @param [in] in_max  Largest possible input value.
/// @param [in] out_min Lowest possible output value.
/// @param [in] out_max Largest possible output value.
/// \return Mapped value (uint8_t).
///
uint8_t Curves::map_uint8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max)
{
    if ((in_min == out_min) && (in_max == out_max))
        return x;

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};

///
/// \brief Calculates inverted value based on input range.
/// @param [in] value   Input value.
/// @param [in] min     Lowest possible input value.
/// @param [in] max     Largest possible input value.
/// \return Inverted value.
///
uint8_t Curves::invertRange(uint8_t value, uint8_t min, uint8_t max)
{
    return (max + min) - value;
}

uint8_t Curves::getCurveValue(curve_t curve, uint8_t value, uint8_t min, uint8_t max)
{
    return (*curveFunc[curve])(value, min, max);
}

Curves curves;
