#include "Firmware.h"

#include "Firmware.h"

bool checkNewRevision()
{
    //current app crc is written to last flash location
    //previous crc is stored into eeprom
    //if two differ, app has changed

    uint16_t crc_eeprom = eeprom_read_word((uint16_t*)CRC_LOCATION_EEPROM);
    uint16_t crc_flash = pgm_read_word_far(CRC_LOCATION_FLASH);

    if (crc_eeprom != crc_flash)
    {
        eeprom_update_word((uint16_t*)CRC_LOCATION_EEPROM, crc_flash);
        return true;
    }

    return false;
}

uint8_t getSWversion(version_t point)
{
    switch(point)
    {
        case version_major:
        case version_minor:
        case version_revision:
        return (uint8_t)pgm_read_word_far(VERSION_POINT_LOCATION+(uint8_t)point*2);

        default:
        return 0;
    }
}
