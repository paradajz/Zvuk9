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
    if ((in_min == out_min) && (in_max == out_max))
        return x;

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};

Curves::Curves()
{
    //def const
    for (int i=0; i<PAD_COORDINATES; i++)
    {
        lastCurve[i] = curveLinear;
        lastMin[i] = 0;
        lastMax[i] = 127;

        for (int j=0; j<128; j++)
            scale[i][j] = j;
    }
}

uint8_t Curves::getCurveValue(padCoordinate_t coordinate, curve_t curve, uint8_t index, uint8_t min, uint8_t max)
{
    bool minMax_differ = false;

    if (coordinate == coordinateX)
    {
        #ifdef DEBUG
        printf("Requested index: %d\n", index);
        #endif
    }

    if (curve == curveLinear)
    {
        if (min || (max < 127))
            return map_u8(index, 0, 127, min, max);
        else
            return index;
    }
    else
    {
        switch(coordinate)
        {
            case coordinateX:
            case coordinateY:
            //check min/max for these coordinates
            if (min != lastMin[(uint8_t)coordinate])
            {
                #ifdef DEBUG
                printf("Updating last min value for this scale.\n");
                #endif
                lastMin[(uint8_t)coordinate] = min;
                minMax_differ = true;
            }

            if (max != lastMax[(uint8_t)coordinate])
            {
                #ifdef DEBUG
                printf("Updating last max value for this scale.\n");
                #endif
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
            #ifdef DEBUG
            printf("Setting up new x/y scale\n");
            #endif

            if ((curve == curveLog) || (curve == curveExp))
            {
                //used only for pressure, no min/max checking
                uint8_t curveIndex = 0;

                if (curve == curveLog)
                    curveIndex = LOG_CURVE_INDEX;
                else if (curve == curveExp)
                    curveIndex = EXP_CURVE_INDEX;
                else
                return 0; //error

                memcpy_P(scale, (uint8_t*)pgm_read_word(&(scaleArray[curveIndex])), 128);
            }
            else
            {
                double curveGain_double;
                //scale gain value to values 0.0-1.0
                if (curve == curveWideEnds)
                    curveGain_double = (uint8_t)WIDE_MIDDLE_GAIN/10.0;
                else if (curve == curveWideMiddle)
                    curveGain_double = (uint8_t)WIDE_ENDS_GAIN/10.0;
                else
                    return 0; //error

                double step, stepValue = 0.0;

                #ifdef DEBUG
                printf("Total of %d values for this scale.\n", numberOfValues);
                #endif

                //scale range
                step = 1.0/(double)numberOfValues;

                #ifdef DEBUG
                printf("Printing scale values.\n");
                #endif

                for (int i=0; i<numberOfValues; i++)
                {
                    //make sure that curve extremes are correct
                    if (!i)
                        scale[(uint8_t)coordinate][i] = 0;
                    else if (i == numberOfValues-1)
                        scale[(uint8_t)coordinate][i] = numberOfValues-1;
                    else
                        scale[(uint8_t)coordinate][i] = round(gain(curveGain_double, stepValue) * (numberOfValues-1)); //round up

                    stepValue += step;
                }

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
                            scale[(uint8_t)coordinate][i] = map_u8(tempArray[i], 0, numberOfValues-1, min, max);
                    }
                }
            }

            lastCurve[(uint8_t)coordinate] = (uint8_t)curve;

            #ifdef DEBUG
            printf("Printing scale values.\n");
            for (int i=0; i<numberOfValues; i++)
                printf("Index: %d, value: %d\n", i, scale[(uint8_t)coordinate][i]);
            #endif
        }

        if (coordinate == coordinateX)
        {
            #ifdef DEBUG
            printf("Returning value: %d\n", scale[(uint8_t)coordinate][index]);
            #endif
        }

        //scale index if necessary
        if (min || max < 127)
        {
            index = map_u8(index, 0, 127, 0, numberOfValues-1);
            #ifdef DEBUG
            printf("Scaled index: %d\n", index);
            #endif
        }

        return scale[(uint8_t)coordinate][index];
    }
}

Curves curves;
