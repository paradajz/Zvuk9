#ifndef SECTIONS_H_
#define SECTIONS_H_

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

#endif