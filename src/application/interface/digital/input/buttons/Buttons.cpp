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
#include "../../../../database/Database.h"
#include "../../../lcd/LCD.h"
#include "../../../lcd/menu/Menu.h"
#include "handlers/Handlers.h"

///
/// \brief External definition for button handler functions.
///
extern void             (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

///
/// \brief Array holding information about whether specific button is enabled or not.
/// Each button has 1 bit in order to reserve space.
///
uint8_t                 buttonEnabled[MAX_NUMBER_OF_BUTTONS/8+1];

///
/// \brief Flag holding information about whether buttons are handled or not.
/// If set to false, pressing buttons will result in no action being taken.
///
bool                    processingEnabled;

///
/// \brief Holds currently active transport control mode.
///
transportControlMode_t  transportControlMode;

///
/// \brief Array holding mapping of notes to specific buttons.
///
uint8_t                 buttonToNoteArray[MAX_NUMBER_OF_BUTTONS];

///
/// \brief Array holding current state of buttons.
/// Each button has 1 bit in order to reserve space.
///
uint8_t                 buttonPressed[MAX_NUMBER_OF_BUTTONS/8+1];

///
/// \brief Array holding debounce count for all buttons to avoid incorrect state detection.
///
uint8_t                 buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];

///
/// \brief Default constructor.
///
Buttons::Buttons()
{
    processingEnabled = true;

    //enable all buttons
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS/8+1; i++)
        buttonEnabled[i] = 0xFF;

    //map notes to buttons
    buttonToNoteArray[BUTTON_NOTE_C] = C;
    buttonToNoteArray[BUTTON_NOTE_C_SHARP] = C_SHARP;
    buttonToNoteArray[BUTTON_NOTE_D] = D;
    buttonToNoteArray[BUTTON_NOTE_D_SHARP] = D_SHARP;
    buttonToNoteArray[BUTTON_NOTE_E] = E;
    buttonToNoteArray[BUTTON_NOTE_F] = F;
    buttonToNoteArray[BUTTON_NOTE_F_SHARP] = F_SHARP;
    buttonToNoteArray[BUTTON_NOTE_G] = G;
    buttonToNoteArray[BUTTON_NOTE_G_SHARP] = G_SHARP;
    buttonToNoteArray[BUTTON_NOTE_A] = A;
    buttonToNoteArray[BUTTON_NOTE_A_SHARP] = A_SHARP;
    buttonToNoteArray[BUTTON_NOTE_B] = B;
}

///
/// \brief Initializes button variables to their default states.
///
void Buttons::init()
{
    initHandlers_buttons();
    uint32_t currentTime = rTimeMs();
    processingEnabled = false;
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
            board.continueDigitalInReadout();
        }
    }
    while ((rTimeMs() - currentTime) < 100);

    if (getButtonState(BUTTON_PROGRAM_ENC) && getButtonState(BUTTON_PRESET_ENC))
    {
        menu.show(serviceMenu);
        processingEnabled = false;
    }
    else
    {
        processingEnabled = true;
    }

    processingEnabled = true;
}

///
/// \brief Continuously reads inputs from buttons and acts if necessary.
///
void Buttons::update()
{
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        if (buttonHandler[i] == NULL)
            continue;

        uint8_t buttonState = board.getButtonState(i);

        if (buttonDebounced(i, buttonState))
            processButton(i, buttonState);
    }
}

///
/// \brief Checks if requested button is currently pressed.
/// @param [in] buttonID    Button index which is being checked.
/// \returns True if button is pressed, false otherwise.
///
bool Buttons::getButtonState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return BIT_READ(buttonPressed[arrayIndex], buttonIndex);
}

///
/// \brief Updates button state (whether it's pressed or released).
/// @param [in] buttonID    Button index for which state is being changed.
/// @param [in] state       New button state (true/pressed, false/released).
///
void Buttons::setButtonState(uint8_t buttonID, bool state)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    BIT_WRITE(buttonPressed[arrayIndex], buttonIndex, state);
}

///
/// \brief Handles changes in button states.
/// @param [in] buttonID    Button index for which state is being changed.
/// @param [in] state       Current button state.
///
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

///
/// \brief Checks if button reading is stable.
/// Shift old value to the left, append new value and
/// append DEBOUNCE_COMPARE with OR command. If final value is equal to 0xFF or
/// DEBOUNCE_COMPARE, signal is debounced.
/// @param [in] buttonID    Button index which is being checked.
/// @param [in] state       Current button state.
/// \returns                True if button reading is stable, false otherwise.
///
bool Buttons::buttonDebounced(uint8_t buttonID, bool buttonState)
{
    //shift new button reading into previousButtonState
    buttonDebounceCounter[buttonID] = (buttonDebounceCounter[buttonID] << (uint8_t)1) | (uint8_t)buttonState | BUTTON_DEBOUNCE_COMPARE;

    //if button is debounced, return true
    return ((buttonDebounceCounter[buttonID] == BUTTON_DEBOUNCE_COMPARE) || (buttonDebounceCounter[buttonID] == 0xFF));
}

///
/// \brief Enables or disables specific button.
/// If button is disabled, its handler function is disabled
/// so pressing that button has no effect.
/// @param [in] buttonID    Button index.
/// @param [in] state       New button enable state.
///
void Buttons::setButtonEnableState(int8_t buttonID, bool state)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    BIT_WRITE(buttonEnabled[arrayIndex], buttonIndex, false);
}

///
/// \brief Checks if button is enabled or not.
/// @param [in] buttonID    Button index.
/// \returns True if button is enabled, false otherwise.
///
bool Buttons::getButtonEnableState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return BIT_READ(buttonEnabled[arrayIndex], buttonIndex);
}

///
/// \brief Checks which note corresponds with requested button index.
/// @param [in] buttonNumber    Button index which is being checked.
/// \returns Corresponding MIDI note.
///
note_t Buttons::getNoteFromButton(uint8_t buttonNumber)
{
    return (note_t)buttonToNoteArray[buttonNumber];
}

///
/// \brief Changes active transport control mode.
/// @param [in] mode New transport control mode (enumerated type, see transportControlMode_t).
///
void Buttons::setTransportControlMode(transportControlMode_t mode)
{
    transportControlMode = mode;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID, mode);
}

///
/// \brief Checks for active transport control mode.
/// \returns Active transport control mode (enumerated type, see transportControlMode_t).
///
transportControlMode_t Buttons::getTransportControlMode()
{
    return transportControlMode;
}

///
/// \brief Buttons class instance.
///
Buttons buttons;
