#include "Reset.h"
#include <util/delay.h>
#include <avr/wdt.h>

#define HOLD_TIME   3000

bool timerStarted = false;
uint32_t timerValue;

#define WDFR 3

void disable_peripherals(void)   {

    //disable eeprom
    EECR = 0;

    //disable analog comparator
    ACSR = 0;

    //disable SPI
    SPCR = 0;

    //disable external interrupts
    EIMSK = 0;

    //disable pin change interrupts
    PCICR = 0;

    //disable ADC
    ADCSRA = 0;

    //disable timers
    TIMSK0 = 0;
    TIMSK1 = 0;
    TIMSK3 = 0;

    //disable USART
    UCSR1B = 0;

    //disable I2C
    TWCR = 0;

    //set all pins to inputs
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    DDRE = 0;
    DDRF = 0;

    //write low to all pins
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTF = 0;

}


bool checkReset(bool buttonState)   {

    switch(buttonState) {

        case true:
        if (!timerStarted)  {

            timerStarted = true;
            timerValue = newMillis();

        }
        return (newMillis() - timerValue > HOLD_TIME);
        break;

        case false:
        timerStarted = false;
        return false;
        break;

    }

}

void resetTeensy()  {

    cli();
    // stop watchdog timer, if running
    MCUSR &= ~(1<<WDFR);
    WDTCSR |= (1<<WDCE);
    WDTCSR = 0;
    _delay_ms(5);
    UDCON = 1;
    USBCON = (1<<FRZCLK);
    _delay_ms(2000);
    disable_peripherals();

    asm volatile("jmp 0x1FC00");
    while (1);

}