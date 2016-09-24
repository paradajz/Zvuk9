#ifndef PADXYSCALES_H_
#define PADXYSCALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "../DataTypes.h"

#define CURVE_STEPS 9

class Curves {

    public:
    Curves();
    void init();
    uint8_t getCurveValue(coordinateType_t coordinate, uint8_t gain, uint8_t index, uint8_t min, uint8_t max);

    private:
    void setupCurve(uint8_t curveGain, uint8_t min, uint8_t max);
    uint8_t scale[128];
    int8_t  lastGain[2],
            lastMin[2],
            lastMax[2];

};

extern Curves curves;

#endif