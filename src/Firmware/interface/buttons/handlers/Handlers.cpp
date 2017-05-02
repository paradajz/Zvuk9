#include "Handlers.h"

#include "../../pads/Pads.h"
#include "../../buttons/Buttons.h"
#include "../../leds/LEDs.h"
#include "../../lcd/LCD.h"
#include "../../lcd/menu/Menu.h"
#include "../../../database/blocks/GlobalSettings.h"
#ifdef NDEBUG
#include "../../../midi/MIDI.h"
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
                leds.setLEDstate(LED_ON_OFF_NOTES, (ledState_t)(pads.getMIDISendState(onOff_notes, pads.getLastTouchedPad())*2));
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
    onOff_t lcdMessageType;
    ledState_t ledState = ledStateOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (id)
    {
        case BUTTON_ON_OFF_NOTES:
        ledNumber = LED_ON_OFF_NOTES;

        pads.setMIDISendState(onOff_notes, !pads.getMIDISendState(onOff_notes, lastTouchedPad));
        lcdMessageType = onOff_notes;

        if (pads.getMIDISendState(onOff_notes, lastTouchedPad))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        leds.setBlinkState(ledNumber, false);
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        pads.setMIDISendState(onOff_aftertouch, !pads.getMIDISendState(onOff_aftertouch, lastTouchedPad));
        lcdMessageType = onOff_aftertouch;
        ledNumber = LED_ON_OFF_AFTERTOUCH;

        if (pads.getMIDISendState(onOff_aftertouch, lastTouchedPad))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_X:
        pads.setMIDISendState(onOff_x, !pads.getMIDISendState(onOff_x, lastTouchedPad));
        lcdMessageType = onOff_x;
        ledNumber = LED_ON_OFF_X;

        if (pads.getMIDISendState(onOff_x, lastTouchedPad))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_Y:
        pads.setMIDISendState(onOff_y, !pads.getMIDISendState(onOff_y, lastTouchedPad));
        lcdMessageType = onOff_y;
        ledNumber = LED_ON_OFF_Y;

        if (pads.getMIDISendState(onOff_y, lastTouchedPad))
            ledState = ledStateFull;
        else
            ledState = ledStateOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        ledNumber = LED_ON_OFF_SPLIT;
        pads.setSplitState(!pads.getSplitState());
        lcdMessageType = onOff_split;
        pads.getSplitState() ? ledState = ledStateFull : ledState = ledStateOff;
        leds.setBlinkState(ledNumber, false);
        break;

        default:
        return;
    }

    leds.setLEDstate(ledNumber, ledState);
    display.displayOnOff(lcdMessageType, pads.getSplitState(), (uint8_t)ledState, lastTouchedPad+1);
}

void handleTransportControl(uint8_t id, bool state)
{
    if (pads.getEditModeState())
        return;

    if (!buttons.getButtonEnableState(id))
        return;

    #ifdef NDEBUG
    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    #endif

    transportControl_t type;

    if (state)
        return;

    switch(id)
    {
        case BUTTON_TRANSPORT_PLAY:
        type = transportPlay;
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
        type = transportStop;
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
        if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull)
        {
            leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
            type = transportRecordOff;
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
            type = transportRecordOn;
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
        break;

        default:
        return;
    }

    #ifdef NDEBUG
    if ((buttons.getTransportControlType() == transportMMC) || (buttons.getTransportControlType() == transportMMC_CC))
        midi.sendSysEx(6, sysExArray, true);
    #endif

    display.displayTransportControl(type);
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
        printf_P(PSTR("Trying to enter pad edit mode.\n"));
        #endif

        //try to enter pad edit mode
        if (pads.isUserScale(pads.getActiveScale()))
        {
            if (!pads.getEditModeState())
            {
                //check if last touched pad is pressed
                if (pads.isPadPressed(pads.getLastTouchedPad()))
                {
                    display.displayEditModeNotAllowed(padNotReleased);
                }
                else
                {
                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #ifdef DEBUG
                    printf_P(PSTR("Pad edit mode\n"));
                    #endif
                    pads.setEditModeState(true, pads.getLastTouchedPad());

                    leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                    leds.setLEDstate(LED_OCTAVE_UP, ledStateFull);
                }
            }
            else
            {
                leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);
                pads.setEditModeState(false);
            }
        }
        else
        {
            display.displayEditModeNotAllowed(notUserPreset);
            leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
            leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);
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
            switch(state)
            {
                case false:
                pads.changeActiveOctave(direction);
                display.displayActiveOctave(normalizeOctave(pads.getActiveOctave()));
                leds.displayActiveNoteLEDs(true, lastTouchedPad);
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                break;

                case true:
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                break;
            }
            break;

            case false:
            if (pads.isUserScale(pads.getActiveScale()) || (pads.isPredefinedScale(pads.getActiveScale()) && !buttons.getButtonState(BUTTON_ON_OFF_NOTES)))
            {
                //shift entire octave up or down
                if (!state)
                {
                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    uint8_t activeOctave = pads.getActiveOctave();
                    display.displayNoteChange(shiftResult, octaveChange, normalizeOctave(activeOctave));
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
                    if (pads.isPadPressed(lastTouchedPad))
                    {
                        display.displayEditModeNotAllowed(padNotReleased);
                        return;
                    }

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                    buttons.disable(BUTTON_ON_OFF_NOTES);

                    changeOutput_t shiftResult = pads.shiftNote(direction);
                    display.displayNoteChange(shiftResult, noteShift, pads.getNoteShiftLevel());
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
        changeOutput_t result = pads.setTonic(note);
        note_t activeTonic = pads.getActiveTonic();

        switch(result)
        {
            case outputChanged:
            case noChange:
            leds.displayActiveNoteLEDs();
            display.displayNoteChange(result, tonicChange, activeTonic);
            break;

            case notAllowed:
            display.displayNoNotesError();
            break;

            default:
            break;
        }
    }
    else
    {
        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        pads.assignPadNote(pad, note);
        pads.displayActivePadNotes(pad);
        leds.displayActiveNoteLEDs(true, pad);
    }
}

void handleProgramEncButton(uint8_t id, bool state)
{
    if (state && !menu.menuDisplayed())
    {
        menu.displayMenu(userMenu);
        #ifdef DEBUG
        printf_P(PSTR("Entering user menu\n"));
        #endif
    }
}

void handlePresetEncButton(uint8_t id, bool state)
{
    if (!state)
        return;

    //make sure movement is enabled ion case we just exited menu
    menu.resetExitTime();

    buttons.setModifierState(!buttons.getModifierState());

    if (buttons.getModifierState())
    {
        //midi channel change mode, display on lcd
        uint8_t lastTouchedPad = pads.getLastTouchedPad();
        display.displayMIDIchannelChange(pads.getMIDIchannel(lastTouchedPad), pads.getSplitState(), lastTouchedPad+1);
    }
    else
    {
        //return
        display.displayProgramAndScale(pads.getActiveProgram()+1, pads.getActiveScale());
    }
}