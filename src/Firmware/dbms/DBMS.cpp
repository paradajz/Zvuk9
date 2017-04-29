#include "DBMS.h"
#include "Config.h"

blockDescriptor block[MAX_BLOCKS];

///
/// \brief Returns section address for specified section within block.
/// \returns Section address.
///
inline uint16_t getSectionAddress(uint8_t blockID, uint8_t sectionID)
{
    return block[blockID].blockStartAddress+block[blockID].sectionAddress[sectionID];
};

///
/// \brief Returns block address for specified block.
/// \returns Block address.
///
inline uint16_t getBlockAddress(uint8_t blockID)
{
    return block[blockID].blockStartAddress;
};

///
/// \brief Returns parameter type for specified block and section.
/// \returns Parameter type.
///
inline sectionParameterType_t getParameterType(uint8_t blockID, uint8_t sectionID)
{
    return block[blockID].section[sectionID].parameterType;
}

///
/// \brief Default constructor
///
DBMS::DBMS()
{
    #ifdef ENABLE_ASYNC_UPDATE
    for (int i=0; i<EEPROM_UPDATE_BUFFER_SIZE; i++)
    {
        eeprom_update_bufer_param_type[i] = 0;
        eeprom_update_bufer_value[i] = 0;
        eeprom_update_bufer_address[i] = 0;
    }

    eeprom_update_buffer_head = 0;
    eeprom_update_buffer_tail = 0;
    #endif
}

///
/// \brief Reads value from database.
/// @param [in] blockID     Block for wanted parameter.
/// @param [in] sectionID   Section for wanted parameter.
/// @param [in] parameterID Parameter ID
/// \returns Retrived value.
///
int32_t DBMS::read(uint8_t blockID, uint8_t sectionID, uint16_t parameterID)
{
    uint16_t startAddress = getSectionAddress(blockID, sectionID);
    uint8_t parameterType = getParameterType(blockID, sectionID);

    uint8_t arrayIndex;
    uint8_t parameterIndex;

    switch(parameterType)
    {
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

        case DWORD_PARAMETER:
        startAddress += ((uint16_t)parameterID*4);
        return eeprom_read_dword((uint32_t*)startAddress);
        break;
    }

    return 0;
}

///
/// \brief Updates value for specified block and section in database.
/// @param [in] blockID     Block for wanted parameter.
/// @param [in] sectionID   Section for wanted parameter.
/// @param [in] parameterID Parameter ID.
/// @param [in] newValue    New value for parameter
/// @param [in] async       Whether to update value immediately (false) or later (true).
/// \returns True on success, false otherwise.
///
bool DBMS::update(uint8_t blockID, uint8_t sectionID, uint16_t parameterID, int32_t newValue, bool async)
{
    uint16_t startAddress = getSectionAddress(blockID, sectionID);

    if (startAddress > EEPROM_SIZE)
    {
        #ifdef DEBUG
        printf_P(PSTR("Requested address out of EEPROM memory range\n"));
        #endif
        return 0;
    }

    uint8_t parameterType = getParameterType(blockID, sectionID);

    uint8_t arrayIndex;
    uint8_t arrayValue;
    uint8_t parameterIndex;

    switch(parameterType)
    {
        case BIT_PARAMETER:
        arrayIndex = parameterID/8;
        parameterIndex = parameterID - 8*arrayIndex;
        arrayValue = eeprom_read_byte((uint8_t*)startAddress+arrayIndex);
        bitWrite(arrayValue, parameterIndex, newValue);
        #ifdef ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+arrayIndex, arrayValue, BIT_PARAMETER);
            return true;
        }
        else
        {
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
        if (async)
        {
            queueData(startAddress+parameterID, newValue, BYTE_PARAMETER);
            return true;
        }
        else
        {
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
        if (async)
        {
            queueData(startAddress+parameterID, newValue, WORD_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
            return (newValue == read(blockID, sectionID, parameterID));
        }
        #else
        eeprom_update_word((uint16_t*)startAddress+parameterID, newValue);
        return (newValue == read(blockID, sectionID, parameterID));
        #endif
        break;

        case DWORD_PARAMETER:
        #ifdef ENABLE_ASYNC_UPDATE
        if (async)
        {
            queueData(startAddress+parameterID, newValue, DWORD_PARAMETER);
            return true;
        }
        else
        {
            eeprom_update_dword((uint32_t*)startAddress+parameterID, newValue);
            return (newValue == read(blockID, sectionID, parameterID));
        }
        #else
        eeprom_update_dword((uint32_t*)startAddress+parameterID, newValue);
        return (newValue == read(blockID, sectionID, parameterID));
        #endif
        break;
    }

    return 0;
}

///
/// \brief Clears entire EEPROM memory by writing 0xFF to each location.
///
void DBMS::clear()
{
    for (int i=0; i<EEPROM_SIZE; i++)
        eeprom_update_byte((uint8_t*)i, 0xFF);
}

///
/// \brief Adds single block to current layout.
/// \returns True on success, false otherwise.
///
bool DBMS::addBlock()
{
    if (blockCounter >= MAX_BLOCKS)
        return false;

    blockCounter++;
    return true;
}

///
/// \brief Adds specified number of blocks to current layout.
/// @param [in] numberOfBlocks  Number of blocks to add.
/// \returns True on success, false otherwise.
///
bool DBMS::addBlocks(uint8_t numberOfBlocks)
{
    if (blockCounter+numberOfBlocks >= MAX_BLOCKS)
        return false;

    blockCounter += numberOfBlocks;
    return true;
}

///
/// \brief Adds section to specified block.
/// @param [in] blockID Block on which to add section.
/// @param [in] section Structure holding description of section.
/// \returns True on success, false otherwise.
///
bool DBMS::addSection(uint8_t blockID, dbSection_t section)
{
    if (block[blockID].sections >= MAX_SECTIONS)
        return false;

    block[blockID].section[block[blockID].sections].parameterType = section.parameterType;
    block[blockID].section[block[blockID].sections].preserveOnPartialReset = section.preserveOnPartialReset;
    block[blockID].section[block[blockID].sections].defaultValue = section.defaultValue;
    block[blockID].section[block[blockID].sections].parameters = section.parameters;

    block[blockID].sections++;
    return true;
}

///
/// \brief Calculates all addresses for specified blocks and sections.
///
void DBMS::commitLayout()
{
    for (int i=0; i<blockCounter; i++)
    {
        uint16_t memory_usage = 0;

        for (int j=0; j<block[i].sections; j++)
        {
            if (!j)
            {
                //first section address is always 0
                block[i].sectionAddress[0] = 0;
            }
            else
            {
                switch(block[i].section[j-1].parameterType)
                {
                    case BIT_PARAMETER:
                    block[i].sectionAddress[j] = ((block[i].section[j].parameters % 8 != 0) + block[i].section[j-1].parameters/8) + block[i].sectionAddress[j-1];
                    break;

                    case BYTE_PARAMETER:
                    block[i].sectionAddress[j] = block[i].section[j-1].parameters + block[i].sectionAddress[j-1];
                    break;

                    case WORD_PARAMETER:
                    block[i].sectionAddress[j] = 2*block[i].section[j-1].parameters + block[i].sectionAddress[j-1];
                    break;

                    case DWORD_PARAMETER:
                    block[i].sectionAddress[j] = 4*block[i].section[j-1].parameters + block[i].sectionAddress[j-1];
                    break;
                }
            }
        }

        uint8_t lastSection = block[i].sections-1;

        switch(block[i].section[lastSection].parameterType)
        {
            case BIT_PARAMETER:
            memory_usage = block[i].sectionAddress[lastSection]+((block[i].section[lastSection].parameters%8 != 0) + block[i].section[lastSection].parameters/8);
            break;

            case BYTE_PARAMETER:
            memory_usage = block[i].sectionAddress[lastSection] + block[i].section[lastSection].parameters;
            break;

            case WORD_PARAMETER:
            memory_usage = block[i].sectionAddress[lastSection] + 2*block[i].section[lastSection].parameters;
            break;

            case DWORD_PARAMETER:
            memory_usage = block[i].sectionAddress[lastSection] + 4*block[i].section[lastSection].parameters;
            break;
        }

        if (i < blockCounter-1)
            block[i+1].blockStartAddress = block[i].blockStartAddress + memory_usage;
    }
}

///
/// \brief Writes default values to EEPROM from defaultValue parameter.
/// @param [in] type    Type of initialization (initPartial or initWipe). Init wipe will simply orverwrite currently existing
///                     data. initPartial will leave data as is, but only if preserveOnPartialReset parameter is set to true.
///
void DBMS::initData(initType_t type)
{
    for (int i=0; i<blockCounter; i++)
    {
        for (int j=0; j<block[i].sections; j++)
        {
            if (block[i].section[j].preserveOnPartialReset && (type == initPartial))
                continue;

            uint16_t startAddress = getSectionAddress(i, j);
            uint8_t parameterType = getParameterType(i, j);
            uint8_t defaultValue = block[i].section[j].defaultValue;
            uint8_t numberOfParameters = block[i].section[j].parameters;

            switch(parameterType)
            {
                case BIT_PARAMETER:
                for (int i=0; i<numberOfParameters/8+1; i++)
                    eeprom_update_byte((uint8_t*)startAddress+i, defaultValue);
                break;

                case BYTE_PARAMETER:
                while (numberOfParameters--)
                {
                    if (defaultValue == AUTO_INCREMENT)
                        eeprom_update_byte((uint8_t*)startAddress+numberOfParameters, numberOfParameters);
                    else
                        eeprom_update_byte((uint8_t*)startAddress+numberOfParameters, defaultValue);
                }
                break;

                case WORD_PARAMETER:
                while (numberOfParameters--)
                {
                    if (defaultValue == AUTO_INCREMENT)
                        eeprom_update_word((uint16_t*)(uint16_t)(startAddress+(numberOfParameters*2)), numberOfParameters);
                    else
                        eeprom_update_word((uint16_t*)(uint16_t)(startAddress+(numberOfParameters*2)), (uint16_t)defaultValue);
                }
                break;

                case DWORD_PARAMETER:
                while (numberOfParameters--)
                {
                    if (defaultValue == AUTO_INCREMENT)
                        eeprom_update_dword((uint32_t*)(uint16_t)(startAddress+(numberOfParameters*4)), numberOfParameters);
                    else
                        eeprom_update_dword((uint32_t*)(uint16_t)(startAddress+(numberOfParameters*4)), (uint32_t)defaultValue);
                }
                break;
            }
        }
    }
}

#ifdef ENABLE_ASYNC_UPDATE
void DBMS::queueData(uint16_t eepromAddress, uint16_t data, uint8_t parameterType)
{
    uint8_t index = eeprom_update_buffer_head + 1;

    if (index >= EEPROM_UPDATE_BUFFER_SIZE)
        index = 0;

    //if buffer is full, wait until there is some space
    if (eeprom_update_buffer_tail == index)
    {
        #if MODE_SERIAL > 0
        printf("Oops, buffer full. Waiting...\n");
        #endif

        while (!checkQueue());
    }

    eeprom_update_bufer_param_type[index] = parameterType;
    eeprom_update_bufer_value[index] = data;
    eeprom_update_bufer_address[index] = eepromAddress;
    eeprom_update_buffer_head = index;
}

bool DBMS::checkQueue()
{
    //write queued data to eeprom

    if (eeprom_update_buffer_head == eeprom_update_buffer_tail)
    {
        //buffer is empty
        return false;
    }

    //there is something in buffer
    uint8_t index = eeprom_update_buffer_tail + 1;

    if (index >= EEPROM_UPDATE_BUFFER_SIZE)
        index = 0;

    //write
    switch(eeprom_update_bufer_param_type[index])
    {
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