#pragma once

#include <avr/cpufunc.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "adc/ADC.h"
#include "i2c/i2c_master.h"
#include "uart/UART.h"
#ifdef NDEBUG
#include "usb/midi/Descriptors.h"
#endif
#include "helpers/PinManipulation.h"
#include "helpers/BitManipulation.h"
#include "timer/Timing.h"
#include "reboot/Reboot.h"