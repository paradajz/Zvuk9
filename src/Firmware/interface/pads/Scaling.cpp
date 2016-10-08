#include "Pads.h"


const uint8_t gamma_15[] = {

    0,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    3,
    3,
    3,
    4,
    4,
    5,
    5,
    6,
    6,
    7,
    7,
    8,
    8,
    9,
    10,
    10,
    11,
    12,
    12,
    13,
    14,
    14,
    15,
    16,
    17,
    17,
    18,
    19,
    20,
    21,
    21,
    22,
    23,
    24,
    25,
    26,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    62,
    63,
    64,
    65,
    66,
    68,
    69,
    70,
    71,
    73,
    74,
    75,
    76,
    78,
    79,
    80,
    81,
    83,
    84,
    85,
    87,
    88,
    89,
    91,
    92,
    93,
    95,
    96,
    97,
    99,
    100,
    101,
    103,
    104,
    106,
    107,
    109,
    110,
    111,
    113,
    114,
    116,
    117,
    119,
    120,
    122,
    123,
    124,
    126,
    127

};


uint32_t Pads::map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)   {

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}

uint8_t Pads::scalePressure(uint8_t pad, int16_t pressure, pressureType_t type) {

    switch(type)  {

        case pressureAftertouch:
        return gamma_15[map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127)];
        //return map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return gamma_15[map(constrain(pressure, padPressureLimitLower[pad], padPressureLimitUpper[pad]), padPressureLimitLower[pad], padPressureLimitUpper[pad], 0, 127)];
        break;

    }

    return 0;

}

inline uint8_t invertRange(uint8_t value, uint8_t min, uint8_t max) {

    return (max + min) - value;

}

uint8_t Pads::scaleXY(uint8_t pad, int16_t xyValue, coordinateType_t type) {

    switch (type)   {

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