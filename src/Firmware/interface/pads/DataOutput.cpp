#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"
#ifdef NDEBUG
#include "../../midi/src/MIDI.h"
#endif

void Pads::sendX(uint8_t pad)
{
    #ifdef DEBUG
    printf_P(PSTR("X for pad %d: "), pad);
    printf_P(PSTR("%d\n"), lastXMIDIvalue[pad]);
    printf_P(PSTR("X CC: %d\n"), ccXPad[pad]);
    #else
    midi.sendControlChange(ccXPad[pad], lastXMIDIvalue[pad], midiChannel[pad]);
    #endif
}

void Pads::sendY(uint8_t pad)
{
    #ifdef DEBUG
    printf_P(PSTR("Y for pad %d: "), pad);
    printf_P(PSTR("%d\n"), lastYMIDIvalue[pad]);
    printf_P(PSTR("Y CC: %d\n"), ccYPad[pad]);
    #else
    midi.sendControlChange(ccYPad[pad], lastYMIDIvalue[pad], midiChannel[pad]);
    #endif
}

void Pads::sendNotes(uint8_t pad, uint8_t velocity, bool state)
{
    bool sendOff;

    switch(state)
    {
        case true:
        //note on
        #ifdef DEBUG
        printf_P(PSTR("Pad %d pressed. Notes:\n"), pad);
        #endif

        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] == BLANK_NOTE)
                continue;

            #ifdef DEBUG
            printf_P(PSTR("%d\n"), padNote[pad][i]);
            #else
            midi.sendNoteOn(padNote[pad][i], velocity, midiChannel[pad]);
            #endif
        }

        #ifdef DEBUG
        printf_P(PSTR("Velocity: %d\n"), velocity);
        #endif
        break;

        case false:
        //note off
        #ifdef DEBUG
        printf_P(PSTR("Pad %d released. Notes: \n"), pad);
        #endif
        //some special considerations here
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            sendOff = true;

            if (padNote[pad][i] == BLANK_NOTE)
                continue;

            for (int j=0; j<NUMBER_OF_PADS; j++)
            {
                //don't check current pad
                if (j == pad)
                    continue;

                //don't check released pads
                if (!isPadPressed(j))
                    continue;

                //don't check pad if noteSend is disabled
                if (!bitRead(noteSendEnabled, j))
                    continue;

                //only send note off if the same note isn't active on some other pad already
                if (padNote[j][i] == padNote[pad][i])
                {
                    //extra check here - check if midi channels differ
                    if (midiChannel[j] == midiChannel[pad])
                    {
                        sendOff = false;
                        break; //no need to check further
                    }
                }
            }

            if (sendOff)
            {
                #ifdef DEBUG
                printf_P(PSTR("%d\n"), padNote[pad][i]);
                #else
                uint8_t velocity_ = 0;
                midi.sendNoteOff(padNote[pad][i], velocity_, midiChannel[pad]);
                #endif
            }
        }
        break;
    }

    handleNoteLEDs(pad, state);
}

void Pads::sendAftertouch(uint8_t pad)
{
    #ifdef NDEBUG
    uint8_t aftertouchValue = bitRead(lastMIDInoteState, pad) ? lastAftertouchValue[pad] : 0;
    #endif

    switch(aftertouchType)
    {
        case aftertouchPoly:
        #ifdef DEBUG
        printf_P(PSTR("Sending key aftertouch, pad %d: %d\n"), pad, lastAftertouchValue[pad]);
        #else
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] != BLANK_NOTE)
                midi.sendPolyPressure(padNote[pad][i], aftertouchValue, midiChannel[pad]);
        }
        #endif
        break;

        case aftertouchChannel:
        #ifdef DEBUG
        printf_P(PSTR("Sending channel aftertouch: %d\n"), maxAftertouchValue);
        #else
        midi.sendAfterTouch(maxAftertouchValue, midiChannel[pad]);
        #endif
        break;
    }

    if (!bitRead(lastMIDInoteState, pad))
        bitWrite(aftertouchActivated, pad, false);
}

void Pads::handleNoteLEDs(uint8_t pad, bool state)
{
    uint8_t noteArray[NOTES_PER_PAD],
            noteCounter = 0;

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] != BLANK_NOTE)
        {
            noteArray[noteCounter] = padNote[pad][i];
            noteCounter++;
        }
    }

    switch(state)
    {
        case true:
        //note on
        uint8_t tonicArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++)
        {
            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            leds.setNoteLEDstate((note_t)tonicArray[i], ledStateFull);
        }
        break;

        case false:
        //note off
        //we need to set LEDs back to dim states for released pad, but only if
        //some other pad with same active note isn't already pressed
        bool noteActive;

        for (int z=0; z<noteCounter; z++)
        {
            //iterate over every note on released pad
            noteActive = false;

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                if (!isPadPressed(i))
                    continue; //skip released pad
                if (i == pad)
                    continue; //skip current pad

                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (getTonicFromNote(padNote[i][j]) == getTonicFromNote(noteArray[z]))
                        noteActive = true;
                }
            }

            if (!noteActive)
                leds.setNoteLEDstate(getTonicFromNote((note_t)noteArray[z]), ledStateDim);
        }
        break;
    }
}

void Pads::handleNoteLCD()
{
    display.displayActivePadNotes();

    if (isPadPressed(getLastTouchedPad()))
    {
        display.displayVelocity(lastVelocityValue[getLastTouchedPad()]);

        if (isPredefinedScale(activeScale))
        {
            if (noteShiftLevel != 0)
            display.displayNoteShiftLevel(noteShiftLevel);
        }
    }
}
