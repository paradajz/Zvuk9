#pragma once

///
/// \brief Setup and manipulation of SPI peripheral.
/// \defgroup spi SPI
/// \ingroup core
/// @{

#define SPI_CLOCK_MASK      0x03
#define SPI_2XCLOCK_MASK    0x01
#define SPI_CLOCK_DIV4      0x00
#define SPI_CLOCK_DIV16     0x01
#define SPI_CLOCK_DIV64     0x02
#define SPI_CLOCK_DIV128    0x03
#define SPI_CLOCK_DIV2      0x04
#define SPI_CLOCK_DIV8      0x05
#define SPI_CLOCK_DIV32     0x06

#define SPI_MODE0           0x00
#define SPI_MODE1           0x04
#define SPI_MODE2           0x08
#define SPI_MODE3           0x0C

#define SPI_MODE_MASK       0x0C

#define LSBFIRST            0
#define MSBFIRST            1

inline static void setBitOrder(uint8_t bitOrder)
{
    if (bitOrder == LSBFIRST)
        SPCR |= _BV(DORD);
    else
        SPCR &= ~(_BV(DORD));
}

inline static void setClockDivider(uint8_t clockDiv)
{
    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (clockDiv & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((clockDiv >> 2) & SPI_2XCLOCK_MASK);
}

inline static void setDataMode(uint8_t dataMode)
{
    SPCR = (SPCR & ~SPI_MODE_MASK) | dataMode;
}

///
/// \brief Initialize SPI peripheral.
///
void spiInit();

///
/// \brief Transfers one byte of data using SPI peripheral.
///
uint8_t spiTransfer(uint8_t data);
/// @}