#include "Pads.h"
#include "../../database/Defaults.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

uint16_t Pads::map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t Pads::scalePressure(uint8_t pad, int16_t pressure, pressureType_t type)
{
    switch(type)
    {
        case pressureAftertouch:
        return map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return map(constrain(pressure, DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad]), DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad], 0, 127);
        break;
    }

    return 0;
}

inline uint8_t invertRange(uint8_t value, uint8_t min, uint8_t max)
{
    return (max + min) - value;
}

uint8_t Pads::scaleXY(uint8_t pad, int16_t xyValue, padCoordinate_t type)
{
    switch (type)
    {
        case coordinateX:
        #if XY_FLIP_VALUES > 0
        return invertRange(map(constrain(xyValue, padXLimitLower[pad], padXLimitUpper[pad]), padXLimitLower[pad], padXLimitUpper[pad], 0, 127), 0, 127);
        #else
        return map(constrain(xyValue, padXLimitLower[pad], padXLimitUpper[pad]), padXLimitLower[pad], padXLimitUpper[pad], 0, 127);
        #endif
        case coordinateY:
        #if XY_FLIP_VALUES > 0
        return map(constrain(xyValue, padYLimitLower[pad], padYLimitUpper[pad]), padYLimitLower[pad], padYLimitUpper[pad], ccYminPad[pad], ccYmaxPad[pad]);
        #else
        return invertRange(map(constrain(xyValue, padYLimitLower[pad], padYLimitUpper[pad]), padYLimitLower[pad], padYLimitUpper[pad], 0, 127), 0, 127);
        #endif

        default:
        return 0;
    }
}
