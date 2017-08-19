#include "Database.h"

#include "../interface/pads/Pads.h"

void Database::createLayout()
{
    DBMS::addBlocks(DB_BLOCKS);

    dbSection_t section;

    {
        //program block

        //programLastActiveProgramSection
        section.numberOfParameters = 1;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLastActiveScaleSection
        section.numberOfParameters = NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programGlobalSettingsSection
        section.numberOfParameters = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLocalSettingsSection
        section.numberOfParameters = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);
    }

    {
        //scale block

        //scalePredefinedSection
        section.numberOfParameters = PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);

        //scaleUserSection
        section.numberOfParameters = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);
    }

    {
        //pad calibration block

        //padCalibrationPressureUpperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = PAD_PRESSURE_UPPER_DEFAULT;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXlowerSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 1023;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXupperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYlowerSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 1023;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYupperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);
    }

    {
        //global settings block

        //globalSettingsMIDI
        section.numberOfParameters = MIDI_SETTINGS;
        section.defaultValue = 1;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);

        //globalSettingsVelocity
        section.numberOfParameters = VELOCITY_SETTINGS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);
    }

    {
        //id block
        section.numberOfParameters = ID_OFFSET;
        section.defaultValue = UNIQUE_ID;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_ID, section);
    }
}
