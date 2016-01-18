#include "UART.h"
#include <avr/interrupt.h>

//RX/TX buffer size in bytes
#define SERIAL_BUFFER_SIZE 64

static volatile uint8_t tx_buffer[SERIAL_BUFFER_SIZE];
static volatile uint8_t rx_buffer[SERIAL_BUFFER_SIZE];

static volatile uint8_t rx_buffer_head = 0;
static volatile uint8_t rx_buffer_tail = 0;

static volatile uint8_t tx_buffer_head = 0;
static volatile uint8_t tx_buffer_tail = 0;

static volatile uint8_t transmitting = 0;

bool    rxEnabled,
        txEnabled;

//isr functions

ISR(USART1_RX_vect) {

    uint8_t c, i;

    c = UDR1;
    i = rx_buffer_head + 1;

    if (i >= SERIAL_BUFFER_SIZE) i = 0;

    if (i != rx_buffer_tail) {

        rx_buffer[i] = c;
        rx_buffer_head = i;

    }

}

ISR(USART1_UDRE_vect)   {

    uint8_t i;

    if (tx_buffer_head == tx_buffer_tail) {

        // buffer is empty, disable transmit interrupt
        if (!rxEnabled)
            UCSR1B = (1<<TXCIE1) | (1<<TXEN1);
        else UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);

    } else {

        i = tx_buffer_tail + 1;
        if (i >= SERIAL_BUFFER_SIZE) i = 0;
        UDR1 = tx_buffer[i];
        tx_buffer_tail = i;

    }

}

ISR(USART1_TX_vect) {

    transmitting = 0;

}

UART::UART()  {

    //default constructor

}


int8_t UART::read(void)   {

    uint8_t data, i;

    if (rx_buffer_head == rx_buffer_tail) return -1;
    i = rx_buffer_tail + 1;
    if (i >= SERIAL_BUFFER_SIZE) i = 0;
    data = rx_buffer[i];
    rx_buffer_tail = i;
    return data;

}

void UART::write(uint8_t data)  {

    if (!txEnabled) return;

    uint8_t i;

    if (!(UCSR1B & (1<<TXEN1))) return;

    i = tx_buffer_head + 1;

    if (i >= SERIAL_BUFFER_SIZE) i = 0;

    while (tx_buffer_tail == i); // wait until space in buffer

    tx_buffer[i] = data;
    transmitting = 1;

    tx_buffer_head = i;

    if (!rxEnabled)
        UCSR1B = (1<<TXCIE1) | (1<<TXEN1) | (1<<UDRIE1);
    else UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<UDRIE1);

}

void UART::begin(uint32_t baudRate, bool enableRX, bool enableTX)   {

    rxEnabled = enableRX;
    txEnabled = enableTX;

    int16_t baud_count = ((F_CPU / 8) + (baudRate / 2)) / baudRate;

    if ((baud_count & 1) && baud_count <= 4096) {

        UCSR1A = (1<<U2X1); //double speed uart
        UBRR1 = baud_count - 1;

    }   else {

        UCSR1A = 0;
        UBRR1 = (baud_count >> 1) - 1;

    }

    if (!(UCSR1B & (1<<TXEN1))) {

        rx_buffer_head = 0;
        rx_buffer_tail = 0;
        tx_buffer_head = 0;
        tx_buffer_tail = 0;

        transmitting = 0;

        UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); //8 bit, no parity, 1 stop bit

        if (enableRX && enableTX)   //enable both rx and tx
            UCSR1B = (1<<RXEN1) | (1<<TXCIE1) | (1<<TXEN1) | (1<<RXCIE1);
        else if (enableRX && !enableTX) //enable only receive
            UCSR1B = (1<<RXEN1) | (1<<RXCIE1);
        else if (enableTX & !enableRX)  //enable only transmit
            UCSR1B = (1<<TXCIE1) | (1<<TXEN1);

    }

}

uint16_t UART::available()    {

    //return available number of bytes in incoming buffer

    uint8_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;

    if (head >= tail) return head - tail;
    return SERIAL_BUFFER_SIZE + head - tail;

}

//initialize serial object
UART uart;