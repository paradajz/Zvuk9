#include "Timing.h"

uint32_t rTimeMs()
{
    uint32_t _rTime_mS;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        _rTime_mS = rTime_ms;
    }

    return _rTime_mS;
}