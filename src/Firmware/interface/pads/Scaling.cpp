#include "Pads.h"
#include "../../database/blocks/PadCalibration.h"

uint8_t Pads::scalePressure(uint8_t pad, uint16_t pressure, padCalibrationSection pressureZone, pressureType_t type)
{
    switch(type)
    {
        case pressureAftertouch:
        return curves.map(constrain(pressure, padAftertouchLimitLower[pad][pressureZone], padAftertouchLimitUpper[pad][pressureZone]), padAftertouchLimitLower[pad][pressureZone], padAftertouchLimitUpper[pad][pressureZone], 0, 127);
        break;

        case pressureVelocity:
        return curves.map(constrain(pressure, DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad][pressureZone]), DEFAULT_PAD_PRESSURE_LIMIT_LOWER, padPressureLimitUpper[pad][pressureZone], 0, 127);
        break;
    }

    return 0;
}

uint16_t Pads::scaleXY(uint8_t pad, uint16_t xyValue, padCoordinate_t type, bool midiScale)
{
    switch (type)
    {
        case coordinateX:
        return curves.map(constrain(xyValue, padXLimitLower[pad], padXLimitUpper[pad]), padXLimitLower[pad], padXLimitUpper[pad], 0, midiScale ? 127 : 1023);

        case coordinateY:
        return curves.invertRange(curves.map(constrain(xyValue, padYLimitLower[pad], padYLimitUpper[pad]), padYLimitLower[pad], padYLimitUpper[pad], 0, midiScale ? 127 : 1023), 0, midiScale ? 127 : 1023);

        default:
        return 0;
    }
}
