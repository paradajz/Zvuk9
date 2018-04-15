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
#include "../../interface/digital/output/leds/Helpers.h"
#include "constants/LEDs.h"

///
/// \ingroup boardAVR
/// @{

///
/// \brief Implementation of core variable used to keep track of run time in milliseconds.
///
volatile uint32_t   rTime_ms;

///
/// \brief Array holding LED row pins for easier access.
///
const uint8_t       ledRowPinArray[] =
{
    LED_ROW_1_PIN,
    LED_ROW_2_PIN,
    LED_ROW_3_PIN
};

///
/// \brief Array holding LED row ports for easier access.
///
volatile uint8_t    *ledRowPortArray[] =
{
    &LED_ROW_1_PORT,
    &LED_ROW_2_PORT,
    &LED_ROW_3_PORT
};

/// @}

///
/// \brief Initializes main and PWM timers.
///
void Board::initTimers()
{
    //configure timer3
    //used for millis and button/led matrix control
    //reset values first
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

///
/// \ingroup boardAVR
/// @{

///
/// \brief Turns requested LED row on with specified PWM intensity.
/// @param [in] rowNumber   Row in LED matrix which is being turned on.
/// @param [in] intensity   PWM intensity of requested row.
///
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

///
/// \brief Turns all rows in LED matrix off.
///
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

///
/// \brief Activates currently active LED matrix column (stored in activeLEDcolumn variable).
///
inline void activateOutputColumn()
{
    //clear current decoder state
    DECODER_OUT_PORT &= DECODER_OUT_CLEAR_MASK;
    //activate new column
    DECODER_OUT_PORT |= decoderOutOrderArray[activeLEDcolumn];
}

///
/// \brief Activates currently active button matrix column (stored in activeInColumn variable).
///
inline void activateInputColumn()
{
    //clear current decoder state
    DECODER_IN_PORT &= DECODER_IN_CLEAR_MASK;

    //activate new column
    DECODER_IN_PORT |= decoderInOrderArray[activeInColumn];
}

///
/// \brief Checks if any of LEDs in currently active LED matrix column needs to be turned on.
/// This function also performs transition effect betweeen two LED states.
///
inline void checkLEDs()
{
    //if there is an active LED in current column, turn on LED row
    //do fancy transitions here
    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)
    {
        uint8_t ledNumber = activeLEDcolumn+i*NUMBER_OF_LED_COLUMNS;
        uint8_t ledStateSingle = LED_ON(ledState[ledNumber]);

        ledStateSingle *= (NUMBER_OF_LED_TRANSITIONS-1);

        if (ledTransitionScale[transitionCounter[ledNumber]])
            ledRowOn(i, ledTransitionScale[transitionCounter[ledNumber]]);

        if (transitionCounter[ledNumber] != ledStateSingle)
        {
            if (transitionCounter[ledNumber] < ledStateSingle)
            {
                //fade up
                transitionCounter[ledNumber] += DEFAULT_FADE_SPEED;

                if (transitionCounter[ledNumber] > ledStateSingle)
                    transitionCounter[ledNumber] = ledStateSingle;
            }
            else
            {
                //fade down
                transitionCounter[ledNumber] -= DEFAULT_FADE_SPEED;

                if (transitionCounter[ledNumber] < 0)
                    transitionCounter[ledNumber] = 0;
            }
        }
    }
}

///
/// Acquires data for all buttons connected in currently active button matrix column by
/// reading inputs from shift register.
///
inline void storeDigitalIn()
{
    setLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
    setLow(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);
    _NOP();

    digitalInBuffer[activeInColumn] = 0;

    setHigh(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);

    for (int i=0; i<8; i++)
    {
        setLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
        _NOP();
        BIT_WRITE(digitalInBuffer[activeInColumn], 7-i, !readPin(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN));
        setHigh(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
    }
}

///
/// \brief Main interrupt service routine.
/// Used to control LED and button matrix and to update current run time.
///
ISR(TIMER3_COMPA_vect)
{
    //1ms
    ledRowsOff();

    if (activeLEDcolumn == NUMBER_OF_LED_COLUMNS)
        activeLEDcolumn = 0;

    activateOutputColumn();
    checkLEDs();

    activeLEDcolumn++;

    //update run time
    rTime_ms++;

    //read input matrix
    if (activeInColumn < NUMBER_OF_BUTTON_COLUMNS)
    {
        for (int i=0; i<NUMBER_OF_BUTTON_COLUMNS; i++)
        {
            activeInColumn = i;
            activateInputColumn();
            storeDigitalIn();
        }

        activeInColumn = NUMBER_OF_BUTTON_COLUMNS;
    }
}

/// @}