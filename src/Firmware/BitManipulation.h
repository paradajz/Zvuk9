#ifndef BITMANIPULATION_H_
#define BITMANIPULATION_H_

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define invertByte(value) ((value) ^ 0xFF)
#define lowByte_7bit(value) ((value) & 0x7f)
#define highByte_7bit(value) ((value >> 7) & 0x7f)

typedef struct {

    uint8_t high;
    uint8_t low;

    uint16_t encodeTo14bit()    {

        uint8_t newLow = low & 0x7F;
        uint8_t newHigh = (high << 1) & 0x7F;
        bitWrite(newHigh, 0, bitRead(low, 7));

        uint16_t joined;
        joined = newHigh;
        joined <<= 8;
        joined |= newLow;

        return joined;

    }

    uint16_t decode14bit()  {

        uint16_t newHigh = (high >> 1) & 0x7F;
        uint16_t newLow = low;
        bitWrite(newLow, 7, bitRead(high, 0));

        uint16_t joined;
        joined = newHigh;
        joined <<= 8;
        joined |= newLow;

        return joined;

    }

} encDec_14bit;
#endif