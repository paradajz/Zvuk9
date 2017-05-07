#include "UART.h"
#include "RingBuffer/RingBuffer.h"
#include <avr/interrupt.h>

RingBuff_t  txBuffer, rxBuffer;

bool        rxEnabled,
            txEnabled;

//isr functions

///
/// \brief Default constructor.
///
UART::UART()
{
    
}

///
/// \brief ISR used to store incoming data from UART to buffer.
///
ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;

    if (!RingBuffer_IsFull(&rxBuffer))
        RingBuffer_Insert(&rxBuffer, data);
}

///
/// \brief ISR used to send data from outgoing buffer to UART.
///
ISR(USART1_UDRE_vect)
{
    if (RingBuffer_IsEmpty(&txBuffer))
    {
        // buffer is empty, disable transmit interrupt
        if (!rxEnabled)
            UCSR1B = (1<<TXCIE1) | (1<<TXEN1);
        else
            UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
    }
    else
    {
        uint8_t data = RingBuffer_Remove(&txBuffer);
        UDR1 = data;
    }
}

ISR(USART1_TX_vect)
{
    
}

///
/// \brief Initializes UART peripheral.
///
void UART::begin(uint32_t baudRate, bool enableRX, bool enableTX)
{
    rxEnabled = enableRX;
    txEnabled = enableTX;

    int16_t baud_count = ((F_CPU / 8) + (baudRate / 2)) / baudRate;

    if ((baud_count & 1) && baud_count <= 4096)
    {
        UCSR1A = (1<<U2X1); //double speed uart
        UBRR1 = baud_count - 1;
    }
    else
    {
        UCSR1A = 0;
        UBRR1 = (baud_count >> 1) - 1;
    }

    if (enableRX && enableTX)
        UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
    else if (enableRX && !enableTX)
        UCSR1B = (1<<RXEN1) | (1<<RXCIE1);
    else if (enableTX & !enableRX)
        UCSR1B = (1<<TXCIE1) | (1<<TXEN1);

    //8 bit, no parity, 1 stop bit
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

    RingBuffer_InitBuffer(&rxBuffer);
    RingBuffer_InitBuffer(&txBuffer);
}

///
/// \brief Reads a byte from incoming UART buffer
/// \returns Single byte on success, -1 is buffer is empty.
///
uint8_t UART::read()
{
    if (RingBuffer_IsEmpty(&rxBuffer))
    return -1;

    uint8_t data = RingBuffer_Remove(&rxBuffer);
    return data;
}

///
/// \brief Writes single byte to TX buffer.
/// @param [in] data    Byte value
/// \returns 0 on success, -1 otherwise.
///
int8_t UART::write(uint8_t data)
{
    if (!txEnabled)
        return -1;

    if (!(UCSR1B & (1<<TXEN1)))
        return -1;

    if (RingBuffer_IsFull(&txBuffer))
        return -1;

    RingBuffer_Insert(&txBuffer, data);

    if (!rxEnabled)
        UCSR1B = (1<<TXCIE1) | (1<<TXEN1) | (1<<UDRIE1);
    else
        UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);

    return 0;
}

///
/// \brief Checks if any incoming UART data is available.
/// \returns True if any data is available, false otherwise.
///
bool UART::available()
{
    return !RingBuffer_IsEmpty(&rxBuffer);
}

UART uart;
