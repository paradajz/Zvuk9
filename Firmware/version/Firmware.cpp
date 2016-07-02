#include "Firmware.h"

VersionControl::VersionControl()    {

    //init
    firmwareVersion.magicValue = 6517;//
    firmwareVersion.minor = FIRMWARE_VERSION_MINOR;
    firmwareVersion.major = FIRMWARE_VERSION_MAJOR;
    firmwareVersion.revision = FIRMWARE_VERSION_REVISION;

}

bool VersionControl::checkNewRevision() {

    if (CODE_REVISION_FLAG) return true;
    return false;

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