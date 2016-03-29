#ifndef DEBUG_H_
#define DEBUG_H_

#include "hardware/pins/board_v1.h"

#ifdef VARIANT_1
#define MODE_SERIAL 0
#elif defined (VARIANT_2)
#define MODE_SERIAL 0
#endif

#define START_UP_ANIMATION  1

#endif