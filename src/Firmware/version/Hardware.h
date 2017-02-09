#pragma once

#ifdef BOARD_R1
#define HARDWARE_VERSION_MAJOR      1
#define HARDWARE_VERSION_MINOR      0
#define HARDWARE_VERSION_REVISION   0
#elif defined (BOARD_R2)
#define HARDWARE_VERSION_MAJOR      2
#define HARDWARE_VERSION_MINOR      0
#define HARDWARE_VERSION_REVISION   0
#else
#error Wrong board selected
#endif
