#pragma once

///
/// \brief Helper macros for easier byte/bit manipulation.
/// \defgroup coreBitHelpers Bit manipulation
/// \ingroup coreHelpers
/// @{
#define bitRead(value, bit)             (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)              ((value) |= (1UL << (bit)))
#define bitClear(value, bit)            ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)  (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define invertByte(value)               ((value) ^ 0xFF)
#define lowByte(value)                  ((value) & 0xFF)
#define highByte(value)                 (((value) >> 8) & 0xFF)
/// @}