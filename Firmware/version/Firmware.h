#ifndef FIRMWARE_VERSION_H_
#define FIRMWARE_VERSION_H_

#include <inttypes.h>
#include "../Debug.h"
#include "../Types.h"

#if MODE_SERIAL > 0
    #include "../vserial/Serial.h"
#endif

#include <inttypes.h>

bool checkNewRevision();
int16_t getSWversion(swVersion_t point);

#endif