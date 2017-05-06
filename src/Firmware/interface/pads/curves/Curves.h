#pragma once

#include "../DataTypes.h"

typedef enum
{
    curveLinear,
    curveWideMiddle,
    curveWideEnds,
    NUMBER_OF_CC_CURVES,
    curveLog = NUMBER_OF_CC_CURVES,
    curveExp = curveLog+1,
    NUMBER_OF_CURVES = curveExp+1
} curve_t;

#define LOG_05  (double)log((double)0.5)

double bias(double b, double x);
double gain(double g, double x);
double gamma(double g, double x);

class Curves
{

    public:
    Curves();
    void init();
    uint8_t getCurveValue(padCoordinate_t coordinate, curve_t type, uint8_t index, uint8_t min, uint8_t max);

    private:
    uint8_t scale[PAD_COORDINATES][128]; //x+y+z
    uint8_t lastCurve[PAD_COORDINATES], //x+y+z
            lastMin[PAD_COORDINATES], //x+y
            lastMax[PAD_COORDINATES]; //x+y
};

extern Curves curves;
