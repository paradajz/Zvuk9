#include "Database.h"

#include "../interface/pads/Pads.h"

void Database::writeCustomValues()
{
    //we need to init each block and section with data
    initProgramSettings();
    initScales();
    initPadCalibration();
    initGlobalSettings();
}

void Database::initProgramSettings()
{
    //init default active program
    //programLastActiveProgramSection only has 1 parameter - index 0
    database.update(DB_BLOCK_PROGRAM, programLastActiveProgramSection, 0, DEFAULT_ACTIVE_PROGRAM);

    //init default active scales for programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
        database.update(DB_BLOCK_PROGRAM, programLastActiveScaleSection, i, DEFAULT_ACTIVE_SCALE);

    //init global settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<GLOBAL_PROGRAM_SETTINGS; j++)
            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, j+(GLOBAL_PROGRAM_SETTINGS*i), defaultGlobalProgramSettingArray[j]);
    }

    //init local settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<NUMBER_OF_PADS; j++)
        {
            for (int k=0; k<LOCAL_PROGRAM_SETTINGS; k++)
            {
                database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*j+k) + (LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*i), defaultLocalProgramSettingArray[k]);
            }
        }
    }
}

void Database::initScales()
{
    //init predefined scale settings
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<PREDEFINED_SCALES; j++)
        {
            for (int k=0; k<PREDEFINED_SCALE_PARAMETERS; k++)
            {
                database.update(DB_BLOCK_SCALE, scalePredefinedSection, (PREDEFINED_SCALE_PARAMETERS*j+k) + (PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*i), defaultPredefinedScaleParametersArray[k]);
            }
        }
    }

    //init user scale settings
    for (int i=0; i<NUMBER_OF_USER_SCALES; i++)
    {
        for (int j=0; j<NUMBER_OF_PADS; j++)
        {
            //all blank notes
            for (int k=0; k<NOTES_PER_PAD; k++)
                database.update(DB_BLOCK_SCALE, scaleUserSection, (NOTES_PER_PAD*j+k) + (NOTES_PER_PAD*NUMBER_OF_PADS*i), BLANK_NOTE);
        }
    }
}

void Database::initPadCalibration()
{
    //init lower pressure limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureLowerSection, i, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);

    //init upper pressure limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, i, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

    //init lower x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, i, DEFAULT_PAD_X_LIMIT_LOWER);

    //init upper x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, i, DEFAULT_PAD_X_LIMIT_UPPER);

    //init lower y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, i, DEFAULT_PAD_Y_LIMIT_LOWER);

    //init upper y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, i, DEFAULT_PAD_Y_LIMIT_UPPER);
}

void Database::initGlobalSettings()
{
    //init midi settings
    for (int i=0; i<MIDI_SETTINGS; i++)
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, i, defaultMIDIsettingArray[i]);

    //init pressure settings
    for (int i=0; i<PRESSURE_SETTINGS; i++)
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsPressure, i, defaultPressureSettingsArray[i]);
}