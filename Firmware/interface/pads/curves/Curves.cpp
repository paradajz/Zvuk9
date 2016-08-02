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

void Curves::setupCurve(uint8_t curveGain, uint8_t min, uint8_t max)   {

    //todo

}


const uint8_t PROGMEM playIcon[] PROGMEM = {

    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11100,
    0b11000,
    0b10000,
    0b00000

};

const uint8_t PROGMEM recordIcon[] PROGMEM = {

    0b01110,
    0b10001,
    0b10101,
    0b11011,
    0b10101,
    0b10001,
    0b01110,
    0b00000

};

const uint8_t PROGMEM stopIcon[] PROGMEM = {

    0b01110,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b10001,
    0b01110,
    0b00000

};

const uint8_t* const iconArray[] PROGMEM = {

    playIcon,
    recordIcon,
    stopIcon

};


uint8_t Curves::getCurveValue(uint8_t gain, uint8_t index, uint8_t min, uint8_t max)    {

    if (gain != lastGain)   {

        //only copy bytes from flash if received gain differs from last one
        memcpy_P(scale, (uint8_t*)pgm_read_word(&scaleArray[gain/10]), sizeof(scaleArray[0]));

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