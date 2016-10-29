#pragma once

#include <inttypes.h>

#define VERSION_POINT_LOCATION  (FLASH_SIZE - 10)
#define CRC_LOCATION_FLASH      (FLASH_SIZE - 2)
#define CRC_LOCATION_EEPROM     (EEPROM_SIZE - 2)   //write crc to last eeprom location

typedef enum
{
    swVersion_major,
    swVersion_minor,
    swVersion_revision,
    swVersion_development
} swVersion_t;

bool checkNewRevision();
int16_t getSWversion(swVersion_t point);
