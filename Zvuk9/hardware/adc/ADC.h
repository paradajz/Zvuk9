#ifndef ADC_H_
#define ADC_H_

//ADC setup and manipulation

#include <avr/io.h>

#define startADCconversion()        (ADCSRA |= (1<<ADSC))
#define adcInterruptEnable()        (ADCSRA |= (1<<ADIE))
#define adcEnable()                 (ADCSRA |= (1<<ADEN))

typedef enum {

    ADC_PRESCALER_16,
    ADC_PRESCALER_32,
    ADC_PRESCALER_64,
    ADC_PRESCALER_128

} adcPrescaler;

typedef enum {

    ADC_V_REF_AREF,
    ADC_V_REF_AVCC,
    ADC_V_REF_INTERNAL_2_56

} adcVRef;

void initADC(adcPrescaler prescaler, bool interruptEnabled, adcVRef vRef);
void setADCprescaler(adcPrescaler prescaler);
void setADCchannel(uint8_t channel);
int16_t getADCvalue();
void disconnectDigitalInADC(uint8_t channel);
bool analogReadInProgress();
void setVoltageReference(adcVRef vRef);

#endif