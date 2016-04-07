#include "Pads.h"

void Pads::sendX(uint8_t pad)  {

    #if MODE_SERIAL
        Serial.print(F("X for pad "));
        Serial.print(pad);
        Serial.print(F(": "));
        #if XY_FLIP_VALUES > 0
            Serial.println(127-lastXMIDIvalue[pad]);
        #else
            Serial.println(lastXMIDIvalue[pad]);
        #endif
        Serial.print(F("X CC: "));
        Serial.println(ccXPad[pad]);
    #else
        #if XY_FLIP_VALUES > 0
            midi.sendControlChange(midiChannel, ccXPad[pad], 127-lastXMIDIvalue[pad]);
        #else
            midi.sendControlChange(midiChannel, ccXPad[pad], lastXMIDIvalue[pad]);
        #endif
    #endif

}

void Pads::sendY(uint8_t pad)  {

    #if MODE_SERIAL
        Serial.print(F("Y for pad "));
        Serial.print(pad);
        Serial.print(F(": "));
        #if XY_FLIP_VALUES > 0
            Serial.println(127-lastYMIDIvalue[pad]);
        #else
            Serial.println(lastYMIDIvalue[pad]);
        #endif
        Serial.print(F("Y CC: "));
        Serial.println(ccYPad[pad]);
    #else
        #if XY_FLIP_VALUES > 0
            midi.sendControlChange(midiChannel, ccYPad[pad], 127-lastYMIDIvalue[pad]);
        #else
            midi.sendControlChange(midiChannel, ccYPad[pad], lastYMIDIvalue[pad]);
        #endif
    #endif

}

void Pads::sendNotes(uint8_t pad, uint8_t velocity, bool state)   {

    bool sendOff = true;

    switch(state)   {

        case true:
        //note on
        #if MODE_SERIAL > 0
            Serial.print(F("Pad "));
            Serial.print(pad);
            Serial.println(F(" pressed. Notes: "));
        #endif

        for (int i=0; i<NOTES_PER_PAD; i++) {

            if (padNote[pad][i] == BLANK_NOTE) continue;

            #if MODE_SERIAL
                Serial.println(padNote[pad][i]);
            #else
                midi.sendNoteOn(midiChannel, padNote[pad][i], velocity);
            #endif

        }

        #if MODE_SERIAL
            Serial.print(F("Velocity: "));
            Serial.println(velocity);
        #endif
        break;

        case false:
        //note off
        #if MODE_SERIAL > 0
            Serial.print(F("Pad "));
            Serial.print(pad);
            Serial.println(F(" released"));
        #else
            //some special considerations here
            for (int i=0; i<NOTES_PER_PAD; i++)    {

                if (padNote[pad][i] == BLANK_NOTE) continue;

                for (int j=0; j<NUMBER_OF_PADS; j++) {

                    //don't check current pad
                    if (j == pad) continue;

                    //don't check released pads
                    if (!isPadPressed(j)) continue;

                    //only send note off if the same note isn't active on some other pad already
                    if (padNote[j][i] == padNote[pad][i])    {

                        sendOff = false;
                        break; //no need to check further

                    }

                }

                if (sendOff) midi.sendNoteOff(midiChannel, padNote[pad][i], 0);

            }
        #endif

        break;

    }

    handleNoteLEDs(pad, state);

}

void Pads::sendAftertouch(uint8_t pad)  {

    uint8_t aftertouchValue = lastMIDInoteState[pad] ? lastAftertouchValue[pad] : 0;

    switch(aftertouchType)  {

        case aftertouchPoly:
        #if MODE_SERIAL > 0
            Serial.print(F("Sending key aftertouch, pad "));
            Serial.print(pad);
            Serial.print(F(": "));
            Serial.println(lastAftertouchValue[pad]);
        #else
            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[pad][i] != BLANK_NOTE)
                    midi.sendKeyAftertouch(midiChannel, padNote[pad][i], aftertouchValue);

            }
        #endif
        break;

        case aftertouchChannel:
        #if MODE_SERIAL > 0
            Serial.print(F("Sending channel aftertouch: "));
            Serial.println(maxAftertouchValue);
        #else
            midi.sendChannelAftertouch(midiChannel, maxAftertouchValue);
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
            leds.setNoteLEDstate((note_t)tonicArray[i], ledIntensityFull);

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

            for (int i=0; i<NUMBER_OF_PADS; i++)    {

                if (!isPadPressed(i)) continue; //skip released pad
                if (i == pad) continue; //skip current pad

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (getTonicFromNote(padNote[i][j]) == getTonicFromNote(noteArray[z])) {

                        noteActive = true;

                    }

                }

            }   if (!noteActive) leds.setNoteLEDstate(getTonicFromNote((note_t)noteArray[z]), ledIntensityDim);

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

            #if MODE_SERIAL > 0
                Serial.print(F("Clearing notes for pad "));
                Serial.println(pad);
            #endif

            display.displayActivePadNotes(0, 0, 0);
            return;

        }
        //note on
        uint8_t tonicArray[NOTES_PER_PAD],
                octaveArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++) {

            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            octaveArray[i] = normalizeOctave(getOctaveFromNote(noteArray[i]));

        }

        display.displayActivePadNotes(tonicArray, octaveArray, noteCounter);
        display.displayVelocity(velocity);
        break;

        case false:
        //note off
        display.displayActivePadNotes(0, 0, 0);
        break;

    }

}