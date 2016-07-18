/*

File contains default settings for controller, as well as
EEPROM addresses of parameters.

 */

#include <stdio.h> 
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "../Modules.h"
#include "UniqueID.h"
#include "../Debug.h"
#include "../interface/lcd/LCD.h"

#if MODE_SERIAL > 0
#include "../vserial/Serial.h"
#endif

#include "../BitManipulation.h"

#ifdef MODULE_LCD
#include "../hardware/lcd/lcd.h"
#endif

#include "../interface/pads/Pads.h"
#include "Defaults.h"
#include "../Scales.h"
#include "Blocks.h"
#include "Sections.h"

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_SIZE     4096
#define START_OFFSET    5

typedef enum {

    BIT_PARAMETER,
    BYTE_PARAMETER,
    WORD_PARAMETER

} sectionParameterType_t;

typedef struct {

    uint8_t sections;
    uint16_t blockStartAddress;
    uint16_t sectionAddress[10];
    uint16_t sectionParameters[10];
    sectionParameterType_t sectionParameterType[10];
    bool preserveOnPartialReset;

} blockDescriptor;

//default controller settings

class Configuration : LCD {

    public:
    Configuration();
    void clearEEPROM();
    void init();
    void initSettings(bool partialReset);
    void factoryReset(factoryResetType_t type);
    friend void factory_reset(factoryResetType_t type);
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
            if (startAddress > EEPROM_SIZE) {

                #if MODE_SERIAL > 0
                    printf("Requested address out of EEPROM memory range\n");
                #endif
                return 0;

            }
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

        return blocks[blockID].blockStartAddress+blocks[blockID].sectionAddress[sectionID] + START_OFFSET;

    };
    inline uint16_t getBlockAddress(uint8_t blockID)   {

        return blocks[blockID].blockStartAddress+START_OFFSET;

    };
    inline uint8_t getParameterType(uint8_t blockID, uint8_t sectionID) {

        return blocks[blockID].sectionParameterType[sectionID];

    }
    void initProgramSettings(bool partialReset);
    void initUserScales(bool partialReset);
    void initPadCalibration(bool partialReset);
    void initMIDIsettings(bool partialReset);
    void checkReset();
    void writeSignature();

    struct {

        uint8_t major;
        uint8_t minor;
        uint8_t revision;
        uint16_t crc;

    } firmwareVersion;

};

extern Configuration configuration;

#endif
