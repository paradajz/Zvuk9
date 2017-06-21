#pragma once

#define DEFAULT_PAD_PRESSURE_LIMIT_UPPER            380

#define DEFAULT_PAD_X_LIMIT_LOWER                   150
#define DEFAULT_PAD_X_LIMIT_UPPER                   850

#define DEFAULT_PAD_Y_LIMIT_LOWER                   100
#define DEFAULT_PAD_Y_LIMIT_UPPER                   900

typedef enum
{
    padCalibrationPressureUpperSection0,
    padCalibrationPressureUpperSection1,
    padCalibrationPressureUpperSection2,
    padCalibrationPressureUpperSection3,
    padCalibrationPressureUpperSection4,
    padCalibrationPressureUpperSection5,
    padCalibrationPressureUpperSection6,
    padCalibrationPressureUpperSection7,
    padCalibrationPressureUpperSection8,
    padCalibrationXlowerSection,
    padCalibrationXupperSection,
    padCalibrationYlowerSection,
    padCalibrationYupperSection,
    PAD_CALIBRATION_SECTIONS
} padCalibrationSection;