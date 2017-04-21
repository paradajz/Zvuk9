#pragma once

#include "DataTypes.h"

#ifndef EEPROM_SIZE
#error EEPROM size not defined
#endif

///
/// \brief Simple database-like management system.
/// \ingroup eeprom
/// @{
///
class DBMS
{
    public:
    DBMS();
    void init();
    #ifdef ENABLE_ASYNC_UPDATE
    bool checkQueue();
    #endif
    void clear();
    int32_t read(uint8_t blockID, uint8_t sectionID, uint16_t parameterID = 0);
    bool update(uint8_t blockID, uint8_t sectionID, uint16_t parameterID, int32_t newValue, bool async = false);

    protected:
    bool addBlock();
    bool addBlocks(uint8_t numberOfBlocks);
    bool addSection(uint8_t blockID, dbSection_t section);
    void commitLayout();
    void initData(initType_t type = initWipe);

    private:
    #ifdef ENABLE_ASYNC_UPDATE
    void queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType);
    #endif

    #ifdef ENABLE_ASYNC_UPDATE
    uint8_t     eeprom_update_bufer_param_type[EEPROM_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_value[EEPROM_UPDATE_BUFFER_SIZE];
    uint16_t    eeprom_update_bufer_address[EEPROM_UPDATE_BUFFER_SIZE];
    uint8_t     eeprom_update_buffer_head;
    uint8_t     eeprom_update_buffer_tail;
    #endif

    uint8_t blockCounter;
};
/// @}