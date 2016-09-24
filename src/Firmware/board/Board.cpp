#include "Board.h"
#include "Variables.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

Board::Board()  {

    //default constructor

}

void Board::init() {

    //clear timer3 info
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    TIMSK3 = 0;
    OCR3A = 0;
    OCR3B = 0;
    OCR3C = 0;

    //turn on CTC mode
    TCCR3B |= (1 << WGM32);

    //set prescaler to 256
    TCCR3B |= (1 << CS32);

    //configure timer1/2 for LED matrix PWM
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIMSK1 = 0;
    TCNT1 = 0;
    OCR1A = 0;
    OCR1B = 0;
    OCR1C = 0;

    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    TCNT2 = 0;
    OCR2A = 0;
    OCR2B = 0;

    //phase correct PWM, top 0xFF
    TCCR1A |= (1<<WGM10);
    TCCR2A |= (1<<WGM20);

    //prescaler 1
    TCCR1B |= (1<<CS10);
    TCCR2B |= (1<<CS20);

    //configure timer0
    //used for delay/millis/leds
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;
    OCR0B = 0;

    //turn on CTC mode
    TCCR0A |= (1 << WGM01);

    //set prescaler to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);

    //set compare match register to desired timer count
    OCR0A = 124; //500uS

    OCR3A = 10;

    //enable CTC interrupt for timer0
    TIMSK0 |= (1 << OCIE0A);

    TIMSK3 |= (1<<OCIE3A);

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        setInput(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setInput(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        setHigh(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setHigh(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        encoderData[i] |= ((uint16_t)0 << 8);
        encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);   //set number of pulses to 8

    }

    i2c_init();

    //ensure that we know the configuration
    write_I2C_reg(expanderAddress[0], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[0], 0x05, 0x00);              //IOCON=0x00 if BANK=1
    write_I2C_reg(expanderAddress[1], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[1], 0x05, 0x00);              //IOCON=0x00 if BANK=1

    write_I2C_reg(expanderAddress[0], iodirAddress[0], 0xFF);   //expander 1, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[0], iodirAddress[1], 0xFF);   //expander 1, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[1], iodirAddress[0], 0xFF);   //expander 2, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[1], iodirAddress[1], 0xFF);   //expander 2, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[0], gppuAddress[0], 0xFF);    //expander 1, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[0], gppuAddress[1], 0xFF);    //expander 1, turn on pull-ups, PORTB

    write_I2C_reg(expanderAddress[1], gppuAddress[0], 0xFF);    //expander 2, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[1], gppuAddress[1], 0xFF);    //expander 2, turn on pull-ups, PORTB

    setOutput(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setOutput(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setOutput(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setOutput(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    setOutput(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    setOutput(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    setOutput(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    setHigh(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

};

Board board;