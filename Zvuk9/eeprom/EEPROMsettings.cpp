#ifndef EEPROMSETTINGS_H_
#define EEPROMSETTINGS_H_

#include "EEPROMsettings.h"
#include <util/delay.h>

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

    #if MODE_SERIAL > 0
        Serial.println(F("EEPROM memory readout"));
        Serial.println(F("----------------------------------------"));
        for (int i=0; i<4096; i++)  {

            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(eeprom_read_byte((uint8_t*)i));

        }   Serial.println(F("----------------------------------------"));
    #endif

}

void EEPROMsettings::clearEEPROM()  {

    for (int i=0; i<EEPROM_SIZE; i++) eeprom_update_byte((uint8_t*)i, 0xFF);

}

void EEPROMsettings::factoryReset()   {

    lcd_set_cursor(0, 0);
    lcd_puts("Factory reset in");

    for (int i=3; i>=1; i--) {

        lcd_set_cursor(17, 0);
        lcd_putc(i+48);   //convert int to char, +48
        _delay_ms(1000);

    }

    lcd_clrscr();
    lcd_set_cursor(0, 0);
    lcd_puts("Factory reset...");
    lcd_set_cursor(0, 1);
    lcd_puts("Wiping memory");

    clearEEPROM();

    lcd_set_cursor(0, 1);
    lcd_puts("Restoring defaults");

    //write default configuration stored in PROGMEM to EEPROM
    initSettings();

    lcd_clrscr();
    lcd_set_cursor(0,0);
    lcd_puts("Factory reset");
    lcd_set_cursor(0, 1);
    lcd_puts("finished!");

    _delay_ms(2000);

}

void EEPROMsettings::createMemoryLayout() {

    //create memory layout

    {
        blocks[CONF_BLOCK_PROGRAM].sections = PROGRAM_SECTIONS;

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

        blocks[CONF_BLOCK_USER_SCALE].sectionParameters[padNotesSection] = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        blocks[CONF_BLOCK_USER_SCALE].sectionParameterType[padNotesSection] = BYTE_PARAMETER;
    }

    {
        blocks[CONF_BLOCK_LIMIT].sections = LIMIT_SECTIONS;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitPressureLowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitPressureLowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitPressureUpperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitPressureUpperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitXlowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitXlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitXupperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitXupperSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitYlowerSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitYlowerSection] = WORD_PARAMETER;

        blocks[CONF_BLOCK_LIMIT].sectionParameters[limitYupperSection] = NUMBER_OF_PADS;
        blocks[CONF_BLOCK_LIMIT].sectionParameterType[limitYupperSection] = WORD_PARAMETER;
    }

}

void EEPROMsettings::createSectionAddresses()   {

    #if MODE_SERIAL > 0
        Serial.println(F("We are now creating EEPROM memory layout"));
        Serial.println();
    #endif

    for (int i=0; i<CONF_BLOCKS; i++)  {

        #if MODE_SERIAL > 0
            Serial.print(F("Creating block "));
            Serial.println(i);
        #endif

        uint16_t memory_usage = 0;

        for (int j=0; j<blocks[i].sections; j++)    {

            #if MODE_SERIAL > 0
                Serial.print(F("Section "));
                Serial.print(j);
                Serial.print(F(", address: "));
            #endif

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

            }

            #if MODE_SERIAL > 0
                Serial.println(blocks[i].sectionAddress[j]);
            #endif

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

        #if MODE_SERIAL > 0
            Serial.print(F("Total memory usage: "));
            Serial.print(memory_usage);
            Serial.println(F(" bytes"));
            Serial.println();
        #endif

        if (i < CONF_BLOCKS-1) {

            blocks[i+1].blockStartAddress = blocks[i].blockStartAddress + memory_usage;

        }

    }

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));

        for (int i=0; i<CONF_BLOCKS; i++)  {

            Serial.print("Block ");
            Serial.print(i);
            Serial.print(" start address: ");
            Serial.println(blocks[i].blockStartAddress);

        } Serial.println(F("----------------------------------------"));
    #endif

}

void EEPROMsettings::initSettings() {

    //we need to init each block and section with data
    //program area
    initProgramSettings();
    initUserScales();
    initLimits();

    eeprom_update_byte((uint8_t*)UNIQUE_ID_LOCATION_0, EEPROM_UNIQUE_ID);
    eeprom_update_byte((uint8_t*)UNIQUE_ID_LOCATION_1, EEPROM_UNIQUE_ID);
    eeprom_update_byte((uint8_t*)UNIQUE_ID_LOCATION_2, EEPROM_UNIQUE_ID);

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

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitPressureLowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);

    }

    //init upper pressure limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitPressureUpperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

    }

    //init lower x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitXlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_LOWER);

    }

    //init upper x limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitXupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_X_LIMIT_UPPER);

    }

    //init lower y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitYlowerSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_LOWER);

    }

    //init upper y limits
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        parameterAddress = blockStartAddress+blocks[CONF_BLOCK_LIMIT].sectionAddress[limitYupperSection] + i*2;
        eeprom_update_word((uint16_t*)parameterAddress, DEFAULT_PAD_Y_LIMIT_UPPER);

    }

}

bool EEPROMsettings::writeParameter(uint8_t blockID, uint8_t sectionID, int16_t parameterID, int16_t newValue)    {

    uint16_t startAddress = getSectionAddress(blockID, sectionID);
    uint8_t parameterType = getParameterType(blockID, sectionID);

    uint8_t arrayIndex;
    uint8_t arrayValue;
    uint8_t parameterIndex;

    switch(parameterType)   {

        case BIT_PARAMETER:
        arrayIndex = parameterID/8;
        parameterIndex = parameterID - 8*arrayIndex;
        arrayValue = eeprom_read_byte((uint8_t*)startAddress+arrayIndex);
        bitWrite(arrayValue, parameterIndex, newValue);
        eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
        return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));
        break;

        case BYTE_PARAMETER:
        eeprom_update_byte((uint8_t*)startAddress+parameterID, newValue);
        return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterID));
        break;

        case WORD_PARAMETER:
        eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
        return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterID));
        break;

    }   return 0;

}

EEPROMsettings configuration;

#endif