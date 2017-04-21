#pragma once

#include "Program.h"
#include "Scales.h"
#include "PadCalibration.h"
#include "GlobalSettings.h"

typedef enum
{
    DB_BLOCK_PROGRAM,
    DB_BLOCK_SCALE,
    DB_BLOCK_PAD_CALIBRATION,
    DB_BLOCK_GLOBAL_SETTINGS,
    DB_BLOCK_ID,
    DB_BLOCKS
} dbBlocks;
