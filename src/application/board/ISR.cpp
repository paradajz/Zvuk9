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

#include "Board.h"

volatile uint32_t   rTime_ms;
volatile uint8_t    inputBuffer;

const uint8_t       ledRowPinArray[] =
{
    LED_ROW_1_PIN,
    LED_ROW_2_PIN,
    LED_ROW_3_PIN
};

volatile uint8_t    *ledRowPortArray[] =
{
    &LED_ROW_1_PORT,
    &LED_ROW_2_PORT,
    &LED_ROW_3_PORT
};

void Board::initTimers()
{
    //configure timer3
    //used for led matrix, millis and encoder readout
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
    OCR3A = 62; //0.25ms

    //enable CTC interrupt for timer3
    TIMSK3 |= (1<<OCIE3A);

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

    //clear timer0 conf
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;
}

inline void checkLEDs()
{
    if (blinkEnabled)
    {
        if (!blinkTimerCounter)
        {
            //change blinkBit state and write it into ledState variable if LED is in blink state
            for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
            {
                if (BIT_READ(ledState[i], LED_BLINK_ON_BIT))
                {
                    if (blinkState)
                        BIT_SET(ledState[i], LED_BLINK_STATE_BIT);
                    else
                        BIT_CLEAR(ledState[i], LED_BLINK_STATE_BIT);
                }
            }

            blinkState = !blinkState;
        }
    }

    //if there is an active LED in current column, turn on LED row
    //do fancy transitions here
    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)
    {
        uint8_t ledNumber = activeLEDcolumn+i*NUMBER_OF_LED_COLUMNS;
        uint8_t ledStateSingle = BIT_READ(ledState[ledNumber], LED_ACTIVE_BIT) && (BIT_READ(ledState[ledNumber], LED_BLINK_ON_BIT) == BIT_READ(ledState[ledNumber], LED_BLINK_STATE_BIT));

        if (ledStateSingle)
        {
            if (BIT_READ(ledState[ledNumber], LED_INTENSITY_BIT))
                ledStateSingle = LED_FULL_INTENSITY;
            else
                ledStateSingle = LED_HALF_INTENSITY;
        }

        //don't bother with pwm if it's disabled
        if (!pwmSteps && ledStateSingle)
        {
            ledRowOn(i, ledTransitionScale[ledStateSingle]);
        }
        else
        {
            if (ledTransitionScale[transitionCounter[ledNumber]])
                ledRowOn(i, ledTransitionScale[transitionCounter[ledNumber]]);

            if (transitionCounter[ledNumber] != ledStateSingle)
            {
                if (transitionCounter[ledNumber] < ledStateSingle)
                {
                    //fade up
                    transitionCounter[ledNumber] += pwmSteps;

                    if (transitionCounter[ledNumber] > ledStateSingle)
                        transitionCounter[ledNumber] = ledStateSingle;
                }
                else
                {
                    //fade down
                    transitionCounter[ledNumber] -= pwmSteps;

                    if (transitionCounter[ledNumber] < 0)
                        transitionCounter[ledNumber] = 0;
                }
            }
        }
    }
}

inline void ledRowOn(uint8_t rowNumber, uint8_t intensity)
{
    if (intensity == 255)
    {
        //max value, don't use pwm
        #ifdef LED_INVERT
        setLow(*(ledRowPortArray[rowNumber]), ledRowPinArray[rowNumber]);
        #else
        setHigh(*(ledRowPortArray[rowNumber]), ledRowPinArray[rowNumber]);
        #endif
        return;
    }

    if (!intensity)
        return;

    #ifdef LED_INVERT
    intensity = 255 - intensity;
    #endif

    switch (rowNumber)
    {
        //turn off pwm if intensity is max
        case 0:
        OCR2A = intensity;
        TCCR2A |= (1<<COM2A1);
        break;

        case 1:
        OCR1A  = intensity;
        TCCR1A |= (1<<COM1A1);
        break;

        case 2:
        OCR1B = intensity;
        TCCR1A |= (1<<COM1B1);
        break;

        default:
        break;
    }
}

inline void ledRowsOff()
{
    //turn off pwm
    TCCR2A &= ~(1<<COM2A1);
    TCCR1A &= ~(1<<COM1A1);
    TCCR1A &= ~(1<<COM1B1);

    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)
    {
        #ifdef LED_INVERT
        setHigh(*(ledRowPortArray[i]), ledRowPinArray[i]);
        #else
        setLow(*(ledRowPortArray[i]), ledRowPinArray[i]);
        #endif
    }
}

inline void activateOutputColumn()
{
    //clear current decoder state
    DECODER_OUT_PORT &= DECODER_OUT_CLEAR_MASK;
    //activate new column
    DECODER_OUT_PORT |= decoderOutOrderArray[activeLEDcolumn];
}

inline void activateInputColumn(uint8_t column)
{
    //clear current decoder state
    DECODER_IN_PORT &= DECODER_IN_CLEAR_MASK;

    //activate new column
    DECODER_IN_PORT |= decoderInOrderArray[column];
}

inline void storeDigitalIn(uint8_t column)
{
    uint8_t buttonNumber;
    uint8_t encoderNumber;
    bool state;
    uint8_t row = NUMBER_OF_BUTTON_ROWS;
    bool even = true;

    //pulse latch pin
    pulseLowToHigh(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);

    while (row--)
    {
        //invert row, starting from last one
        buttonNumber = column + row*NUMBER_OF_BUTTON_COLUMNS;
        state = !readPin(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN);
        inputBuffer <<= 1;
        inputBuffer |= state;
        inputBuffer &= 0x03;
        //button readout
        buttonDebounceCounter[buttonNumber] = (buttonDebounceCounter[buttonNumber] << 1) | state | BUTTON_DEBOUNCE_COMPARE;
        //encoder readout - check even rows only
        even = !even;

        //pulse clock pin
        pulseHighToLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);

        if (even)
        {
            //check only every second row
            //get last encoder direction
            //hardcoded! formula for getting encoder number is ACTIVE_ROW * (NUMBER_OF_COLUMNS/2) + COLUMN
            //since we know there are 8 columns, half of that is 4
            //instead of multiplying row by four, simply right shift row by two places
            encoderNumber = (row << 2) + column;
            //get last encoder direction
            bool lastDirection = BIT_READ(encoderState[encoderNumber], 7);

            //shift in new encoder readings
            encoderState[encoderNumber] <<= 2;
            encoderState[encoderNumber] |= inputBuffer;
            //make sure not to reset last direction
            encoderState[encoderNumber] &= 0x8F;

            //update new encoder direction
            BIT_WRITE(encoderState[encoderNumber], 7, lastDirection);

            int8_t newPulse = encoderLookUpTable[encoderState[encoderNumber] & 0x0F];

            if (!newPulse)
                continue; //no movement

            bool newDirection = newPulse > 0;

            //add new pulse count
            encPulses_x4[encoderNumber] += newPulse;

            //update new direction
            BIT_WRITE(encoderState[encoderNumber], 7, newDirection);

            if (lastDirection != newDirection)
                continue;

            if (abs(encPulses_x4[encoderNumber]) < pulsesPerStep[encoderNumber])
                continue;

            encPulses[encoderNumber] += newDirection ? 1 : -1;
            encPulses_x4[encoderNumber] = 0;
        }
    }
}

ISR(TIMER3_COMPA_vect)
{
    static uint8_t updateStuff = 0;

    updateStuff++;

    if (updateStuff == 4)
    {
        //1ms
        ledRowsOff();

        if (activeLEDcolumn == NUMBER_OF_LED_COLUMNS)
            activeLEDcolumn = 0;

        activateOutputColumn();
        checkLEDs();

        activeLEDcolumn++;
        blinkTimerCounter++;

        if (blinkTimerCounter >= ledBlinkTime)
            blinkTimerCounter = 0;

        //update run time
        rTime_ms++;

        for (int i=0; i<NUMBER_OF_BUTTON_COLUMNS; i++)
        {
            activateInputColumn(i);
            storeDigitalIn(i);
        }

        updateStuff = 0;
    }

    startADCconversion();
}
