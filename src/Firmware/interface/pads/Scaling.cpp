#include "Pads.h"
#include "../../database/blocks/PadCalibration.h"

uint8_t Pads::scalePressure(uint8_t pad, int16_t pressure, pressureType_t type)
{
    switch(type)
    {
        case pressureAftertouch:
        return curves.map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return curves.map(constrain(pressure, DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad]), DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad], 0, 127);
        break;
    }

    return 0;
}

uint8_t Pads::scaleXY(uint8_t pad, int16_t xyValue, padCoordinate_t type)
{
    switch (type)
    {
        case coordinateX:
        return curves.map(constrain(xyValue, padXLimitLower[pad], padXLimitUpper[pad]), padXLimitLower[pad], padXLimitUpper[pad], 0, 127);

        case coordinateY:
        return curves.invertRange(curves.map(constrain(xyValue, padYLimitLower[pad], padYLimitUpper[pad]), padYLimitLower[pad], padYLimitUpper[pad], ccYminPad[pad], ccYmaxPad[pad]), 0, 127);

        default:
        return 0;
    }
}
