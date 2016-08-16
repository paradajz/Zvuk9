#ifndef BLOCKS_H_
#define BLOCKS_H_

typedef enum {

    CONF_BLOCK_PROGRAM,
    CONF_BLOCK_USER_SCALE,
    CONF_BLOCK_PAD_CALIBRATION,
    CONF_BLOCK_MIDI,
    CONF_BLOCK_PRESSURE_SETTINGS,
    CONF_BLOCKS

} blocks;


typedef enum {

    programLastActiveProgramSection,
    programLastActiveScaleSection,
    programGlobalSettingsSection,
    programLocalSettingsSection,
    programScalePredefinedSection,
    PROGRAM_SECTIONS

} programSection;

typedef enum {

    padCalibrationPressureLowerSection,
    padCalibrationPressureUpperSection,
    padCalibrationXlowerSection,
    padCalibrationXupperSection,
    padCalibrationYlowerSection,
    padCalibrationYupperSection,
    PAD_CALIBRATION_SECTIONS

} padCalibrationSection;

typedef enum {

    padNotesSection,
    USER_SCALE_SECTIONS

} userScaleSection;

typedef enum {

    pressureSensitivitySection,
    pressureCurveSection,

    PRESSURE_SETTINGS_SECTIONS

} pressureSettingsSection;

#endif