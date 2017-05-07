#pragma once

#define PROGRAM_ENCODER         0
#define X_CURVE_ENCODER         1
#define Y_CURVE_ENCODER         2
#define PRESET_ENCODER          8
#define X_MAX_ENCODER           9
#define Y_MAX_ENCODER           10
#define X_CC_ENCODER            16
#define Y_CC_ENCODER            17
#define X_MIN_ENCODER           24
#define Y_MIN_ENCODER           25

const uint8_t encoderMap[] =
{
    PROGRAM_ENCODER,
    PRESET_ENCODER,
    X_CC_ENCODER,
    X_MIN_ENCODER,
    X_MAX_ENCODER,
    X_CURVE_ENCODER,
    Y_CC_ENCODER,
    Y_MIN_ENCODER,
    Y_MAX_ENCODER,
    Y_CURVE_ENCODER
};

const uint8_t pulsesPerStep[] =
{
    2,  //program enc
    4,  //x curve enc
    4,  //y curve enc
    0,
    0,
    0,
    0,
    0,
    2,  //preset enc
    4,  //x max enc
    4,  //y max enc
    0,
    0,
    0,
    0,
    0,
    4,  //x cc enc
    4,  //y cc enc
    0,
    0,
    0,
    0,
    0,
    0,
    4,  //x min enc
    4,  //y min enc
    0,
    0,
    0,
    0,
    0,
    0,
};

const uint8_t encInverted[] =
{

    1,  //program enc
    1,  //x curve enc
    1,  //y curve enc
    0,
    0,
    0,
    0,
    0,
    1,  //preset enc
    1,  //x max enc
    1,  //y max enc
    0,
    0,
    0,
    0,
    0,
    0,  //x cc enc
    0,  //y cc enc
    0,
    0,
    0,
    0,
    0,
    0,
    0,  //x min enc
    0,  //y min enc
    0,
    0,
    0,
    0,
    0,
    0,
};
