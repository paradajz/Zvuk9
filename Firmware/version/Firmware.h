#ifndef FIRMWARE_VERSION_H_
#define FIRMWARE_VERSION_H_

#include <inttypes.h>
#include "../Debug.h"
#include "../Types.h"
#include "../hardware/gpior/GPIOR.h"

#if MODE_SERIAL > 0
    #include "../vserial/Serial.h"
#endif

const uint32_t ProgramLength __attribute__ ((section (".revision"))) = 0;

#include <inttypes.h>

#define MAGIC_VALUE                 0xCEFABEBA

#define FIRMWARE_VERSION_MAJOR      1
#define FIRMWARE_VERSION_MINOR      46
#define FIRMWARE_VERSION_REVISION   71

class VersionControl {

    public:
    VersionControl();
    bool checkNewRevision();
    uint8_t getSWversion(swVersion_t point);

    private:
    struct {

        uint32_t magicValue;
        uint8_t major;
        uint8_t minor;
        uint8_t revision;

    } firmwareVersion;

};

extern VersionControl versionControl;

#endif