#include <avr/io.h>

#ifndef Uart_h
#define Uart_h

//most of UART code copied from teensy core libs
//some modifications are made to transmission part

class UART    {

    public:
        UART();
        void begin(uint32_t baud_count, bool enableRX, bool enableTX);
        uint16_t available();
        int8_t read();
        void write(uint8_t);

};

extern UART uart;

#endif