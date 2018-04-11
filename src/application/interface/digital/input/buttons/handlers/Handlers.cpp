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

#include "Handlers.h"
#include "../../../../analog/pads/Pads.h"
#include "../../../../MIDIconf.h"
#include "../../../../digital/input/buttons/Buttons.h"
#include "../../../../digital/input/encoders/Encoders.h"
#include "../../../../digital/output/leds/LEDs.h"
#include "../../../../display/Display.h"
#include "../../../../display/menu/Menu.h"
#include "../../../../../database/blocks/GlobalSettings.h"

///
/// \brief Handler for on/off buttons (split, notes, aftertouch, X and Y).
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handleOnOff(uint8_t id, bool state)
{
    if (pads.getEditModeState())
        return;

    if (menu.isMenuDisplayed())
        return;

    if (!buttons.getButtonEnableState(id))
    {
        //button disabled
        if (!state)
        {
            switch(id)
            {
                case BUTTON_ON_OFF_NOTES:
                //stop blinking
                leds.setBlinkState(LED_ON_OFF_NOTES, false);
                //make sure led state is correct
                //hack: notes on/off can only be ledStateOff (0) or ledStateFull (2) - multiply by 2
                leds.setLEDstate(LED_ON_OFF_NOTES, (ledState_t)(pads.getMIDISendState(pads.getLastTouchedPad(), functionOnOffNotes)*2));
                break;

                case BUTTON_ON_OFF_SPLIT:
                //stop blinking
                leds.setBlinkState(LED_ON_OFF_SPLIT, false);
                //make sure led state is correct
                //hack: split can only be ledStateOff (0) or ledStateFull (2) - multiply by 2
                leds.setLEDstate(LED_ON_OFF_SPLIT, (ledState_t)(pads.getSplitState()*2));
                break;

                default:
                return;
            }
        }

        return;
    }

    if (state)
    {
        if (id == BUTTON_ON_OFF_NOTES)
        {
            leds.setLEDstate(LED_ON_OFF_NOTES, ledStateFull);
            leds.setBlinkState(LED_ON_OFF_NOTES, true);
            return;
        }
        else if (id == BUTTON_ON_OFF_SPLIT)
        {
            leds.setLEDstate(LED_ON_OFF_SPLIT, ledStateFull);
            leds.setBlinkState(LED_ON_OFF_SPLIT, true);
            return;
        }
        else
        {
            return;
        }
    }

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    ledState_t ledState = ledStateOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    changeResult_t result;
    function_t function;

    switch (id)
    {
        case BUTTON_ON_OFF_NOTES:
        ledNumber = LED_ON_OFF_NOTES;
        function = functionOnOffNotes;
        result = pads.setMIDISendState(functionOnOffNotes, !pads.getMIDISendState(lastTouchedPad, functionOnOffNotes));

        if (pads.getMIDISendState(lastTouchedPad, functionOnOffNotes))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        leds.setBlinkState(ledNumber, false);
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        result = pads.setMIDISendState(functionOnOffAftertouch, !pads.getMIDISendState(lastTouchedPad, functionOnOffAftertouch));
        ledNumber = LED_ON_OFF_AFTERTOUCH;
        function = functionOnOffAftertouch;
        if (pads.getMIDISendState(lastTouchedPad, functionOnOffAftertouch))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_X:
        result = pads.setMIDISendState(functionOnOffX, !pads.getMIDISendState(lastTouchedPad, functionOnOffX));
        ledNumber = LED_ON_OFF_X;
        function = functionOnOffX;

        if (pads.getMIDISendState(lastTouchedPad, functionOnOffX))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_Y:
        result = pads.setMIDISendState(functionOnOffY, !pads.getMIDISendState(lastTouchedPad, functionOnOffY));
        ledNumber = LED_ON_OFF_Y;
        function = functionOnOffY;

        if (pads.getMIDISendState(lastTouchedPad, functionOnOffY))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        ledNumber = LED_ON_OFF_SPLIT;
        function = functionOnOffSplit;
        result = pads.setSplitState(!pads.getSplitState());
        pads.getSplitState() ? ledState = ledStateFull : ledState = ledStateOff;
        leds.setBlinkState(ledNumber, false);
        break;

        default:
        return;
    }

    if (result == valueChanged)
    {
        leds.setLEDstate(ledNumber, ledState);
        if (function == functionOnOffSplit)
            display.displayChangeResult(function, (bool)ledState, globalSetting);
        else
            display.displayChangeResult(function, (bool)ledState, pads.getSplitState() ? singlePadSetting : allPadsSetting);
    }
    else
    {
        display.displayError(function, result);
    }
}

///
/// \brief Handler for transport control buttons (play, stop and record).
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handleTransportControl(uint8_t id, bool state)
{
    if (pads.getEditModeState())
        return;

    if (menu.isMenuDisplayed())
    {
        //when in menu, allow record button only during calibration
        if (pads.isCalibrationEnabled())
        {
            if (id != BUTTON_TRANSPORT_RECORD)
                return;
        }
        else
        {
            return;
        }
    }

    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control

    function_t function;

    if (state)
        return;

    bool recordOff = false;

    switch(id)
    {
        case BUTTON_TRANSPORT_PLAY:
        function = functionPlay;
        switch(buttons.getTransportControlMode())
        {
            case transportCC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_PLAY, 127, 1);
            break;

            case transportMMC:
            sysExArray[4] = 0x02;
            midi.sendSysEx(6, sysExArray, true);
            break;

            case transportMMC_CC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_PLAY, 127, 1);
            sysExArray[4] = 0x02;
            midi.sendSysEx(6, sysExArray, true);
            break;
        }
        leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateFull);
        break;

        case BUTTON_TRANSPORT_STOP:
        function = functionStop;
        recordOff = leds.getLEDstate(LED_TRANSPORT_RECORD);
        switch(buttons.getTransportControlMode())
        {
            case transportCC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_STOP, 127, 1);
            if (recordOff)
                midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
            break;

            case transportMMC:
            sysExArray[4] = 0x01;
            midi.sendSysEx(6, sysExArray, true);

            if (recordOff)
            {
                sysExArray[4] = 0x07;
                midi.sendSysEx(6, sysExArray, true);
            }
            break;

            case transportMMC_CC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_STOP, 127, 1);
            sysExArray[4] = 0x01;
            midi.sendSysEx(6, sysExArray, true);

            if (recordOff)
            {
                midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
                sysExArray[4] = 0x07;
                midi.sendSysEx(6, sysExArray, true);
            }
            break;
        }
        leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateOff);
        leds.setLEDstate(LED_TRANSPORT_STOP, ledStateOff);

        if (recordOff)
            leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
        break;

        case BUTTON_TRANSPORT_RECORD:
        function = functionRecord;
        if (pads.isCalibrationEnabled())
        {
            if ((pads.getCalibrationMode() == coordinateX) || (pads.getCalibrationMode() == coordinateY))
            {
                if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                    display.setupCalibrationScreen(pads.getCalibrationMode(), false);
                    return;
                }
                else
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                    display.setupCalibrationScreen(pads.getCalibrationMode(), true);
                    return;
                }
            }
            else
            {
                if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                    display.displayCalibrationStatus(pads.getCalibrationMode(), false);
                    return;
                }
                else
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                    display.displayCalibrationStatus(pads.getCalibrationMode(), true);
                    return;
                }
            }
        }
        else
        {
            if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
            {
                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                switch(buttons.getTransportControlMode())
                {
                    case transportCC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
                    break;

                    case transportMMC:
                    sysExArray[4] = 0x07;
                    midi.sendSysEx(6, sysExArray, true);
                    break;

                    case transportMMC_CC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
                    sysExArray[4] = 0x07;
                    midi.sendSysEx(6, sysExArray, true);
                    break;
                }
            }
            else
            {
                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                switch(buttons.getTransportControlMode())
                {
                    case transportCC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 127, 1);
                    break;

                    case transportMMC:
                    sysExArray[4] = 0x06;
                    midi.sendSysEx(6, sysExArray, true);
                    break;

                    case transportMMC_CC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 127, 1);
                    sysExArray[4] = 0x06;
                    midi.sendSysEx(6, sysExArray, true);
                    break;
                }
            }
        }
        break;

        default:
        return;
    }

    //only record can have on/off state, ignore for other controls
    display.displayChangeResult(function, leds.getLEDstate(LED_TRANSPORT_RECORD), globalSetting);
}

///
/// \brief Handler for up/down buttons.
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handleUpDown(uint8_t id, bool state)
{
    if (!buttons.getButtonEnableState(id))
        return;

    if (menu.isMenuDisplayed())
        return;

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    bool direction = false;

    if (id == BUTTON_OCTAVE_UP)
        direction = true;

    if (buttons.getButtonState(BUTTON_OCTAVE_DOWN) && buttons.getButtonState(BUTTON_OCTAVE_UP))
    {
        #ifdef DEBUG
        printf_P(PSTR("Trying to enter pad edit mode...\n"));
        #endif

        if (!pads.getEditModeState())
        {
            //try to enter pad edit mode
            changeResult_t result = pads.setEditModeState(true, pads.getLastTouchedPad());

            if (result == valueChanged)
            {
                #ifdef DEBUG
                printf_P(PSTR("Pad edit mode entered.\n"));
                #endif

                leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                leds.setLEDstate(LED_OCTAVE_UP, ledStateFull);
            }
            else
            {
                display.displayError(functionPadEditMode, result);
                leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);
            }
        }
        else
        {
            //exit pad edit mode
            leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
            leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);
            pads.setEditModeState(false);
        }

        //temporarily disable buttons
        buttons.setButtonEnableState(BUTTON_OCTAVE_UP, false);
        buttons.setButtonEnableState(BUTTON_OCTAVE_DOWN, false);
    }
    else
    {
        bool editMode = pads.getEditModeState();

        switch(editMode)
        {
            case true:
            //pad edit mode
            switch(state)
            {
                case false:
                pads.setOctave(direction ? pads.getOctave(true)+1 : pads.getOctave(true)-1, true);
                display.setupPadEditScreen(pads.getLastTouchedPad()+1, pads.getOctave(true));
                pads.setActiveNoteLEDs(true, lastTouchedPad);
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                break;

                case true:
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                break;
            }
            break;

            case false:
            if (!buttons.getButtonState(BUTTON_ON_OFF_NOTES))
            {
                //shift entire octave up or down
                if (!state)
                {
                    changeResult_t result = pads.setOctave(direction ? pads.getOctave()+1 : pads.getOctave()-1);

                    switch(result)
                    {
                        case noChange:
                        case valueChanged:
                        display.displayChangeResult(functionOctave, pads.getOctave(), globalSetting);
                        break;

                        default:
                        display.displayError(functionOctave, result);
                        break;
                    }

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                }
                else
                {
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateDim);
                }
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_NOTES))
            {
                //shift single note, but only in predefined presets
                if (!state)
                {
                    int8_t shiftLevel = direction ? 1 : -1;
                    changeResult_t result = pads.setScaleShiftLevel(pads.getScaleShiftLevel()+shiftLevel);

                    switch(result)
                    {
                        case noChange:
                        case valueChanged:
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                        buttons.setButtonEnableState(BUTTON_ON_OFF_NOTES, false);

                        //make sure scale shifting is updated on display after message is cleared
                        display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
                        display.displayChangeResult(functionNoteShift, pads.getScaleShiftLevel(), globalSetting);
                        break;

                        default:
                        display.displayError(functionNoteShift, result);
                        break;
                    }
                }
                else
                {
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                }
            }
            break;
        }
    }
}

///
/// \brief Handler for note buttons.
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handleTonic(uint8_t id, bool state)
{
    if (state)
        return;

    if (!buttons.getButtonEnableState(id))
        return;

    if (menu.isMenuDisplayed())
        return;

    note_t note = buttons.getNoteFromButton(id);

    if (!pads.getEditModeState())
    {
        changeResult_t result = pads.setTonic(note);

        switch(result)
        {
            case valueChanged:
            case noChange:
            pads.setActiveNoteLEDs(false, 0);
            //make sure tonic is updated on display after message is cleared
            display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
            display.displayChangeResult(functionNotes, pads.getTonic(), globalSetting);
            break;

            default:
            display.displayError(functionNotes, result);
            break;
        }
    }
    else
    {
        //add note to pad
        uint8_t lastTouchedPad = pads.getLastTouchedPad();
        changeResult_t result = pads.setPadNote(lastTouchedPad, note);

        switch(result)
        {
            case noChange:
            case valueChanged:
            display.displayActivePadNotes();
            pads.setActiveNoteLEDs(true, lastTouchedPad);
            break;

            default:
            display.displayError(functionNotes, result);
        }
    }
}

///
/// \brief Handler for button located on program encoder.
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handleProgramEncButton(uint8_t id, bool state)
{
    if (!state)
        return;

    if (pads.getEditModeState())
        return;

    if (!menu.isMenuDisplayed())
    {
        if (pads.getNumberOfPressedPads())
        {
            display.displayError(functionMenu, releasePads);
        }
        else
        {
            //clear entire display first
            display.clearAll();
            menu.setMenuType(userMenu);
            #ifdef DEBUG
            printf_P(PSTR("Entering user menu.\n"));
            #endif
        }
    }
    else
    {
        //enter
        menu.confirmOption(true);
    }
}

///
/// \brief Handler for button located on preset encoder.
/// @param [in] id      Button index.
/// @param [in] state   Button state (true/pressed, false/released).
///
void handlePresetEncButton(uint8_t id, bool state)
{
    if (!state)
        return;

    if (pads.getEditModeState())
        return;

    if (menu.isMenuDisplayed())
    {
        //return
        menu.confirmOption(false);
        //always disable calibration on return
        if (pads.isCalibrationEnabled())
            pads.setCalibrationMode(false);

        //check if we exited from menu
        if (!menu.isMenuDisplayed())
        {
            //disable calibration if active
            pads.setCalibrationMode(false);
            //exit menu and restore initial state
            display.setupHomeScreen();
            //re-enable buttons
            for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
                buttons.setButtonEnableState(i, true);
        }

        return;
    }

    encoders.setMIDIchannelPresetEncMode(true);
    //midi channel change mode, display on display
    display.displayChangeResult(functionChannel, pads.getMIDIchannel(pads.getLastTouchedPad()), pads.getSplitState() ? singlePadSetting : globalSetting);
}
