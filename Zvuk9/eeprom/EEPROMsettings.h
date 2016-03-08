/*

File contains default settings for controller, as well as
EEPROM addresses of parameters.

 */

#include <stdio.h> 
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "UniqueID.h"
#include "midi/MIDI_parameters.h"
#include "hardware/lcd/hd44780.h"
#include "interface/lcd/LCDstrings.h"
#include "hardware/timer/TimerObject.h"
#include "Defaults.h"
#include "interface/pads/Pads.h"
#include "Scales.h"
#include "Blocks.h"
#include "Sections.h"

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_SIZE                             4096

//parameters

#define NUMBER_OF_PREDEFINED_SCALES             7
#define NUMBER_OF_USER_SCALES                   10

#define NUMBER_OF_PROGRAMS                      10

typedef enum {

    BIT_PARAMETER,
    BYTE_PARAMETER,
    WORD_PARAMETER

} sectionParameterType_t;

typedef enum {

    FACTORY_RESET_WIPE_RESTORE,
    FACTORY_RESET_RESTORE_FULL,
    FACTORY_RESET_RESTORE_PARTIAL

} factoryResetType_t;

typedef struct {

    uint8_t sections;
    uint16_t blockStartAddress;
    uint16_t sectionAddress[10];
    uint16_t sectionParameters[10];
    sectionParameterType_t sectionParameterType[10];
    bool partialResetEnabled;

} blockDescriptor;

//default controller settings

class EEPROMsettings {

    public:
    EEPROMsettings();
    void clearEEPROM();
    void init();
    void initSettings(bool partialReset = false);
    void factoryReset(factoryResetType_t type);
    void createMemoryLayout();
    void createSectionAddresses();
    inline uint16_t readParameter(uint8_t blockID, uint8_t sectionID, uint16_t parameterID = 0)  {

        uint16_t startAddress = getSectionAddress(blockID, sectionID);
        uint8_t parameterType = getParameterType(blockID, sectionID);

        uint8_t arrayIndex;
        uint8_t parameterIndex;

        switch(parameterType)   {

            case BIT_PARAMETER:
            arrayIndex = parameterID/8;
            parameterIndex = parameterID - 8*arrayIndex;
            startAddress += arrayIndex;
            return bitRead(eeprom_read_byte((uint8_t*)startAddress), parameterIndex);
            break;

            case BYTE_PARAMETER:
            startAddress += parameterID;
            return eeprom_read_byte((uint8_t*)startAddress);
            break;

            case WORD_PARAMETER:
            startAddress += ((uint16_t)parameterID*2);
            return eeprom_read_word((uint16_t*)startAddress);
            break;

        }   return 0;

    };
    bool writeParameter(uint8_t blockID, uint8_t sectionID, int16_t parameterID, int16_t newValue);
    blockDescriptor blocks[CONF_BLOCKS];

    private:
    inline uint16_t getSectionAddress(uint8_t blockID, uint8_t sectionID)   {

        return blocks[blockID].blockStartAddress+blocks[blockID].sectionAddress[sectionID];

    };
    inline uint8_t getParameterType(uint8_t blockID, uint8_t sectionID) {

        return blocks[blockID].sectionParameterType[sectionID];

    }
    void initProgramSettings(bool partialReset = false);
    void initUserScales(bool partialReset = false);
    void initPadCalibration(bool partialReset = false);

};

extern EEPROMsettings configuration;


#endif