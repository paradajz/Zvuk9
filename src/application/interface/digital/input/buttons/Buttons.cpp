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

#include "Config.h"
#include "../../../../database/Database.h"
#include "../../../lcd/LCD.h"
#include "../../../lcd/menu/Menu.h"
#include "handlers/Handlers.h"
#include "../encoders/Encoders.h"
#include "../../../analog/pads/Pads.h"
#include "../../../digital/output/leds/LEDs.h"

extern void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

Buttons::Buttons()
{
    //default constructor
    lastCheckTime               = 0;
    processingEnabled           = true;

    //enable all buttons
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS/8+1; i++)
        buttonEnabled[i] = 0xFF;
}

void Buttons::init()
{
    mapButtonsToNotes();
    initHandlers_buttons();
    uint32_t currentTime = rTimeMs();
    processingEnabled = false;
    transportControlType = (transportControlType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID);

    //read buttons for 0.1 seconds
    // do
    // {
    //     //read all buttons without activating event handlers
    //     update();
    // }
    // while ((rTimeMs() - currentTime) < 100);

    // if (getButtonState(BUTTON_PROGRAM_ENC) && getButtonState(BUTTON_PRESET_ENC))
    // {
    //     menu.show(serviceMenu);
    //     disable();
    // }
    // else
    // {
    //     processingEnabled = true;
    // }

    processingEnabled = true;
}

bool Buttons::getButtonState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return BIT_READ(buttonPressed[arrayIndex], buttonIndex);
}

void Buttons::setButtonState(uint8_t buttonID, bool state)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    BIT_WRITE(buttonPressed[arrayIndex], buttonIndex, state);
}

void Buttons::processButton(uint8_t buttonID, uint8_t state)
{
    //if button state is same as last one, do nothing
    //act on change only
    if (state == getButtonState(buttonID))
        return;

    //update previous button state with current one
    setButtonState(buttonID, state);
    lastPressedButton = buttonID;

    if (processingEnabled)
    {
        (*buttonHandler[buttonID])(buttonID, state);
        //resume button processing
        if (!getButtonEnableState(buttonID) && !getButtonState(buttonID))
        {
            enable(buttonID);
        }
    }
}

void Buttons::update()
{
    uint8_t buttonState;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        if (buttonHandler[i] == NULL)
            continue;

        buttonState = board.getButtonState(i);

        if (buttonDebounced(i, buttonState))
            processButton(i, buttonState);
    }

    if (getMIDIchannelEnc() && (display.getActiveTextType() == lcdtext_still))
        setMIDIchannelEnc(false);
}

bool Buttons::buttonDebounced(uint8_t buttonID, bool buttonState)
{
    //shift new button reading into previousButtonState
    buttonDebounceCounter[buttonID] = (buttonDebounceCounter[buttonID] << (uint8_t)1) | (uint8_t)buttonState | BUTTON_DEBOUNCE_COMPARE;

    //if button is debounced, return true
    return ((buttonDebounceCounter[buttonID] == BUTTON_DEBOUNCE_COMPARE) || (buttonDebounceCounter[buttonID] == 0xFF));
}

void Buttons::enable(int8_t buttonID)
{
    if (buttonID == -1)
    {
        processingEnabled = true;
    }
    else
    {
        uint8_t arrayIndex = buttonID/8;
        uint8_t buttonIndex = buttonID - 8*arrayIndex;

        BIT_WRITE(buttonEnabled[arrayIndex], buttonIndex, true);
    }
}

void Buttons::disable(int8_t buttonID)
{
    if (buttonID == -1)
    {
        processingEnabled = false;
    }
    else
    {
        uint8_t arrayIndex = buttonID/8;
        uint8_t buttonIndex = buttonID - 8*arrayIndex;

        BIT_WRITE(buttonEnabled[arrayIndex], buttonIndex, false);
    }
}

bool Buttons::getButtonEnableState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return BIT_READ(buttonEnabled[arrayIndex], buttonIndex);
}

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)
{
    return (note_t)buttonToNoteArray[buttonNumber];
}

void Buttons::setTransportControlType(transportControlType_t type)
{
    transportControlType = type;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID, type);
}

transportControlType_t Buttons::getTransportControlType()
{
    return transportControlType;
}

uint8_t Buttons::getLastPressedButton()
{
    return lastPressedButton;
}

void Buttons::setMIDIchannelEnc(bool state)
{
    midiChannelEncState = state;
}

bool Buttons::getMIDIchannelEnc()
{
    return midiChannelEncState;
}

Buttons buttons;
