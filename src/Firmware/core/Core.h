#pragma once

#include <avr/cpufunc.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "adc/ADC.h"
#include "i2c/i2c_master.h"
#include "reset/Reset.h"
#include "uart/UART.h"
#ifdef NDEBUG
#include "usb/midi/Descriptors.h"
#endif
#include "PinManipulation.h"
#include "timer/Timing.h"
