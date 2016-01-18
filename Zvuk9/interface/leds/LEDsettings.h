#ifndef LEDSETTINGS_H_
#define LEDSETTINGS_H_

#define NUMBER_OF_TRANSITIONS       64
#define DEFAULT_FADE_SPEED          15

#define NUMBER_OF_LEDS              24

const uint8_t ledTransitionScale[] = {

    0,
    2,
    4,
    6,
    8,
    10,
    12,
    14,
    16,
    18,
    20,
    22,
    24,
    26,
    28,
    30,
    32,
    34,
    36,
    38,
    40,
    42,
    44,
    46,
    48,
    50,
    52,
    54,
    56,
    58,
    60,
    70, //half intensity
    72,
    74,
    75,
    78,
    80,
    85,
    90,
    95,
    100,
    105,
    110,
    115,
    120,
    125,
    130,
    135,
    140,
    145,
    150,
    155,
    160,
    165,
    170,
    180,
    190,
    200,
    210,
    220,
    230,
    240,
    250,
    255 //full intensity

};

#endif /* LEDSETTINGS_H_ */