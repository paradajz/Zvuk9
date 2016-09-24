#include "ADC.h"
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

int16_t adcValue = 0;

void setUpADC() {

    ADMUX = 0x00;
    ADCSRA = 0x0;

    //setup adc prescaler
    #if ADC_PRESCALER == 128
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    #elif ADC_PRESCALER == 64
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
    #elif ADC_PRESCALER == 32
    ADCSRA |= (1<<ADPS2)|(1<<ADPS0);
    #elif ADC_PRESCALER == 16
    ADCSRA |= (1<<ADPS2);
    #else
    #error Incorrect ADC prescaler setting. Valid values are 128, 64, 32 and 16.
    #endif

    //setup voltage reference
    #ifdef VREF_AREF
    //nothing, this is default setting
    #elif defined(VREF_AVCC)
    ADMUX |= (1<<REFS0);
    #elif defined(VREF_INTERNAL_2V56)
    ADMUX |= (1<<REFS0) | (1<<REFS1);
    #elif defined(VREF_INTERNAL_1V1)
    ADMUX |= (1<<REFS1);
    #else
    #error "No ADC reference selected or setting is invalid. Valid options are VREF_AREF, VREF_AVCC, VREF_INTERNAL_2V56 and VREF_INTERNAL_1V1"
    #endif

    adcEnable();
    setADCchannel(0);

    for (int i=0; i<5; i++)
        getADCvalue(); //few dummy reads to init adc

    #if ADC_INTERRUPT == 1
        adcInterruptEnable();
    #endif

}

void setADCchannel(uint8_t channel)  {

    //check for valid channel
    if ((channel < 0) || (channel > 7))   return;

    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    _NOP();

}

void disconnectDigitalInADC(uint8_t channel) {

    if (channel < 6)
        DIDR0 |= (1<<channel);

}

int16_t getADCvalue()   {

    if ((ADCSRA >> ADSC) & 0x01)  {

        //conversion is still running
        return -1;

    }   else {

        //no conversion running
        if ((ADCSRA >> ADIE) & 0x01) { return adcValue; }
        else {

            //wait until ADC conversion is complete
            while (ADCSRA & (1<<ADSC));
            return ADC;

        }

    }

}

ISR(ADC_vect)   {

    adcValue = ADC;

}

bool analogReadInProgress() {

    return ((ADCSRA >> ADSC) & 0x01);

}