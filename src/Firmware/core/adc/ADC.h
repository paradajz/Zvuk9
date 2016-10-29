#pragma once

//ADC setup and manipulation

#include <avr/io.h>
#include "Config.h"

#define startADCconversion()        (ADCSRA |= (1<<ADSC))
#define adcInterruptEnable()        (ADCSRA |= (1<<ADIE))
#define adcEnable()                 (ADCSRA |= (1<<ADEN))

void setUpADC();
void setADCchannel(uint8_t channel);
int16_t getADCvalue();
void disconnectDigitalInADC(uint8_t channel);
bool analogReadInProgress();
