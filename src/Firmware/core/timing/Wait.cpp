#include "Timing.h"

void wait_ms(uint32_t ms)
{
    while(ms--)
    {
        _delay_ms(1);
    }
}