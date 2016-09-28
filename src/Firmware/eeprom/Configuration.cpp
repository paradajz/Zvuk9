#ifndef EEPROMSETTINGS_H_
#define EEPROMSETTINGS_H_

#include "Configuration.h"
#include <util/delay.h>

#ifdef DEBUG
#include "../vserial/Serial.h"
#endif

Configuration::Configuration()    {

    //def const
    #ifdef ENABLE_ASYNC_UPDATE
    for (int i=0; i<EEPROM_UPDATE_BUFFER_SIZE; i++) {

        eeprom_update_bufer_param_type[i] = 0;
        eeprom_update_bufer_value[i] = 0;
        eeprom_update_bufer_address[i] = 0;

    }

    eeprom_update_buffer_head = 0;
    eeprom_update_buffer_tail = 0;
    #endif

}

void Configuration::init() {

    createMemoryLayout();
    createSectionAddresses();

}

void Configuration::factoryReset(factoryResetType_t type)   {

    initSettings(type == factoryReset_partial);
    writeSignature();

}

void Configuration::writeSignature()    {

    uint8_t unique_id_invert = invertByte(EEPROM_UNIQUE_ID);

    for (int i=0; i<START_OFFSET; i++)
        (i%2) ? eeprom_update_byte((uint8_t*)i, unique_id_invert) : eeprom_update_byte((uint8_t*)i, EEPROM_UNIQUE_ID);

}

bool Configuration::checkSignature()    {

    uint8_t unique_id_invert = invertByte(EEPROM_UNIQUE_ID);
    uint8_t checkByte;

    //check if all bytes up to START_OFFSET address match unique id
    for (int i=0; i<START_OFFSET; i++)  {

        (i%2) ? checkByte = unique_id_invert : checkByte = EEPROM_UNIQUE_ID;

        if (eeprom_read_byte((uint8_t*)i) != checkByte)
            return false;

    }   return true;

}

void Configuration::clearEEPROM()  {

    for (int i=0; i<EEPROM_SIZE; i++)
        eeprom_update_byte((uint8_t*)i, 0xFF);

}

void Configuration::createSectionAddresses()   {

    for (int i=0; i<CONF_BLOCKS; i++)  {

        uint16_t memory_usage = 0;

        for (int j=0; j<blocks[i].sections; j++)    {

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

        if (i < CONF_BLOCKS-1) {

            blocks[i+1].blockStartAddress = blocks[i].blockStartAddress + memory_usage;

        }

    }

}

bool Configuration::writeParameter(uint8_t blockID, uint8_t sectionID, int16_t parameterID, int16_t newValue, bool async)    {

    uint16_t startAddress = getSectionAddress(blockID, sectionID);

    if (startAddress > EEPROM_SIZE) {

        #ifdef DEBUG
            printf("Requested address out of EEPROM memory range\n");
        #endif
        return 0;

    }

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
        #ifdef ENABLE_ASYNC_UPDATE
        if (async)  {

            queueData(startAddress+arrayIndex, arrayValue, BIT_PARAMETER);
            return true;

        } else {

            eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
            return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));

        }
        #else
        eeprom_update_byte((uint8_t*)startAddress+arrayIndex, arrayValue);
        return (arrayValue == eeprom_read_byte((uint8_t*)startAddress+arrayIndex));
        #endif
        break;

        case BYTE_PARAMETER:
        #ifdef ENABLE_ASYNC_UPDATE
        if (async)  {

            queueData(startAddress+parameterID, newValue, BYTE_PARAMETER);
            return true;

        }   else {

            eeprom_update_byte((uint8_t*)startAddress+parameterID, newValue);
            return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterID));

        }
        #else
        eeprom_update_byte((uint8_t*)startAddress+parameterID, newValue);
        return (newValue == eeprom_read_byte((uint8_t*)startAddress+parameterID));
        #endif
        break;

        case WORD_PARAMETER:
        #ifdef ENABLE_ASYNC_UPDATE
        if (async)  {

            queueData(startAddress+parameterID, newValue, WORD_PARAMETER);
            return true;

        }   else {

            eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
            return (newValue == (int16_t)eeprom_read_word((uint16_t*)startAddress+parameterID));

        }
        #else
        eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
        return (newValue == (int16_t)eeprom_read_word((uint16_t*)startAddress+parameterID));
        #endif
        break;

    }   return 0;

}

#ifdef ENABLE_ASYNC_UPDATE
void Configuration::queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType)    {

    uint8_t index = eeprom_update_buffer_head + 1;
    if (index >= EEPROM_UPDATE_BUFFER_SIZE) index = 0;
    //if buffer is full, wait until there is some space
    if (eeprom_update_buffer_tail == index)  {

        #ifdef DEBUG
        printf("Oops, buffer full. Waiting...\n");
        #endif

        while (!update());

    }

    eeprom_update_bufer_param_type[index] = parameterType;
    eeprom_update_bufer_value[index] = data;
    eeprom_update_bufer_address[index] = eepromAddress;
    eeprom_update_buffer_head = index;

}

bool Configuration::update()    {

    //write queued data to eeprom

    if (eeprom_update_buffer_head == eeprom_update_buffer_tail)   {

        //buffer is empty
        return false;

    }

    //there is something in buffer
    uint8_t index = eeprom_update_buffer_tail + 1;
    if (index >= EEPROM_UPDATE_BUFFER_SIZE) index = 0;
    //write
    switch(eeprom_update_bufer_param_type[index])   {

        case BIT_PARAMETER:
        case BYTE_PARAMETER:
        eeprom_update_byte((uint8_t*)eeprom_update_bufer_address[index], eeprom_update_bufer_value[index]);
        break;

        case WORD_PARAMETER:
        eeprom_update_word((uint16_t*)eeprom_update_bufer_address[index], eeprom_update_bufer_value[index]);
        break;

    }

    eeprom_update_buffer_tail = index;
    return true;

}
#endif

uint8_t Configuration::getBlockSections(uint8_t block)  {

    if (block >= CONF_BLOCKS) return 0;

    return blocks[block].sections;

}

Configuration configuration;

#endif
