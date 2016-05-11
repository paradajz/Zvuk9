#include "Pads.h"

void Pads::sendX(uint8_t pad)  {

    #if MODE_SERIAL > 0
        printf("X for pad %d: ", pad);
        #if XY_FLIP_VALUES > 0
            printf("%d\n", 127-lastXMIDIvalue[pad]);
        #else
            printf("%d\n", lastXMIDIvalue[pad]);
        #endif
        printf("X CC: %d\n", ccXPad[pad]);
    #else
        #if XY_FLIP_VALUES > 0
            midi.sendControlChange(midiChannel[pad], ccXPad[pad], 127-lastXMIDIvalue[pad]);
        #else
            midi.sendControlChange(midiChannel[pad], ccXPad[pad], lastXMIDIvalue[pad]);
        #endif
    #endif

}

void Pads::sendY(uint8_t pad)  {

    #if MODE_SERIAL > 0
        printf("Y for pad %d: ", pad);
        #if XY_FLIP_VALUES > 0
            printf("%d\n", 127-lastYMIDIvalue[pad]);
        #else
            printf("%d\n", lastYMIDIvalue[pad]);
        #endif
        printf("Y CC: %d\n", ccYPad[pad]);
    #else
        #if XY_FLIP_VALUES > 0
            midi.sendControlChange(midiChannel[pad], ccYPad[pad], 127-lastYMIDIvalue[pad]);
        #else
            midi.sendControlChange(midiChannel[pad], ccYPad[pad], lastYMIDIvalue[pad]);
        #endif
    #endif

}

void Pads::sendNotes(uint8_t pad, uint8_t velocity, bool state)   {

    bool sendOff;

    switch(state)   {

        case true:
        //note on
        #if MODE_SERIAL > 0
            printf("Pad %d pressed. Notes:\n", pad);
        #endif

        for (int i=0; i<NOTES_PER_PAD; i++) {

            if (padNote[pad][i] == BLANK_NOTE) continue;

            #if MODE_SERIAL > 0
                printf("%d\n", padNote[pad][i]);
            #else
                midi.sendNoteOn(midiChannel[pad], padNote[pad][i], velocity);
            #endif

        }

        #if MODE_SERIAL > 0
            printf("Velocity: %d\n", velocity);
        #endif
        break;

        case false:
        //note off
        #if MODE_SERIAL > 0
            printf("Pad %d released. Notes: \n", pad);
        #endif
            //some special considerations here
            for (int i=0; i<NOTES_PER_PAD; i++)    {

                sendOff = true;

                if (padNote[pad][i] == BLANK_NOTE) continue;

                for (int j=0; j<MAX_PADS; j++) {

                    //don't check current pad
                    if (j == pad) continue;

                    //don't check released pads
                    if (!isPadPressed(j)) continue;

                    //don't check pad if noteSend is disabled
                    if (!noteSendEnabled[j]) continue;

                    //only send note off if the same note isn't active on some other pad already
                    if (padNote[j][i] == padNote[pad][i])    {

                        sendOff = false;
                        break; //no need to check further

                    }

                }

                if (sendOff)    {

                    #if MODE_SERIAL > 0
                        printf("%d\n", padNote[pad][i]);
                    #else
                        midi.sendNoteOff(midiChannel[pad], padNote[pad][i], 0);
                    #endif

                }

            }

        break;

    }

    handleNoteLEDs(pad, state);

}

void Pads::sendAftertouch(uint8_t pad)  {

    uint8_t aftertouchValue = lastMIDInoteState[pad] ? lastAftertouchValue[pad] : 0;

    switch(aftertouchType)  {

        case aftertouchPoly:
        #if MODE_SERIAL > 0
            printf("Sending key aftertouch, pad %d: %d\n", pad, lastAftertouchValue[pad]);
        #else
            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[pad][i] != BLANK_NOTE)
                    midi.sendKeyAftertouch(midiChannel[pad], padNote[pad][i], aftertouchValue);

        }
        if (!lastMIDInoteState[pad]) aftertouchActivated[pad] = false;
        #endif
        break;

        case aftertouchChannel:
        #if MODE_SERIAL > 0
            printf("Sending channel aftertouch: %d\n", maxAftertouchValue);
        #else
            midi.sendChannelAftertouch(midiChannel[pad], maxAftertouchValue);
        #endif
        break;

    }

}

void Pads::handleNoteLEDs(uint8_t pad, bool state)  {

    uint8_t noteArray[NOTES_PER_PAD],
            noteCounter = 0;

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[pad][i] != BLANK_NOTE)  {

            noteArray[noteCounter] = padNote[pad][i];
            noteCounter++;

        }

    }

    switch(state)   {

        case true:
        //note on
        uint8_t tonicArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++) {

            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            #ifdef MODULE_LEDS
                leds.setNoteLEDstate((note_t)tonicArray[i], ledIntensityFull);
            #endif

        }
        break;

        case false:
        //note off
        //we need to set LEDs back to dim states for released pad, but only if
        //some other pad with same active note isn't already pressed

        bool noteActive;

        for (int z=0; z<noteCounter; z++) {

            //iterate over every note on released pad

            noteActive = false;

            for (int i=0; i<MAX_PADS; i++)    {

                if (!isPadPressed(i)) continue; //skip released pad
                if (i == pad) continue; //skip current pad

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (getTonicFromNote(padNote[i][j]) == getTonicFromNote(noteArray[z])) {

                        noteActive = true;

                    }

                }

            }   if (!noteActive)    {

                    #ifdef MODULE_LEDS
                        leds.setNoteLEDstate(getTonicFromNote((note_t)noteArray[z]), ledIntensityDim);
                    #endif

            }

        }
        break;

    }

}

void Pads::handleNoteLCD(uint8_t pad, uint8_t velocity, bool state)    {

    uint8_t noteArray[NOTES_PER_PAD],
            noteCounter = 0;

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[pad][i] != BLANK_NOTE)  {

            noteArray[noteCounter] = padNote[pad][i];
            noteCounter++;

        }

    }

    switch(state)   {

        case true:
        if (!noteCounter || !noteSendEnabled[pad])  {

            #ifdef MODULE_LCD
                display.displayActivePadNotes(0, 0, 0, editModeActive());
            #endif
            return;

        }
        //note on
        uint8_t tonicArray[NOTES_PER_PAD];
        int8_t octaveArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++) {

            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            octaveArray[i] = normalizeOctave(getOctaveFromNote(noteArray[i]));

        }

        #ifdef MODULE_LCD
            display.displayActivePadNotes(tonicArray, octaveArray, noteCounter, editModeActive());
            display.displayVelocity(velocity);
        #endif
        break;

        case false:
            //note off
            #ifdef MODULE_LCD
            display.displayActivePadNotes(0, 0, 0, editModeActive());
            #endif
        break;

    }

}