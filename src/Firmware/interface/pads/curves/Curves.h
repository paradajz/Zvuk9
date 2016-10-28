#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "../DataTypes.h"

typedef enum
{
    curveTypeLinear,
    curveTypeWideMiddle,
    curveTypeWideEnds,
    NUMBER_OF_CC_CURVES,
    curveTypeLog = NUMBER_OF_CC_CURVES,
    curveTypeExp = curveTypeLog+1,
    NUMBER_OF_CURVES = curveTypeExp+1
} curveType_t;

class Curves
{

    public:
    Curves();
    void init();
    uint8_t getCurveValue(coordinateType_t coordinate, curveType_t type, uint8_t index, uint8_t min, uint8_t max);

    private:
    uint8_t scale[3][128]; //x+y+z
    int8_t  lastCurve[3], //x+y+z
            lastMin[2], //x+y
            lastMax[2]; //x+y
};

extern Curves curves;
