#pragma once

#include "../pins/Pins.h"

#define PROGRAM_ENCODER         0
#define PRESET_ENCODER          1

#define X_CC_ENCODER            8
#define X_MAX_ENCODER           3
#define X_MIN_ENCODER           6
#define X_CURVE_ENCODER         2

#define Y_CC_ENCODER            7
#define Y_MAX_ENCODER           9
#define Y_MIN_ENCODER           5
#define Y_CURVE_ENCODER         4

#ifdef BOARD_R1
const uint8_t encoderPinIndex1Array[] =
{
    ENCODER_00_PIN_0_PIN,
    ENCODER_01_PIN_0_PIN,
    ENCODER_02_PIN_0_PIN,
    ENCODER_03_PIN_0_PIN,
    ENCODER_04_PIN_0_PIN,
    ENCODER_05_PIN_0_PIN,
    ENCODER_06_PIN_0_PIN,
    ENCODER_07_PIN_0_PIN,
    ENCODER_08_PIN_0_PIN,
    ENCODER_09_PIN_0_PIN
};

const uint8_t encoderPinIndex2Array[] =
{
    ENCODER_00_PIN_1_PIN,
    ENCODER_01_PIN_1_PIN,
    ENCODER_02_PIN_1_PIN,
    ENCODER_03_PIN_1_PIN,
    ENCODER_04_PIN_1_PIN,
    ENCODER_05_PIN_1_PIN,
    ENCODER_06_PIN_1_PIN,
    ENCODER_07_PIN_1_PIN,
    ENCODER_08_PIN_1_PIN,
    ENCODER_09_PIN_1_PIN
};
#endif
