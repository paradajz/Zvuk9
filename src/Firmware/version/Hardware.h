#pragma once

#define HARDWARE_VERSION_MAJOR      2
#define HARDWARE_VERSION_MINOR      0
#define HARDWARE_VERSION_REVISION   0

const struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t revision;
} hardwareVersion =
{
    HARDWARE_VERSION_MAJOR,
    HARDWARE_VERSION_MINOR,
    HARDWARE_VERSION_REVISION
};
