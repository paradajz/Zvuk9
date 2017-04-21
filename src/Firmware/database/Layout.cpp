#include "Database.h"

#include "../interface/pads/Pads.h"

void Database::createLayout()
{
    DBMS::addBlocks(DB_BLOCKS);

    dbSection_t section;

    {
        //program block

        //programLastActiveProgramSection
        section.parameters = 1;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLastActiveScaleSection
        section.parameters = NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programGlobalSettingsSection
        section.parameters = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLocalSettingsSection
        section.parameters = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);
    }

    {
        //scale block

        //scalePredefinedSection
        section.parameters = PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);

        //scaleUserSection
        section.parameters = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);
    }

    {
        //pad calibration block

        //padCalibrationPressureLowerSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationPressureUpperSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXlowerSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXupperSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYlowerSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYupperSection
        section.parameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);
    }

    {
        //global settings block

        //globalSettingsMIDI
        section.parameters = MIDI_SETTINGS;
        section.defaultValue = 1;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);

        //globalSettingsPressure
        section.parameters = PRESSURE_SETTINGS;
        section.defaultValue = 0;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);
    }

    {
        //id block
        section.parameters = ID_OFFSET;
        section.defaultValue = UNIQUE_ID;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_ID, section);
    }
}
