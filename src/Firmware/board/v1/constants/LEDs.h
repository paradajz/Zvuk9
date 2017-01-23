#pragma once

#define NUMBER_OF_LED_TRANSITIONS           64
#define LED_ACTIVE_BIT                      0x00
#define LED_CONSTANT_ON_BIT                 0x01
#define LED_BLINK_ON_BIT                    0x02
#define LED_BLINK_STATE_BIT                 0x03
#define LED_INTENSITY_BIT                   0x04

#define DEFAULT_FADE_SPEED                  3

#define LED_NO_INTENSITY                    0
#define LED_FULL_INTENSITY                  NUMBER_OF_LED_TRANSITIONS-1
#define LED_HALF_INTENSITY                  55

//#define LED_INVERT

const uint8_t ledTransitionScale[] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    3,
    3,
    4,
    4,
    5,
    5,
    6,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    16,
    17,
    19,
    21,
    23,
    25,
    28,
    30,
    33,
    36,
    40,
    44,
    48,
    52,
    57,
    62,
    68,
    74,
    81,
    89,
    97,
    106,
    115,
    126,
    138,
    150,
    164,
    179,
    195,
    213,
    232,
    255
};