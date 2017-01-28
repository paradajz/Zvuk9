#pragma once

typedef enum
{
    version_major,
    version_minor,
    version_revision
} version_t;

#define VERSION_POINT_LOCATION  (FLASH_SIZE - 8) //6 bytes for version, 2 bytes for crc
#define CRC_LOCATION_FLASH      (FLASH_SIZE - 2)
#define CRC_LOCATION_EEPROM     (EEPROM_SIZE - 4)

bool checkNewRevision();
uint8_t getSWversion(version_t point);
