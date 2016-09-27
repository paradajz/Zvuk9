#include "Buttons.h"

#ifdef BUTTONS_H_

#ifdef NDEBUG
#include "../../midi/MIDI.h"
#endif
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)   {

    switch(buttonNumber)    {

        case BUTTON_NOTE_C:
        return C;

        case BUTTON_NOTE_C_SHARP:
        return C_SHARP;

        case BUTTON_NOTE_D:
        return D;

        case BUTTON_NOTE_D_SHARP:
        return D_SHARP;

        case BUTTON_NOTE_E:
        return E;

        case BUTTON_NOTE_F:
        return F;

        case BUTTON_NOTE_F_SHARP:
        return F_SHARP;

        case BUTTON_NOTE_G:
        return G;

        case BUTTON_NOTE_G_SHARP:
        return G_SHARP;

        case BUTTON_NOTE_A:
        return A;

        case BUTTON_NOTE_A_SHARP:
        return A_SHARP;

        case BUTTON_NOTE_B:
        return B;

    }   return MIDI_NOTES;   //impossible case

}

void Buttons::handleOnOffEvent(uint8_t buttonNumber, bool state)    {

    if (pads.editModeActive()) return;

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    onOff_t lcdMessageType;
    ledState_t ledState = ledStateOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        ledNumber = LED_ON_OFF_NOTES;
        if (!state)  {

            pads.midiSendOnOff(onOff_notes);
            lcdMessageType = onOff_notes;
            if (pads.getMIDISendState(onOff_notes, lastTouchedPad))
                ledState = ledStateFull;
            else
                ledState = ledStateOff;

        }   else {

            leds.setLEDstate(ledNumber, pads.getMIDISendState(onOff_notes, lastTouchedPad) ? ledStateFull : ledStateOff, true);
            return;

        }
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        if (!state)  {

            pads.midiSendOnOff(onOff_aftertouch);
            lcdMessageType = onOff_aftertouch;
            ledNumber = LED_ON_OFF_AFTERTOUCH;
            if (pads.getMIDISendState(onOff_aftertouch, lastTouchedPad)) ledState = ledStateFull; else ledState = ledStateOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_X:
        if (!state)  {

            pads.midiSendOnOff(onOff_x);
            lcdMessageType = onOff_x;
            ledNumber = LED_ON_OFF_X;
            if (pads.getMIDISendState(onOff_x, lastTouchedPad)) ledState = ledStateFull; else ledState = ledStateOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_Y:
        if (!state)  {

            pads.midiSendOnOff(onOff_y);
            lcdMessageType = onOff_y;
            ledNumber = LED_ON_OFF_Y;
            if (pads.getMIDISendState(onOff_y, lastTouchedPad)) ledState = ledStateFull; else ledState = ledStateOff;

        }   else return;
        break;

        case BUTTON_ON_OFF_SPLIT:
        ledNumber = LED_ON_OFF_SPLIT;
        if (!state)  {

            pads.splitOnOff();
            lcdMessageType = onOff_split;
            pads.getSplitState() ? ledState = ledStateFull : ledState = ledStateOff;

        }   else {

            leds.setLEDstate(ledNumber, pads.getSplitState() ? ledStateFull : ledStateOff, true);
            return;

        }
        break;

        default:
        return;

    }

    leds.setLEDstate(ledNumber, ledState);
    display.displayOnOff(lcdMessageType, pads.getSplitState(), (uint8_t)ledState, lastTouchedPad+1);

}

void Buttons::handleTransportControlEvent(uint8_t buttonNumber, bool state)  {

    if (pads.editModeActive()) return;

    #ifdef NDEBUG
    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    #endif

    transportControl_t type;

    if (!state)  {

        switch(buttonNumber)    {

            case BUTTON_TRANSPORT_PLAY:
            type = transportPlay;
            #ifdef NDEBUG
            sysExArray[4] = 0x02;
            #endif
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateFull);
            break;

            case BUTTON_TRANSPORT_STOP:
            type = transportStop;
            #ifdef NDEBUG
            sysExArray[4] = 0x01;
            #endif
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledStateOff);
            leds.setLEDstate(LED_TRANSPORT_STOP, ledStateOff);
            break;

            case BUTTON_TRANSPORT_RECORD:
            if (leds.getLEDstate(LED_TRANSPORT_RECORD) == ledStateFull) {

                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateOff);
                type = transportRecordOff;
                #ifdef NDEBUG
                sysExArray[4] = 0x07;
                #endif

            }   else {

                leds.setLEDstate(LED_TRANSPORT_RECORD, ledStateFull);
                type = transportRecordOn;
                #ifdef NDEBUG
                sysExArray[4] = 0x06;
                #endif

            }
            break;

            default:
            return;

        }

    }   else return;

    #ifdef NDEBUG
        midi.sendSysEx(6, sysExArray, true);
    #endif

    display.displayTransportControl(type);

}

void Buttons::handleTonicEvent(note_t note, bool state) {

    if (state) return;

    if (!pads.editModeActive())   {

        changeOutput_t result = pads.setTonic(note);
        note_t activeTonic = pads.getActiveTonic();

        switch(result)  {

            case outputChanged:
            leds.displayActiveNoteLEDs();
            break;

            default:
            break;

        }

        //always do this
        display.displayNoteChange(result, tonicChange, activeTonic);

    }   else {

        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        pads.assignPadNote(pad, note);
        pads.displayActivePadNotes(pad);
        leds.displayActiveNoteLEDs(true, pad);

    }

}

void Buttons::handleOctaveEvent(bool direction, bool state)   {

    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    if (buttons.getButtonState(BUTTON_OCTAVE_DOWN) && buttons.getButtonState(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode
        if (pads.isUserScale(pads.getActiveScale()))    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.isPadPressed(pads.getLastTouchedPad()))   {

                    display.displayEditModeNotAllowed(padNotReleased);
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #ifdef DEBUG
                        printf("Pad edit mode\n");
                    #endif
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                    leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
                    leds.setLEDstate(LED_OCTAVE_UP, ledStateFull);

                }

            }   else {

                leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);
                pads.exitPadEditMode();

            }

        }   else {

                display.displayEditModeNotAllowed(notUserPreset);
                leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                leds.setLEDstate(LED_OCTAVE_UP, ledStateOff);

        }

        //temporarily disable buttons
        buttonEnabled[BUTTON_OCTAVE_UP] = false;
        buttonEnabled[BUTTON_OCTAVE_DOWN] = false;

    } else {

        bool editMode = pads.editModeActive();

        switch(editMode)    {

            case true:
            switch(state)   {

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
            if (pads.isUserScale(pads.getActiveScale()) || (pads.isPredefinedScale(pads.getActiveScale()) && !getButtonState(BUTTON_ON_OFF_NOTES)))   {

                //shift entire octave up or down
                if (!state)    {

                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    uint8_t activeOctave = pads.getActiveOctave();
                    display.displayNoteChange(shiftResult, octaveChange, normalizeOctave(activeOctave));
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);

                }   else {

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateDim);

                }

            }   else if (getButtonState(BUTTON_ON_OFF_NOTES)) {

                    //shift single note, but only in predefined presets
                    if (!state)    {

                        if (pads.isPadPressed(lastTouchedPad))   {

                            display.displayEditModeNotAllowed(padNotReleased);
                            return;

                        }

                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateOff);
                        buttonEnabled[BUTTON_ON_OFF_NOTES] = false;

                        changeOutput_t shiftResult = pads.shiftNote(direction);
                        display.displayNoteChange(shiftResult, noteShift, direction);

                    }   else {

                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledStateFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);

                    }

                }
                break;

            }

        }

}

Buttons buttons;
#endif