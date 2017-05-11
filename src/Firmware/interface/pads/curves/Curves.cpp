#include "Curves.h"
#include "../Pads.h"
#include "functions/Functions.h"

extern double (*curveFunc[NUMBER_OF_CURVES]) (double value);

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
    for (int i=0; i<PAD_COORDINATES; i++)
    {
        lastCurve[i] = curve_linear_up_1;
        lastMin[i] = 0;
        lastMax[i] = 127;

        for (int j=0; j<128; j++)
            scale[i][j] = j;
    }

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

uint8_t Curves::getCurveValue(padCoordinate_t coordinate, curve_t curve, uint8_t value, uint8_t min, uint8_t max)
{
    bool minMax_differ = false;

    //simplest
    switch(coordinate)
    {
        case coordinateX:
        case coordinateY:
        //check min/max for these coordinates
        if (min != lastMin[(uint8_t)coordinate])
        {
            lastMin[(uint8_t)coordinate] = min;
            minMax_differ = true;
        }

        if (max != lastMax[(uint8_t)coordinate])
        {
            lastMax[(uint8_t)coordinate] = max;
            minMax_differ = true;
        }
        break;

        case coordinateZ:
        //no min/max checking
        minMax_differ = false;
        break;

        default:
        return 0;
    }

    uint8_t numberOfValues = max-min+1;

    if ((uint8_t)curve != lastCurve[(uint8_t)coordinate] || minMax_differ)
    {
        double step, stepValue = 0.0;

        //scale range
        step = 1.0/(double)numberOfValues;

        //#ifdef DEBUG
        //printf_P("Printing scale values.\n");
        //#endif

        //for (int i=0; i<numberOfValues; i++)
        //{
            ////make sure that curve extremes are correct
            //if (!i)
                //scale[(uint8_t)coordinate][i] = 0;
            //else if (i == numberOfValues-1)
                //scale[(uint8_t)coordinate][i] = numberOfValues-1;
            //else
                //scale[(uint8_t)coordinate][i] = round(gain(curveGain_double, stepValue) * (numberOfValues-1)); //round up
//
            //stepValue += step;
        //}

        if (min > 0)
        {
            //if min value is zero, curve is already scaled
            uint8_t tempArray[numberOfValues];

            for (int i=0; i<numberOfValues; i++)
                tempArray[i] = scale[(uint8_t)coordinate][i];

            for (int i=0; i<numberOfValues; i++)
            {
                if (!i)
                    scale[(uint8_t)coordinate][i] = min;
                else if (i == max)
                    scale[(uint8_t)coordinate][i] = max;
                else
                    scale[(uint8_t)coordinate][i] = map_uint8(tempArray[i], 0, numberOfValues-1, min, max);
            }
        }

        lastCurve[(uint8_t)coordinate] = (uint8_t)curve;
    }

    //scale index if necessary
    if (min || max < 127)
        value = map_uint8(value, 0, 127, 0, numberOfValues-1);

    return scale[(uint8_t)coordinate][value];
}

Curves curves;
