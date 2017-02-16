#pragma once

#ifdef BOARD_R2

#define PROGRAM_ENCODER         0
#define PRESET_ENCODER          8

#define X_CC_ENCODER            16
#define X_MIN_ENCODER           24
#define X_MAX_ENCODER           9
#define X_CURVE_ENCODER         1

#define Y_CC_ENCODER            17
#define Y_MIN_ENCODER           25
#define Y_MAX_ENCODER           10
#define Y_CURVE_ENCODER         2

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

#endif
