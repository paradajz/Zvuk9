#ifndef FIRMWARE_VERSION_H_
#define FIRMWARE_VERSION_H_

#include <inttypes.h>
#include "../Types.h"

#define VERSION_POINT_LOCATION  (FLASH_SIZE - 10)
#define CRC_LOCATION_FLASH      (FLASH_SIZE - 2)
#define CRC_LOCATION_EEPROM     (EEPROM_SIZE - 2)   //write crc to last eeprom location

bool checkNewRevision();
int16_t getSWversion(swVersion_t point);

#endif