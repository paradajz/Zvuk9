/*

File contains default settings for controller, as well as
EEPROM addresses of parameters.

 */

#include <stdio.h> 
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "UniqueID.h"
#include "Config.h"
#include "Defaults.h"
#include "Blocks.h"

#ifndef EEPROM_H_
#define EEPROM_H_

#ifdef ENABLE_ASYNC_UPDATE
#define EEPROM_UPDATE_BUFFER_SIZE   64
#endif

typedef enum {

    BIT_PARAMETER,
    BYTE_PARAMETER,
    WORD_PARAMETER

} sectionParameterType_t;

typedef struct {

    uint8_t sections;
    uint16_t blockStartAddress;
    uint16_t sectionAddress[MAX_SECTIONS];
    uint16_t sectionParameters[MAX_SECTIONS];
    sectionParameterType_t sectionParameterType[MAX_SECTIONS];
    bool preserveOnPartialReset;

} blockDescriptor;

typedef enum {

    sections,
    parameters,
    parameterType,
    preserveOnPartialreset

} blockInfo_t;

typedef enum {

    factoryReset_restore, //update eeprom with defaults
    factoryReset_partial //partially restore defaults

} factoryResetType_t;

//default controller settings

class Configuration {

    public:
    Configuration();
    #ifdef ENABLE_ASYNC_UPDATE
    bool update();
    #endif
    void clearEEPROM();
    void init();
    bool checkSignature();
    void initSettings(bool partialReset);
    void factoryReset(factoryResetType_t type);
    friend void factory_reset(factoryResetType_t type);
    void createMemoryLayout();
    void createSectionAddresses();
    uint8_t getBlockSections(uint8_t block);
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

                #ifdef DEBUG
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
    bool writeParameter(uint8_t blockID, uint8_t sectionID, int16_t parameterID, int16_t newValue, bool async = false);
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
    void writeSignature();
    #ifdef ENABLE_ASYNC_UPDATE
    void queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType);
    #endif

    #ifdef ENABLE_ASYNC_UPDATE
    //update buffer
    uint8_t     eeprom_update_bufer_param_type[EEPROM_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_value[EEPROM_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_address[EEPROM_UPDATE_BUFFER_SIZE];
    uint8_t     eeprom_update_buffer_head;
    uint8_t     eeprom_update_buffer_tail;
    #endif

};

extern Configuration configuration;

#endif
