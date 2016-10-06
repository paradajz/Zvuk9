#include "Curves.h"
#include "../Pads.h"
#include "Values.h"
#include <avr/pgmspace.h>
#include <math.h>

#define LOG_05      (double)log((double)0.5)

double bias(double b, double x)    {

    return pow(x, log(b)/LOG_05);

}

double gain(double g, double x)    {

    if (x < 0.5)
        return bias(1-g, 2*x)/2;
    else return 1 - bias(1-g,2 - 2*x)/2;

}

uint8_t map_u8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

};

Curves::Curves()    {

    //def const
    for (int i=0; i<2; i++) {

        lastGain[i] = curveTypeLinear;
        lastMin[i] = 0;
        lastMax[i] = 127;

    }

    //set linear scale by default
    memcpy_P(scale, (uint8_t*)pgm_read_word(&(scaleArray[(uint8_t)curveTypeLinear-1])), 128);

}

void Curves::setupCurve(uint8_t curveGain, uint8_t min, uint8_t max)   {

    if (curveGain == 5)    {

        //normally, gain 0.5 produces linear scale
        //however, we are converting decimal numbers to int so there is slight error
        //in this case, setup curve manually
        for (int i=0; i<128; i++)
            scale[i] = i;

        //scale range if min isn't 0 or max isn't 127
        if (min || max != 127) {

            for (int i=0; i<128; i++)
                scale[i] = map_u8(i, 0, 127, min, max);

        }

    }   else {

        //printf("Curve scaling updated. Min: %d, max: %d\n", min, max);

        double curveGain_double = curveGain/10.0; //scale gain value to values 0.0-1.0
        uint8_t numberOfValues = max-min;
        double step, stepValue = 0.0;

        //scale range
        step = 1.0/(double)numberOfValues;

        for (int i=0; i<=numberOfValues; i++)   {

            //make sure that curve extremes are correct
            if (!i)
                scale[i] = 0;
            else if (i == numberOfValues)
                scale[i] = numberOfValues;
            else
                scale[i] = round(gain(curveGain_double, stepValue) * numberOfValues); //round up

            stepValue += step;

        }

        if (min > 0)   {

            //if min value is zero, curve is already scaled
            uint8_t tempArray[numberOfValues+1];

            for (int i=0; i<=numberOfValues; i++)
                tempArray[i] = scale[i];

            for (int i=0; i<=numberOfValues; i++)   {

                if (!i)
                    scale[i] = min;
                else if (i == max)
                    scale[i] = max;
                else
                    scale[i] = map_u8(tempArray[i], 0, numberOfValues, min, max);

            }

        }

    }

}

uint8_t Curves::getCurveValue(coordinateType_t coordinate, uint8_t curveGain, uint8_t index, uint8_t min, uint8_t max)    {

    if (curveGain != lastGain[(uint8_t)coordinate])   {

        #ifdef DEBUG
        printf("Setting up new x/y scale\n");
        #endif

        //only copy bytes from flash if received gain differs from last one
        memcpy_P(scale, (uint8_t*)pgm_read_word(&(scaleArray[curveGain-1])), 128);

        lastGain[(uint8_t)coordinate] = curveGain;

        //#ifdef DEBUG
            //printf("Curve gain updated for coordinate %d. New gain: %d\n", coordinate, curveGain);
            //printf("scale:\n");
            //for (int i=0; i<128; i++)
                //printf("%d\n", scale[i]);
        //#endif

    }

    if ((min != 0) || (max != 127))    {

        if ((lastMin[(uint8_t)coordinate] != min) || (lastMax[(uint8_t)coordinate] != max))   {

            #ifdef DEBUG
            printf("Setting up new x/y value\n");
            #endif

            lastMin[(uint8_t)coordinate] = min;
            lastMax[(uint8_t)coordinate] = max;

            //in this case, we are setting up curve manually
            setupCurve(curveGain, min, max);

        }

    }

    return scale[index];

}

Curves curves;