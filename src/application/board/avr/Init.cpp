/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#include "../Board.h"
#include "pins/Pins.h"
#include "../../interface/analog/pads/DataTypes.h"
#include "constants/Pads.h"
#include "pins/Pins.h"
#include "pins/map/Pads.h"
#include "../common/analog/Variables.h"

#ifdef DEBUG
#include "usb/vserial/VSerial.h"
#endif

///
/// \ingroup boardAVR
/// @{

Board::Board()
{

}

void Board::init()
{
    cli();
    //disable watchdog
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    initPins();
    initPads();

    #ifndef DEBUG
    initUSB_MIDI();
    #else
    CDC_Init();
    #endif

    initUART_MIDI();

    initTimers();
};

void Board::initPins()
{
    setOutput(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setOutput(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setOutput(LED_ROW_3_PORT, LED_ROW_3_PIN);

    #ifdef LED_INVERT
    setHigh(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setHigh(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setHigh(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #else
    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #endif

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);

    setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);

    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setOutput(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    setOutput(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    setOutput(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    setOutput(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    setHigh(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setInput(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN);
    setOutput(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);
    setOutput(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);

    setOutput(DECODER_IN_A0_PORT, DECODER_IN_A0_PIN);
    setOutput(DECODER_IN_A1_PORT, DECODER_IN_A1_PIN);
    setOutput(DECODER_IN_A2_PORT, DECODER_IN_A2_PIN);

    setInput(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN);
    setHigh(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN);

    //init unused pins - output, low
    setOutput(PORTA, 0);
    setOutput(PORTA, 1);
    setOutput(PORTA, 2);
    setOutput(PORTA, 6);
    setOutput(PORTA, 7);

    setLow(PORTA, 0);
    setLow(PORTA, 1);
    setLow(PORTA, 2);
    setLow(PORTA, 6);
    setLow(PORTA, 7);

    setOutput(PORTB, 0);
    setOutput(PORTB, 3);
    setOutput(PORTB, 7);

    setLow(PORTB, 0);
    setLow(PORTB, 3);
    setLow(PORTB, 7);

    setOutput(PORTC, 6);
    setOutput(PORTC, 7);

    setLow(PORTC, 6);
    setLow(PORTC, 7);

    setOutput(PORTD, 0);
    setOutput(PORTD, 1);
    setOutput(PORTD, 2);
    setOutput(PORTD, 4);

    setLow(PORTD, 0);
    setLow(PORTD, 1);
    setLow(PORTD, 2);
    setLow(PORTD, 4);

    setOutput(PORTE, 0);
    setOutput(PORTE, 1);
    setOutput(PORTE, 3);
    setOutput(PORTE, 4);
    setOutput(PORTE, 5);
    setOutput(PORTE, 6);
    setOutput(PORTE, 7);

    setLow(PORTE, 0);
    setLow(PORTE, 1);
    setLow(PORTE, 3);
    setLow(PORTE, 4);
    setLow(PORTE, 5);
    setLow(PORTE, 6);
    setLow(PORTE, 7);

    //setup pressure0
    setInput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setInput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);

    setLow(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setHigh(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
}

void Board::initTimers()
{
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;
    TIMSK3 = 0;
    OCR3A = 0;
    OCR3B = 0;
    OCR3C = 0;

    //set CTC mode
    TCCR3B |= (1 << WGM32);

    //set prescaler to 64
    TCCR3B |= (1 << CS31) | (1 << CS30);

    //set compare match register to desired timer count
    OCR3A = 249; //1ms

    //enable CTC interrupt for timer3
    TIMSK3 |= (1<<OCIE3A);

    //configure timer1/2 for LED matrix PWM
    //reset values first
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

    //clear timer0 conf (unused)
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;
}

void Board::initPads()
{
    //initialize ADC peripheral first
    adcConf adcConfiguration;

    adcConfiguration.prescaler = ADC_PRESCALER_128;
    adcConfiguration.vref = ADC_VREF_AVCC;

    setUpADC(adcConfiguration);

    //few dummy reads to init ADC properly
    for (int i=0; i<5; i++)
        getADCvalue();

    //start by reading first pressure value
    padReadingIndex = readPressure0;
    setADCchannel(coordinateAnalogInput[padReadingIndex]);
    //select first pad
    activePad = 0;

    adcInterruptEnable();
}

Board board;

/// @}