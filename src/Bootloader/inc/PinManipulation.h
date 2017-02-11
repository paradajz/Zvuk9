#pragma once

#include <avr/cpufunc.h>

#define DDR(x) (*(&x-1))
#define PIN(x) (*(&x-2))

#define setOutput(port, pin) ((DDR(port)) |= (1 << (pin)))
#define setInput(port, pin) ((DDR(port)) &= ~(1 << (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
#define readPin(port, pin) (((PIN(port)) >> (pin)) & 0x01)

#define pulseHightToLow(port, pin) do \
{ \
    setHigh((port), (pin)); \
    _NOP(); \
    setLow((port), (pin)); \
} while (0)

#define pulseLowToHigh(port, pin) do \
{ \
    setLow((port), (pin)); \
    _NOP(); \
    setHigh((port), (pin)); \
} while (0)
