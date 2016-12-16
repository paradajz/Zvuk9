#include "Curves.h"
#include "../Pads.h"
#include "Values.h"
#include <avr/pgmspace.h>

#define WIDE_MIDDLE_GAIN    8
#define WIDE_ENDS_GAIN      2

#define LOG_CURVE_INDEX     0
#define EXP_CURVE_INDEX     1

double bias(double b, double x)
{
    return pow(x, log(b)/LOG_05);
}

double gain(double g, double x)
{
    if (x < 0.5)
    return bias(1-g, 2*x)/2;
    else
    return 1 - bias(1-g,2 - 2*x)/2;
}

double gamma(double g, double x)
{
    return pow(x, (1.0/g));
}

uint8_t map_u8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};

Curves::Curves()    {

    //def const
    for (int i=0; i<3; i++)
    {
        lastCurve[i] = curveTypeLinear;
    }

    for (int i=0; i<2; i++)
    {
        lastMin[i] = 0;
        lastMax[i] = 127;
    }

    //x+y+z
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<128; j++)
        {
            scale[i][j] = j;
        }
    }

}

uint8_t Curves::getCurveValue(coordinateType_t coordinate, curveType_t type, uint8_t index, uint8_t min, uint8_t max)
{
    if ((uint8_t)type != lastCurve[(uint8_t)coordinate] || min != lastMin[(uint8_t)coordinate] || max != lastMax[(uint8_t)coordinate])
    {
        #ifdef DEBUG
        printf("Setting up new x/y scale\n");
        #endif

        bool minMax_differ = (min || (max != 127));

        if (type == curveTypeLinear)
        {
            //setup curve manually
            if (minMax_differ)
            {
                for (int i=0; i<128; i++)
                    scale[(uint8_t)coordinate][i]= map_u8(i, 0, 127, min, max);
            }
            else
            {
                for (int i=0; i<128; i++)
                    scale[(uint8_t)coordinate][i] = i;
            }
        }
        else if ((type == curveTypeLog) || (type == curveTypeExp))
        {
            if (type == curveTypeLog)
                index = LOG_CURVE_INDEX;
            else if (type == curveTypeExp)
                index = EXP_CURVE_INDEX;
            else
                return 0; //error

            memcpy_P(scale, (uint8_t*)pgm_read_word(&(scaleArray[index])), 128);
        }
        else
        {
            double curveGain_double;
            //scale gain value to values 0.0-1.0
            if (type == curveTypeWideEnds)
                curveGain_double = (uint8_t)WIDE_MIDDLE_GAIN/10.0;
            else if (type == curveTypeWideMiddle)
                curveGain_double = (uint8_t)WIDE_ENDS_GAIN/10.0;
            else
                return 0; //error

            uint8_t numberOfValues = max-min;
            double step, stepValue = 0.0;

            //scale range
            step = 1.0/(double)numberOfValues;

            for (int i=0; i<=numberOfValues; i++)
            {
                //make sure that curve extremes are correct
                if (!i)
                    scale[(uint8_t)coordinate][i] = 0;
                else if (i == numberOfValues)
                    scale[(uint8_t)coordinate][i] = numberOfValues;
                else
                    scale[(uint8_t)coordinate][i] = round(gain(curveGain_double, stepValue) * numberOfValues); //round up

                stepValue += step;
            }

            if (min > 0)
            {
                //if min value is zero, curve is already scaled
                uint8_t tempArray[numberOfValues+1];

                for (int i=0; i<=numberOfValues; i++)
                    tempArray[i] = scale[(uint8_t)coordinate][i];

                for (int i=0; i<=numberOfValues; i++)
                {
                    if (!i)
                        scale[(uint8_t)coordinate][i] = min;
                    else if (i == max)
                        scale[(uint8_t)coordinate][i] = max;
                    else
                        scale[(uint8_t)coordinate][i] = map_u8(tempArray[i], 0, numberOfValues, min, max);
                }
            }
        }

        lastCurve[(uint8_t)coordinate] = (uint8_t)type;

        if (minMax_differ)
        {
            lastMin[(uint8_t)coordinate] = min;
            lastMax[(uint8_t)coordinate] = max;
        }
    }

    return scale[(uint8_t)coordinate][index];
}

Curves curves;
