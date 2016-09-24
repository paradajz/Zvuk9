#include "Include.h"

void Configuration::initSettings(bool partialReset) {

    //we need to init each block and section with data
    //program area
    initProgramSettings(partialReset);
    initUserScales(partialReset);
    initPadCalibration(partialReset);
    initMIDIsettings(partialReset);

}

void Configuration::initProgramSettings(bool partialReset) {

    if (partialReset && blocks[CONF_BLOCK_PROGRAM].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_PROGRAM);

    uint16_t parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLastActiveProgramSection] + blockStartAddress;

    //init default active program
    eeprom_update_byte((uint8_t*)parameterAddress, DEFAULT_ACTIVE_PROGRAM);

    parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLastActiveScaleSection] + blockStartAddress;

    //init default active scales for programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    eeprom_update_byte((uint8_t*)parameterAddress+i, DEFAULT_ACTIVE_SCALE);

    //init global settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)    {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_PROGRAM].sectionAddress[programGlobalSettingsSection] + (i*GLOBAL_PROGRAM_SETTINGS);

        for (int j=0; j<GLOBAL_PROGRAM_SETTINGS; j++)
        eeprom_update_byte((uint8_t*)parameterAddress+j, defaultGlobalProgramSettingArray[j]);

    }

    //init local settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)    {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLocalSettingsSection] + ((LOCAL_PROGRAM_SETTINGS*MAX_PADS)*i);

        for (int j=0; j<MAX_PADS; j++)    {

            for (int k=0; k<LOCAL_PROGRAM_SETTINGS; k++)    {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(LOCAL_PROGRAM_SETTINGS*j), defaultLocalProgramSettingArray[k]);

            }

        }

    }

    //init predefined scale settings
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++) {

        parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programScalePredefinedSection] + ((PREDEFINED_SCALES*PREDEFINED_SCALE_PARAMETERS)*i) + blockStartAddress;

        for (int j=0; j<PREDEFINED_SCALES; j++)  {

            for (int k=0; k<PREDEFINED_SCALE_PARAMETERS; k++)   {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(PREDEFINED_SCALE_PARAMETERS*j), defaultPredefinedScaleParametersArray[k]);

            }

        }

    }

}

void Configuration::initUserScales(bool partialReset)   {

    if (partialReset && blocks[CONF_BLOCK_USER_SCALE].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_USER_SCALE);
    uint16_t parameterAddress;

    for (int i=0; i<NUMBER_OF_USER_SCALES; i++) {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_USER_SCALE].sectionAddress[padNotesSection] + ((MAX_PADS*NOTES_PER_PAD)*i);

        for (int j=0; j<MAX_PADS; j++)    {

            for (int k=0; k<NOTES_PER_PAD; k++) {

                //set first note to default note
                //set all other notes to blank

                if (!k) eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), DEFAULT_NOTE);
                else    eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), BLANK_NOTE);

            }

        }

    }

}

void Configuration::initPadCalibration(bool partialReset)   {

    if (partialReset && blocks[CONF_BLOCK_PAD_CALIBRATION].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_PAD_CALIBRATION);
    uint16_t parameterAddress;

    //init lower pressure limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationPressureLowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);

    }

    //init upper pressure limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationPressureUpperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

    }

    //init lower x limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationXlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_LOWER);

    }

    //init upper x limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationXupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_UPPER);

    }

    //init lower y limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationYlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_LOWER);

    }

    //init upper y limits
    for (int i=0; i<MAX_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_PAD_CALIBRATION].sectionAddress[padCalibrationYupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_UPPER);

    }

}

void Configuration::initMIDIsettings(bool partialReset) {

    if (partialReset && blocks[CONF_BLOCK_MIDI].preserveOnPartialReset) return;

    uint16_t blockStartAddress = getBlockAddress(CONF_BLOCK_MIDI);
    uint16_t parameterAddress = blockStartAddress;

    for (int i=0; i<MIDI_SETTINGS; i++)
    eeprom_update_byte((uint8_t*)parameterAddress+i, defaultMIDIsettingArray[i]);

}
