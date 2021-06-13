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

#include "Buttons.h"
#include "../../../display/menu/Menu.h"
#include "handlers/Handlers.h"
#include "pins/map/Buttons.h"
#include "core/src/general/BitManipulation.h"
#include "core/src/general/Timing.h"

/// Default constructor.
Buttons::Buttons()
{
    processingEnabled = true;

    //enable all buttons
    for (int i = 0; i < MAX_NUMBER_OF_BUTTONS / 8 + 1; i++)
        buttonEnabled[i] = 0xFF;

    //map notes to buttons
    buttonToNoteArray[BUTTON_NOTE_C]       = C;
    buttonToNoteArray[BUTTON_NOTE_C_SHARP] = C_SHARP;
    buttonToNoteArray[BUTTON_NOTE_D]       = D;
    buttonToNoteArray[BUTTON_NOTE_D_SHARP] = D_SHARP;
    buttonToNoteArray[BUTTON_NOTE_E]       = E;
    buttonToNoteArray[BUTTON_NOTE_F]       = F;
    buttonToNoteArray[BUTTON_NOTE_F_SHARP] = F_SHARP;
    buttonToNoteArray[BUTTON_NOTE_G]       = G;
    buttonToNoteArray[BUTTON_NOTE_G_SHARP] = G_SHARP;
    buttonToNoteArray[BUTTON_NOTE_A]       = A;
    buttonToNoteArray[BUTTON_NOTE_A_SHARP] = A_SHARP;
    buttonToNoteArray[BUTTON_NOTE_B]       = B;
}

/// Initializes button variables to their default states.
void Buttons::init()
{
    initHandlers();
    uint32_t currentTime = rTimeMs();
    processingEnabled    = false;
    transportControlMode = (transportControlMode_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID);

    //read buttons for 0.1 seconds
    do
    {
        //read all buttons without activating event handlers
        //normally, update function is called in DigitalInterace abstraction
        //call board functions manually here
        if (board.digitalInputDataAvailable())
        {
            buttons.update();
        }
    } while ((rTimeMs() - currentTime) < 100);

    if (getButtonState(BUTTON_PROGRAM_ENC) && getButtonState(BUTTON_PRESET_ENC))
    {
        //clear entire display first
        display.clearAll();
        menu.setMenuType(serviceMenu);
        processingEnabled = false;
    }
    else
    {
        processingEnabled = true;
    }

    processingEnabled = true;
}

void Buttons::initHandlers()
{
    for (int i = 0; i < MAX_NUMBER_OF_BUTTONS; i++)
        buttonHandler[i] = NULL;

    //transport controls
    buttonHandler[BUTTON_TRANSPORT_PLAY]   = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_STOP]   = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_RECORD] = handleTransportControl;

    //on/ff
    buttonHandler[BUTTON_ON_OFF_NOTES]      = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_AFTERTOUCH] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_X]          = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_Y]          = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_SPLIT]      = handleOnOff;

    //up/down
    buttonHandler[BUTTON_OCTAVE_DOWN] = handleUpDown;
    buttonHandler[BUTTON_OCTAVE_UP]   = handleUpDown;

    //notes
    buttonHandler[BUTTON_NOTE_C_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_D_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_F_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_G_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_A_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_C]       = handleTonic;
    buttonHandler[BUTTON_NOTE_D]       = handleTonic;
    buttonHandler[BUTTON_NOTE_E]       = handleTonic;
    buttonHandler[BUTTON_NOTE_F]       = handleTonic;
    buttonHandler[BUTTON_NOTE_G]       = handleTonic;
    buttonHandler[BUTTON_NOTE_A]       = handleTonic;
    buttonHandler[BUTTON_NOTE_B]       = handleTonic;

    buttonHandler[BUTTON_PROGRAM_ENC] = handleProgramEncButton;
    buttonHandler[BUTTON_PRESET_ENC]  = handlePresetEncButton;
}

void Buttons::update()
{
    for (int i = 0; i < static_cast<int>(button_t::AMOUNT); i++)
    {
        if (buttonHandler[i] == nullptr)
            continue;

        uint8_t buttonState = _hwa.state(static_cast<button_t>(i));

        if (buttonDebounced(i, buttonState))
            processButton(i, buttonState);
    }
}

/// Checks if requested button is currently pressed.
/// @param [in] buttonID    Button index which is being checked.
/// \returns True if button is pressed, false otherwise.
bool Buttons::getButtonState(uint8_t buttonID)
{
    uint8_t arrayIndex  = buttonID / 8;
    uint8_t buttonIndex = buttonID - 8 * arrayIndex;

    return BIT_READ(buttonPressed[arrayIndex], buttonIndex);
}

/// Updates button state (whether it's pressed or released).
/// @param [in] buttonID    Button index for which state is being changed.
/// @param [in] state       New button state (true/pressed, false/released).
void Buttons::setButtonState(uint8_t buttonID, bool state)
{
    uint8_t arrayIndex  = buttonID / 8;
    uint8_t buttonIndex = buttonID - 8 * arrayIndex;

    BIT_WRITE(buttonPressed[arrayIndex], buttonIndex, state);
}

/// Handles changes in button states.
/// @param [in] buttonID    Button index for which state is being changed.
/// @param [in] state       Current button state.
void Buttons::processButton(uint8_t buttonID, uint8_t state)
{
    //if button state is same as last one, do nothing
    //act on change only
    if (state == getButtonState(buttonID))
        return;

    //update previous button state with current one
    setButtonState(buttonID, state);

    if (processingEnabled)
    {
        (*buttonHandler[buttonID])(buttonID, state);
        //resume button processing
        if (!getButtonEnableState(buttonID) && !getButtonState(buttonID))
        {
            setButtonEnableState(buttonID, true);
        }
    }
}

/// Checks if button reading is stable.
/// Shift old value to the left, append new value and
/// append DEBOUNCE_COMPARE with OR command. If final value is equal to 0xFF or
/// DEBOUNCE_COMPARE, signal is debounced.
/// @param [in] buttonID    Button index which is being checked.
/// @param [in] buttonState Current button state.
/// \returns                True if button reading is stable, false otherwise.
bool Buttons::buttonDebounced(uint8_t buttonID, bool buttonState)
{
    //shift new button reading into previousButtonState
    buttonDebounceCounter[buttonID] = (buttonDebounceCounter[buttonID] << (uint8_t)1) | (uint8_t)buttonState | BUTTON_DEBOUNCE_COMPARE;

    //if button is debounced, return true
    return ((buttonDebounceCounter[buttonID] == BUTTON_DEBOUNCE_COMPARE) || (buttonDebounceCounter[buttonID] == 0xFF));
}

/// Enables or disables specific button.
/// If button is disabled, its handler function is disabled
/// so pressing that button has no effect.
/// @param [in] buttonID    Button index.
/// @param [in] state       New button enable state.
void Buttons::setButtonEnableState(int8_t buttonID, bool state)
{
    uint8_t arrayIndex  = buttonID / 8;
    uint8_t buttonIndex = buttonID - 8 * arrayIndex;

    BIT_WRITE(buttonEnabled[arrayIndex], buttonIndex, state);
}

/// Checks if button is enabled or not.
/// @param [in] buttonID    Button index.
/// \returns True if button is enabled, false otherwise.
bool Buttons::getButtonEnableState(uint8_t buttonID)
{
    uint8_t arrayIndex  = buttonID / 8;
    uint8_t buttonIndex = buttonID - 8 * arrayIndex;

    return BIT_READ(buttonEnabled[arrayIndex], buttonIndex);
}

/// Checks which note corresponds with requested button index.
/// @param [in] buttonNumber    Button index which is being checked.
/// \returns Corresponding MIDI note.
note_t Buttons::getNoteFromButton(uint8_t buttonNumber)
{
    return (note_t)buttonToNoteArray[buttonNumber];
}

/// Changes active transport control mode.
/// @param [in] mode New transport control mode (enumerated type, see transportControlMode_t).
void Buttons::setTransportControlMode(transportControlMode_t mode)
{
    transportControlMode = mode;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID, mode);
}

/// Checks for active transport control mode.
/// \returns Active transport control mode (enumerated type, see transportControlMode_t).
transportControlMode_t Buttons::getTransportControlMode()
{
    return transportControlMode;
}