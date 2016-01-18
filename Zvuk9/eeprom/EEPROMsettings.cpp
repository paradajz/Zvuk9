#ifndef EEPROMSETTINGS_H_
#define EEPROMSETTINGS_H_

#include "EEPROMsettings.h"

EEPROMsettings::EEPROMsettings()    {
    
    //def const
    
}

void EEPROMsettings::init() {

    createMemoryLayout();
    createSectionAddresses();

    //if ID bytes haven't been written to EEPROM on specified address,
    //write default configuration to EEPROM
    if  (!(

    (eeprom_read_byte((uint8_t*)UNIQUE_ID_LOCATION_0) == EEPROM_UNIQUE_ID) &&
    (eeprom_read_byte((uint8_t*)UNIQUE_ID_LOCATION_1) == EEPROM_UNIQUE_ID) &&
    (eeprom_read_byte((uint8_t*)UNIQUE_ID_LOCATION_2) == EEPROM_UNIQUE_ID)

    ))  factoryReset();

}

void EEPROMsettings::factoryReset()  {

    clearEEPROM();
    writeConfiguration();

}

void EEPROMsettings::clearEEPROM()  {

    for (int i=0; i<EEPROM_SIZE; i++) eeprom_update_byte((uint8_t*)i, 0xFF);

}

void EEPROMsettings::writeConfiguration()   {

    lcd_set_cursor(0, 0);
    lcd_puts("Factory reset in");

    for (int i=3; i>=1; i--) {

        lcd_set_cursor(17, 0);
        lcd_putc(i+48);   //convert int to char, +48
        newDelay(1000);

    }

    lcd_clrscr();
    lcd_set_cursor(0, 0);
    lcd_puts("Factory reset...");
    lcd_set_cursor(0, 1);
    lcd_puts("Wiping memory");

    lcd_set_cursor(0, 1);
    lcd_puts("Restoring defaults");

    //write default configuration stored in PROGMEM to EEPROM
    initSettings();

    lcd_clrscr();
    lcd_set_cursor(0,0);
    lcd_puts("Factory reset");
    lcd_set_cursor(0, 1);
    lcd_puts("finished!");

    newDelay(2000);

}

void EEPROMsettings::createMemoryLayout() {

    //create memory layout

    {
        blocks[CONF_BLOCK_PROGRAM].sections = PROGRAM_SECTIONS;

        blocks[CONF_BLOCK_PROGRAM].sectionParameters[programLastActiveProgramSection] = 1;
        blocks[CONF_BLOCK_PROGRAM].sectionParameterType[programLastActiveScaleSection] = BYTE_PARAMETER;

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

        blocks[CONF_BLOCK_USER_SCALE].sectionParameters[padNotesSection] = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        blocks[CONF_BLOCK_USER_SCALE].sectionParameterType[padNotesSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_LIMIT].sections = LIMIT_SECTIONS;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitPressureLower] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitPressureLower] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitPressureUpper] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitPressureUpper] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitXlower] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitXlower] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitXupper] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitXupper] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitYlower] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitYlower] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitYupper] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitYupper] = WORD_PARAMETER;
    }

}

void EEPROMsettings::createSectionAddresses()   {

    Serial.println();
    Serial.println("We are now creating EEPROM memory layout");
    Serial.println();

    for (int i=0; i<CONF_BLOCKS; i++)  {

        Serial.print("Creating block ");
        Serial.println(i);

        uint16_t memory_usage = 0;

        for (int j=0; j<blocks[i].sections; j++)    {

            Serial.print("Section ");
            Serial.print(j);
            Serial.print(", address: ");

            if (!j) {

                //first section address is always 0
                blocks[i].sectionAddress[0] = 0;

                }   else {

                switch(blocks[i].sectionParameterType[j-1])   {

                    case BIT_PARAMETER:
                    blocks[i].sectionAddress[j] = ((blocks[i].sectionParameters[j]%8 != 0) + blocks[i].sectionParameters[j-1]/8) + blocks[i].sectionAddress[j-1];
                    break;

                    case BYTE_PARAMETER:
                    blocks[i].sectionAddress[j] = blocks[i].sectionParameters[j-1] + blocks[i].sectionAddress[j-1];
                    break;

                    case WORD_PARAMETER:
                    blocks[i].sectionAddress[j] = 2*blocks[i].sectionParameters[j-1] + blocks[i].sectionAddress[j-1];
                    break;

                }

            } Serial.println(blocks[i].sectionAddress[j]);

        }

        uint8_t lastSection = blocks[i].sections-1;

        switch(blocks[i].sectionParameterType[lastSection]) {

            case BIT_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+((blocks[i].sectionParameters[lastSection]%8 != 0)+blocks[i].sectionParameters[lastSection]/8);
            break;

            case BYTE_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+blocks[i].sectionParameters[lastSection];
            break;

            case WORD_PARAMETER:
            memory_usage = blocks[i].sectionAddress[lastSection]+2*blocks[i].sectionParameters[lastSection];
            break;

        }

        Serial.print("Total memory usage: "); Serial.print(memory_usage); Serial.println(" bytes"); Serial.println(); //total memory usage here

        if (i < CONF_BLOCKS-1) {

            blocks[i+1].blockStartAddress = blocks[i].blockStartAddress + memory_usage;

        }

    }

    Serial.println("----------------------------------------");

    for (int i=0; i<CONF_BLOCKS; i++)  {

        Serial.print("Block ");
        Serial.print(i);
        Serial.print(" start address: ");
        Serial.println(blocks[i].blockStartAddress);

    } Serial.println("----------------------------------------");

}

void EEPROMsettings::initSettings() {

    //we need to init each block and section with data
    //program area
    initProgramSettings();
    initUserScales();
    initLimits();
    while(1) {}

}

void EEPROMsettings::initProgramSettings() {

    uint16_t blockStartAddress = blocks[CONF_BLOCK_PROGRAM].blockStartAddress;

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

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_PROGRAM].sectionAddress[programLocalSettingsSection] + ((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*i);

        for (int j=0; j<NUMBER_OF_PADS; j++)    {

            for (int k=0; k<LOCAL_PROGRAM_SETTINGS; k++)    {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(LOCAL_PROGRAM_SETTINGS*j), defaultLocalProgramSettingArray[k]);

            }

        }

    }

    //init predefined scale settings
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++) {

        parameterAddress = blocks[CONF_BLOCK_PROGRAM].sectionAddress[programScalePredefinedSection] + ((NUMBER_OF_PREDEFINED_SCALES*PREDEFINED_SCALE_PARAMETERS)*i) + blockStartAddress;

        for (int j=0; j<NUMBER_OF_PREDEFINED_SCALES; j++)  {

            for (int k=0; k<PREDEFINED_SCALE_PARAMETERS; k++)   {

                eeprom_update_byte((uint8_t*)parameterAddress+k+(PREDEFINED_SCALE_PARAMETERS*j), defaultPredefinedScaleParametersArray[k]);

            }

        }

    }

}

void EEPROMsettings::initUserScales()   {

    uint16_t blockStartAddress = blocks[CONF_BLOCK_USER_SCALE].blockStartAddress;
    uint16_t parameterAddress;

    for (int i=0; i<NUMBER_OF_USER_SCALES; i++) {

        parameterAddress = blockStartAddress + blocks[CONF_BLOCK_USER_SCALE].sectionAddress[padNotesSection] + ((NUMBER_OF_PADS*NOTES_PER_PAD)*i);

        for (int j=0; j<NUMBER_OF_PADS; j++)    {

            for (int k=0; k<NOTES_PER_PAD; k++) {

                //set first note to default note
                //set all other notes to blank

                if (!k) eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), DEFAULT_NOTE);
                else    eeprom_update_byte((uint8_t*)parameterAddress+k+(NOTES_PER_PAD*j), BLANK_NOTE);

            }

        }

    }

}

void EEPROMsettings::initLimits()   {

    uint16_t blockStartAddress = blocks[CONF_BLOCK_LIMIT].blockStartAddress;
    uint16_t parameterAddress;

    //init lower pressure limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitPressureLower] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);

    }

    //init upper pressure limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitPressureUpper] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

    }

    //init lower x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitXlower] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_LOWER);

    }

    //init upper x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitXupper] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_UPPER);

    }

    //init lower y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitYlower] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_LOWER);

    }

    //init upper y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitYupper] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_UPPER);

    }

}

EEPROMsettings configuration;

#endif