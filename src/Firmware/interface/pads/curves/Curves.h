#ifndef PADXYSCALES_H_
#define PADXYSCALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "../DataTypes.h"

#define NUMBER_OF_CURVES 3

class Curves {

    public:
    Curves();
    void init();
    uint8_t getCurveValue(coordinateType_t coordinate, curveType_t gain, uint8_t index, uint8_t min, uint8_t max);

    private:
    void setupCurve(curveType_t type, uint8_t min, uint8_t max);
    void setupLinearCurve();
    uint8_t scale[128];
    int8_t  lastGain[2], //x+y
            lastMin[2], //x+y
            lastMax[2]; //x+y

};

extern Curves curves;

#endif