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
#include "board/common/digital/output/Variables.h"
#include "board/common/digital/input/Variables.h"
#include "../../interface/digital/output/leds/Helpers.h"
#include "constants/LEDs.h"
#include "pins/Pins.h"
#include "pins/map/LEDs.h"
#include "pins/map/Pads.h"
#include "core/src/HAL/avr/PinManipulation.h"
#include "core/src/general/BitManipulation.h"
#include "core/src/HAL/avr/adc/ADC.h"


///
/// \ingroup board
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
        setLow(*ledRowPins[rowNumber].port, ledRowPins[rowNumber].pin);
        return;
    }

    if (!intensity)
        return;

    intensity = 255 - intensity;

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
        setHigh(*ledRowPins[i].port, ledRowPins[i].pin);
}

///
/// \brief Activates currently active LED matrix column (stored in activeOutColumn variable).
///
inline void activateOutputColumn()
{
    //clear current decoder state
    DECODER_OUT_PORT &= DECODER_OUT_CLEAR_MASK;
    //activate new column
    DECODER_OUT_PORT |= decoderOutOrderArray[activeOutColumn];
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

    if (++activeInColumn == NUMBER_OF_BUTTON_COLUMNS)
        activeInColumn = 0;
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
        uint8_t ledNumber = activeOutColumn+i*NUMBER_OF_LED_COLUMNS;
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
/// \brief Acquires data for all buttons connected in currently active button matrix column by
/// reading inputs from shift register.
///
inline void storeDigitalIn()
{
    for (int i=0; i<NUMBER_OF_BUTTON_COLUMNS; i++)
    {
        activateInputColumn();
        _NOP();

        setLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
        setLow(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);
        _NOP();
        setHigh(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);

        for (int j=0; j<8; j++)
        {
            setLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
            _NOP();
            BIT_WRITE(digitalInBuffer[dIn_head][i], 7-j, !readPin(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN));
            setHigh(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
        }
    }
}

///
/// \brief Configures pad pins so that pressure value can be read correctly.
// Two pressure setups are used. Using only one setup results in pressure being lower
/// in one pad corner. To overcome this, use two setups where opposite pins are configured.
/// Once both setups are finished, pressure is average value of two read pressure readings.
///
inline void setupPressure0()
{
    //apply voltage from one X conductor to one Y conductor
    //read voltages at the other X and Y conductors
    setInput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setInput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);

    setLow(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setHigh(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
}

///
/// \brief Configures pad pins so that pressure value can be read correctly.
// Two pressure setups are used. Using only one setup results in pressure being lower
/// in one pad corner. To overcome this, use two setups where opposite pins are configured.
/// Once both setups are finished, pressure is average value of two read pressure readings.
///
inline void setupPressure1()
{
    //apply voltage from one X conductor to one Y conductor
    //read voltages at the other X and Y conductors
    setInput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setInput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);

    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setHigh(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
}

/// @}

///
/// \brief Configures pad pins so that X value can be read correctly.
///
inline void setupX()
{
    //apply voltage across the X-plane
    //read the voltage from either Y conductor
    setInput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setInput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setOutput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setOutput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);

    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setHigh(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
}

///
/// \brief Configures pad pins so that Y value can be read correctly.
///
inline void setupY()
{
    //apply voltage across the Y-plane
    //read the voltage from either X conductor
    setInput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setInput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);

    setLow(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setHigh(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
}

///
/// \brief Configures one of 16 inputs/outputs on 4067 multiplexer.
/// @param [in] muxInput Index of multiplexer input which should be activated.
///
inline void setMuxInput(uint8_t muxInput)
{
    BIT_READ(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    BIT_READ(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    BIT_READ(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    BIT_READ(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);
}

/// @}