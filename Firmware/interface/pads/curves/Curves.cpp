#include "Curves.h"
#include "../Pads.h"
#include "Values.h"
#include <avr/pgmspace.h>

Curves::Curves()    {

    //def const
    lastGain = -1;
    lastMin = -1;
    lastMax = -1;

}

uint8_t Curves::getCurveValue(uint8_t gain, uint8_t index, uint8_t min, uint8_t max)    {

    if (gain != lastGain)   {

        //only copy bytes from flash if received gain differs from last one
        memcpy_P(scale, (uint8_t*)pgm_read_word(&scaleArray[gain]), sizeof(scaleArray[0]));

        lastGain = gain;

    }

    if ((min != 0) || (max != 127))    {

        if ((lastMin != min) || (lastMax != max))   {

            lastMin = min;
            lastMax = max;

            uint8_t numberOfValues = max-min;
            //scale temp array to min and max
            for (int i=min; i<=max; i++)
                scale[i] = pads.map(scale[i], 0, numberOfValues, min, max);

        }

    }

    return scale[index];

}

Curves curves;