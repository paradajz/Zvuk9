#include <stdio.h>
#include <util/delay.h>
#include "Timing.h"

void wait_ms(uint32_t ms)
{
    while(ms--)
    {
        _delay_ms(1);
    }
}

void wait_us(uint32_t us)
{
    while (us--)
    {
        _delay_us(1);
    }
}