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

#include "LEDs.h"
#include "Variables.h"
#include "Helpers.h"
#include "pins/map/LEDs.h"
#include "core/src/general/BitManipulation.h"
#include "core/src/general/Timing.h"

/// \ingroup interfaceLEDs
/// @{

/// Array holding states of all LEDs.
uint8_t ledState[MAX_NUMBER_OF_LEDS];

/// Last time in milliseconds LED blink state has been changed.
uint32_t lastLEDblinkUpdateTime;

/// Array of LED indexes matched with note_t enumeration defined in MIDI module.
const uint8_t ledNoteArray[] = {
    LED_NOTE_C,
    LED_NOTE_C_SHARP,
    LED_NOTE_D,
    LED_NOTE_D_SHARP,
    LED_NOTE_E,
    LED_NOTE_F,
    LED_NOTE_F_SHARP,
    LED_NOTE_G,
    LED_NOTE_G_SHARP,
    LED_NOTE_A,
    LED_NOTE_A_SHARP,
    LED_NOTE_B
};

/// @}

/// Default constructor.
LEDs::LEDs()
{
}

/// Used only to check if any of blinking LEDs needs updating.
void LEDs::update()
{
    if ((rTimeMs() - lastLEDblinkUpdateTime) >= DEFAULT_BLINK_SPEED)
    {
        //time to update blinking leds
        //change blinkBit state and write it into ledState variable if LED is in blink state
        for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
        {
            if (BIT_READ(ledState[i], LED_BLINK_ON_BIT))
            {
                if (!BIT_READ(ledState[i], LED_STATE_BIT))
                    BIT_SET(ledState[i], LED_STATE_BIT);
                else
                    BIT_CLEAR(ledState[i], LED_STATE_BIT);
            }
        }

        lastLEDblinkUpdateTime = rTimeMs();
    }
}

/// Turns all LEDs off.
void LEDs::setAllOff()
{
    for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateOff);
}

/// Turns all LEDs on.
void LEDs::setAllOn()
{
    for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateOn);
}

/// Changes state of requested LED index.
/// @param [in] ledNumber   LED index for which state is being changed.
/// @param [in] state       New LED state. Enumerated type, see ledState_t.
void LEDs::setLEDstate(uint8_t ledNumber, ledState_t state)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        switch (state)
        {
        case ledStateOff:
            ledState[ledNumber] = 0;
            break;

        case ledStateOn:
        case ledStateBlink:
            //set active and state bit
            BIT_SET(ledState[ledNumber], LED_ACTIVE_BIT);
            BIT_SET(ledState[ledNumber], LED_STATE_BIT);

            if (state == ledStateBlink)
            {
                //enable blinking
                BIT_SET(ledState[ledNumber], LED_BLINK_ON_BIT);
                //this will turn the led immediately no matter how little time it's
                //going to blink first time
                BIT_SET(ledState[ledNumber], LED_STATE_BIT);
            }
            else
            {
                //disable led blinking
                BIT_CLEAR(ledState[ledNumber], LED_BLINK_ON_BIT);
            }
            break;

        default:
            return;
        }
    }
}

/// Changes state of LED index which corresponds with requested note index.
/// @param [in] note    Note index.
/// @param [in] state   New LED state.
void LEDs::setNoteLEDstate(note_t note, ledState_t state)
{
    setLEDstate(ledNoteArray[note], state);
}

/// Checks state for requested LED index.
/// @param [in] ledNumber   LED which is being checked.
/// \returns LED state (enumerated type, see ledState_t).
ledState_t LEDs::getLEDstate(uint8_t ledNumber)
{
    if (LED_ON(ledState[ledNumber]))
        return ledStateOn;
    else
        return ledStateOff;
}

/// Checks state of LED index which cooresponds with requested note index.
/// @param [in] note    Note index.
/// \returns LED state (enumerated type, see ledState_t).
ledState_t LEDs::getNoteLEDstate(note_t note)
{
    return getLEDstate(ledNoteArray[note]);
}

LEDs leds;
