#ifndef FIRMWARE_VERSION_H_
#define FIRMWARE_VERSION_H_

#include <inttypes.h>
#include "../Debug.h"
#include "../Types.h"

#if MODE_SERIAL > 0
    #include "../vserial/Serial.h"
#endif

#include <inttypes.h>

#define FIRMWARE_VERSION_MAJOR      1
#define FIRMWARE_VERSION_MINOR      0

#define LAST_FLASH_ADDRESS          0x1DFFE
#define EEPROM_CRC_ADDRESS          4000

class VersionControl {

    public:
    VersionControl();
    bool checkNewRevision();
    uint8_t getSWversion(swVersion_t point);

    private:
    struct {

        uint8_t major;
        uint8_t minor;
        uint8_t revision;

    } firmwareVersion;

};

extern VersionControl versionControl;

#endif