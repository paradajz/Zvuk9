#include "Include.h"

void Configuration::createMemoryLayout() {

    {
        blocks[CONF_BLOCK_PROGRAM].sections = PROGRAM_SECTIONS;

        blocks[CONF_BLOCK_PROGRAM].preserveOnPartialReset = false;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLastActiveProgramSection] = 1;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLastActiveProgramSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLastActiveScaleSection] = NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLastActiveScaleSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programGlobalSettingsSection] = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programGlobalSettingsSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLocalSettingsSection] = LOCAL_PROGRAM_SETTINGS*MAX_PADS*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLocalSettingsSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programScalePredefinedSection] = PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programScalePredefinedSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_USER_SCALE].sections = USER_SCALE_SECTIONS;

        blocks[CONF_BLOCK_USER_SCALE].preserveOnPartialReset = false;

        blocks[CONF_BLOCK_USER_SCALE].sectionParameters[padNotesSection] = MAX_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        blocks[CONF_BLOCK_USER_SCALE].sectionParameterType[padNotesSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_PAD_CALIBRATION].sections = PAD_CALIBRATION_SECTIONS;

        blocks[CONF_BLOCK_PAD_CALIBRATION].preserveOnPartialReset = true;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationPressureLowerSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationPressureLowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationPressureUpperSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationPressureUpperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationXlowerSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationXlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationXupperSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationXupperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationYlowerSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationYlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationYupperSection] = MAX_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationYupperSection] = WORD_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_MIDI].sections = 1;

        blocks[CONF_BLOCK_MIDI].preserveOnPartialReset = false;

        blocks[CONF_BLOCK_MIDI].sectionParameters[0] = MIDI_SETTINGS;
        blocks[CONF_BLOCK_MIDI].sectionParameterType[0] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_PRESSURE_SETTINGS].sections = PRESSURE_SETTINGS_SECTIONS;

        blocks[CONF_BLOCK_PRESSURE_SETTINGS].preserveOnPartialReset = false;

        blocks[CONF_BLOCK_PRESSURE_SETTINGS].sectionParameters[pressureSensitivitySection] = 1;
        blocks[CONF_BLOCK_PRESSURE_SETTINGS].sectionParameterType[pressureSensitivitySection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PRESSURE_SETTINGS].sectionParameters[pressureCurveSection] = 1;
        blocks[CONF_BLOCK_PRESSURE_SETTINGS].sectionParameterType[pressureCurveSection] = BYTE_PARAMETER;

    }

}
