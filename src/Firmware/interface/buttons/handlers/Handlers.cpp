#include "Handlers.h"

#include "../../pads/Pads.h"
#include "../../MIDIconf.h"
#include "../../buttons/Buttons.h"
#include "../../leds/LEDs.h"
#include "../../lcd/LCD.h"
#include "../../lcd/menu/Menu.h"
#include "../../../database/blocks/GlobalSettings.h"
#ifdef NDEBUG
#include "../../../midi/src/MIDI.h"
#endif

void handleOnOff(uint8_t id, bool state)
{
    if (pads.getEditModeState())
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
            leds.setBlinkState(LED_ON_OFF_NOTES, true, true);
            return;
        }
        else if (id == BUTTON_ON_OFF_SPLIT)
        {
            leds.setBlinkState(LED_ON_OFF_SPLIT, true, true);
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

void handleTransportControl(uint8_t id, bool state)
{
    if (pads.getEditModeState())
        return;

    #ifdef NDEBUG
    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    #endif

    function_t function;

    if (state)
        return;

    switch(id)
    {
        case BUTTON_TRANSPORT_PLAY:
        function = functionPlay;
        #ifdef NDEBUG
        switch(buttons.getTransportControlType())
        {
            case transportCC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_PLAY, 127, 1);
            break;

            case transportMMC:
            sysExArray[4] = 0x02;
            break;

            case transportMMC_CC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_PLAY, 127, 1);
            sysExArray[4] = 0x02;
            break;
        }
        #endif
        leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateFull);
        break;

        case BUTTON_TRANSPORT_STOP:
        function = functionStop;
        #ifdef NDEBUG
        switch(buttons.getTransportControlType())
        {
            case transportCC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_STOP, 127, 1);
            break;

            case transportMMC:
            sysExArray[4] = 0x01;
            break;

            case transportMMC_CC:
            midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_STOP, 127, 1);
            sysExArray[4] = 0x01;
            break;
        }
        #endif
        leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateOff);
        leds.setLEDstate(LED_TRANSPORT_STOP, ledStateOff);
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
                    display.displayScrollCalibrationStatus(false);
                    return;
                }
                else
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                    display.displayScrollCalibrationStatus(true);
                    return;
                }
            }
            else
            {
                if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                    display.displayPressureCalibrationStatus(false);
                    return;
                }
                else
                {
                    leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                    display.displayPressureCalibrationStatus(true);
                    return;
                }
            }
        }
        else
        {
            if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
            {
                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                #ifdef NDEBUG
                switch(buttons.getTransportControlType())
                {
                    case transportCC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
                    break;

                    case transportMMC:
                    sysExArray[4] = 0x07;
                    break;

                    case transportMMC_CC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 0, 1);
                    sysExArray[4] = 0x07;
                    break;
                }
                #endif
            }
            else
            {
                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                #ifdef NDEBUG
                switch(buttons.getTransportControlType())
                {
                    case transportCC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 127, 1);
                    break;

                    case transportMMC:
                    sysExArray[4] = 0x06;
                    break;

                    case transportMMC_CC:
                    midi.sendControlChange(MIDI_SETTING_TRANSPORT_CC_RECORD, 127, 1);
                    sysExArray[4] = 0x06;
                    break;
                }
                #endif
            }
        }
        break;

        default:
        return;
    }

    #ifdef NDEBUG
    if ((buttons.getTransportControlType() == transportMMC) || (buttons.getTransportControlType() == transportMMC_CC))
        midi.sendSysEx(6, sysExArray, true);
    #endif

    display.displayChangeResult(function, 0, globalSetting);
}

void handleUpDown(uint8_t id, bool state)
{
    if (!buttons.getButtonEnableState(id))
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
        buttons.disable(BUTTON_OCTAVE_UP);
        buttons.disable(BUTTON_OCTAVE_DOWN);
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
                leds.displayActiveNoteLEDs(true, lastTouchedPad);
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
                        buttons.disable(BUTTON_ON_OFF_NOTES);

                        //make sure scale shifting is updated on display after message is cleared
                        display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
                        display.displayChangeResult(functionNoteShift, pads.getScaleShiftLevel(), globalSetting);
                        break;

                        default:
                        display.displayError(functionNoteShift, result);
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

void handleTonic(uint8_t id, bool state)
{
    if (state)
        return;

    if (!buttons.getButtonEnableState(id))
        return;

    note_t note = buttons.getTonicFromButton(id);

    if (!pads.getEditModeState())
    {
        changeResult_t result = pads.setTonic(note);

        switch(result)
        {
            case valueChanged:
            case noChange:
            leds.displayActiveNoteLEDs();
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
            leds.displayActiveNoteLEDs(true, lastTouchedPad);
            break;

            default:
            display.displayError(functionNotes, result);
        }
    }
}

void handleProgramEncButton(uint8_t id, bool state)
{
    if (!state)
        return;

    if (!menu.isMenuDisplayed())
    {
        if (pads.getNumberOfPressedPads())
        {
            display.displayError(functionMenu, releasePads);
        }
        else
        {
            menu.show(userMenu);
            #ifdef DEBUG
            printf_P(PSTR("Entering user menu\n"));
            #endif
        }
    }
    else
    {
        //enter
        menu.confirmOption(true);
    }
}

void handlePresetEncButton(uint8_t id, bool state)
{
    if (!state)
        return;

    if (menu.isMenuDisplayed())
    {
        //return
        menu.confirmOption(false);
        return;
    }

    buttons.setModifierState(!buttons.getModifierState());

    if (buttons.getModifierState())
    {
        //midi channel change mode, display on lcd
        display.displayChangeResult(functionChannel, pads.getMIDIchannel(pads.getLastTouchedPad()), pads.getSplitState() ? singlePadSetting : globalSetting);
        display.setMessageTime(INFINITE_MESSAGE_TIME);
    }
    else
    {
        //return
        display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
    }
}