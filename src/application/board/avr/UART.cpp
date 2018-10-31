/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#include "../Board.h"
#include "core/src/general/RingBuffer.h"

///
/// \ingroup board
/// @{

///
/// \brief Buffer in which outgoing UART data is stored.
///
static RingBuff_t  txBuffer;

///
/// \brief ISR used to write outgoing data in buffer to UART.
///
ISR(USART1_UDRE_vect)
{
    if (RingBuffer_IsEmpty(&txBuffer))
    {
        //buffer is empty, disable transmit interrupt
        UCSR1B &= ~(1<<UDRIE1);
    }
    else
    {
        uint8_t data = RingBuffer_Remove(&txBuffer);
        UDR1 = data;
    }
}

///
/// \brief Writes a byte to outgoing UART buffer.
/// \returns Positive value on success. Since this function waits if
/// outgoig buffer is full, result will always be success (1).
///
bool UARTwrite(uint8_t data)
{
    //if both the outgoing buffer and the UART data register are empty
    //write the byte to the data register directly
    if (RingBuffer_IsEmpty(&txBuffer) && (UCSR1A & (1<<UDRE1)))
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            UDR1 = data;
        }

        return true;
    }

    while (RingBuffer_IsFull(&txBuffer));
    RingBuffer_Insert(&txBuffer, data);
    UCSR1B |= (1<<UDRIE1);
    return true;
}

/// @}

void Board::initUART_MIDI()
{
    int32_t baud_count = ((F_CPU / 8) + (MIDI_BAUD_RATE_STD / 2)) / MIDI_BAUD_RATE_STD;

    //clear registers first
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        UCSR1A = 0;
        UCSR1B = 0;
        UCSR1C = 0;
        UBRR1 = 0;
    }

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

    //8 bit, no parity, 1 stop bit
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

    //enable transmitter only
    UCSR1B = (1<<TXEN1);

    RingBuffer_InitBuffer(&txBuffer);
    midi.handleUARTwrite(UARTwrite);
}