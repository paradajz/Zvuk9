#ifndef PADXYSCALES_H_
#define PADXYSCALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "../../../Types.h"

#define CURVE_STEPS 10

class Curves {

    public:
    Curves();
    void init();
    uint8_t getCurveValue(uint8_t gain, uint8_t index, uint8_t min, uint8_t max);

    private:
    uint8_t scale[128];
    int8_t  lastGain,
            lastMin,
            lastMax;

};

extern Curves curves;

#endif