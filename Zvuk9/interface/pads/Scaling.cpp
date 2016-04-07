#include "Pads.h"


uint8_t Pads::scalePressure(uint8_t pad, int16_t pressure, pressureType_t type) {

    switch(type)  {

        case pressureAftertouch:
        return map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return map(constrain(pressure, padPressureLimitLower[pad], padPressureLimitUpper[pad]), padPressureLimitLower[pad], padPressureLimitUpper[pad], 0, 127);
        break;

    }

    return 0;

}

uint8_t Pads::scaleXY(uint8_t padNumber, int16_t xyValue, ccType_t type) {

    switch (type)   {

        case ccTypeX:
        return map(constrain(xyValue, padXLimitLower[padNumber], padXLimitUpper[padNumber]), padXLimitLower[padNumber], padXLimitUpper[padNumber], ccXminPad[padNumber], ccXmaxPad[padNumber]);

        case ccTypeY:
        return map(constrain(xyValue, padYLimitLower[padNumber], padYLimitUpper[padNumber]), padYLimitLower[padNumber], padYLimitUpper[padNumber], ccYminPad[padNumber], ccYmaxPad[padNumber]);

    }   return 0;

}
