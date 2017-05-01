#pragma once

#define DEFAULT_PAD_PRESSURE_LIMIT_LOWER            40
#define DEFAULT_PAD_PRESSURE_LIMIT_UPPER            380

#define DEFAULT_PAD_X_LIMIT_LOWER                   150
#define DEFAULT_PAD_X_LIMIT_UPPER                   850

#define DEFAULT_PAD_Y_LIMIT_LOWER                   100
#define DEFAULT_PAD_Y_LIMIT_UPPER                   900

typedef enum
{
    padCalibrationPressureLowerSection,
    padCalibrationPressureUpperSection,
    padCalibrationXlowerSection,
    padCalibrationXupperSection,
    padCalibrationYlowerSection,
    padCalibrationYupperSection,
    PAD_CALIBRATION_SECTIONS
} padCalibrationSection;