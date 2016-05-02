#include "ADC.h"
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

int16_t adcValue = 0;

void initADC(adcPrescaler prescaler, bool interruptEnabled, adcVRef vRef) {

    ADMUX = 0x00;
    ADCSRA = 0x0;

    //set reference voltage
    setVoltageReference(vRef);

    //set prescaler
    setADCprescaler(prescaler);

    //enable high-speed mode if available
    #ifdef ADHSM
        ADCSRB |= (1<<ADHSM);
    #endif

    setADCchannel(0);

    adcEnable();

    for (int i=0; i<5; i++)
        getADCvalue(); //few dummy reads to init adc

    if (interruptEnabled) adcInterruptEnable();

}

void setADCprescaler(adcPrescaler prescaler) {

    //disable ADC before setting new prescaler
    ADCSRA &= ~(1<<ADEN);

    switch(prescaler)   {

        case ADC_PRESCALER_16:
        ADCSRA |= (1<<ADPS2);
        break;

        case ADC_PRESCALER_32:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS0);
        break;

        case ADC_PRESCALER_64:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
        break;

        case ADC_PRESCALER_128:
        ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
        break;

        default:
        break;

    }

}

void setVoltageReference(adcVRef vRef)  {

    switch(vRef)    {

        case ADC_V_REF_AREF:
        ADMUX &= ~(1<<REFS0);
        ADMUX &= ~(1<<REFS1);
        break;

        case ADC_V_REF_AVCC:
        ADMUX |= (1<<REFS0);
        ADMUX &= ~(1<<REFS1);
        break;

        case ADC_V_REF_INTERNAL_2_56:
        ADMUX |= (1<<REFS0);
        ADMUX |= (1<<REFS1);
        break;

        default:
        break;

    }

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