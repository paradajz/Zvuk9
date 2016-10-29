#pragma once

#include <avr/io.h>

class UART
{
    public:
    UART();
    void begin(uint32_t baud_count, bool enableRX, bool enableTX);
    uint16_t available();
    int8_t read();
    void write(uint8_t);
};

extern UART uart;
