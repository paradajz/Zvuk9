#include "SPI.h"

void spiInit()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        //SS, at90usb1286
        setOutput(PORTB, 0);
        setHigh(PORTB, 0);

        SPCR |= (1<<MSTR);
        SPCR |= (1<<SPE);

        //clock, at90usb1286
        setOutput(PORTB, 1);
        //mosi, at90usb1286
        setOutput(PORTB, 2);
    }
}

uint8_t spiTransfer(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}