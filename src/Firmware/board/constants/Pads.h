#pragma once

#include "Timer.h"
#include "Hardware.h"
#include "../DataTypes.h"

#define PAD_READOUT_TIME    (int)(CORE_TIMER_RUN_TIME*(float)NUMBER_OF_PADS*(float)PAD_READINGS+(float)1.0)