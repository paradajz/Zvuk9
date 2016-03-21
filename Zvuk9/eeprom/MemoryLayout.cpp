#include "EEPROMsettings.h"

void EEPROMsettings::createMemoryLayout() {

    //create memory layout

    {
        blocks[CONF_BLOCK_PROGRAM].sections = PROGRAM_SECTIONS;

        blocks[CONF_BLOCK_PROGRAM].resetEnabled = false;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLastActiveProgramSection] = 1;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLastActiveProgramSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLastActiveScaleSection] = NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLastActiveScaleSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programGlobalSettingsSection] = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programGlobalSettingsSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLocalSettingsSection] = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLocalSettingsSection] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programScalePredefinedSection] = PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programScalePredefinedSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_USER_SCALE].sections = USER_SCALE_SECTIONS;
        
        blocks[CONF_BLOCK_USER_SCALE].resetEnabled = false;

        blocks[CONF_BLOCK_USER_SCALE].sectionParameters[padNotesSection] = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        blocks[CONF_BLOCK_USER_SCALE].sectionParameterType[padNotesSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_PAD_CALIBRATION].sections = PAD_CALIBRATION_SECTIONS;

        blocks[CONF_BLOCK_PAD_CALIBRATION].resetEnabled = true;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationStatus] = 1;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationStatus] = BYTE_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationPressureLowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationPressureLowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationPressureUpperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationPressureUpperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationXlowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationXlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationXupperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationXupperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationYlowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationYlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameters[padCalibrationYupperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_PAD_CALIBRATION].sectionParameterType[padCalibrationYupperSection] = WORD_PARAMETER;
    }

}
