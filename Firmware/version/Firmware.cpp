#include "Firmware.h"

VersionControl::VersionControl()    {

    //init
    firmwareVersion.minor = FIRMWARE_VERSION_MINOR;
    firmwareVersion.major = FIRMWARE_VERSION_MAJOR;

}

bool VersionControl::checkNewRevision() {

    uint16_t crc_old = eeprom_read_word((uint16_t*)EEPROM_CRC_ADDRESS);
    uint16_t crc_hex = pgm_read_word_far(LAST_FLASH_ADDRESS);

    if (crc_old != crc_hex)   {

        eeprom_update_word((uint16_t*)EEPROM_CRC_ADDRESS, crc_hex);
        return true;

    }

}

uint8_t VersionControl::getSWversion(swVersion_t point) {

    switch(point)   {

        case swVersion_major:
        return firmwareVersion.major;

        case swVersion_minor:
        return firmwareVersion.minor;

        case swVersion_revision:
        return firmwareVersion.revision;

        default:
        return 0;

    }

}

VersionControl versionControl;