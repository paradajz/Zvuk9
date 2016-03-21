#include "Pads.h"
#include <avr/cpufunc.h>

#define PAD_NOTE_BUFFER_SIZE    32

static uint8_t pad_buffer[PAD_NOTE_BUFFER_SIZE];
static uint8_t velocity_buffer[PAD_NOTE_BUFFER_SIZE];
static uint32_t pad_note_timer_buffer[PAD_NOTE_BUFFER_SIZE];
static uint8_t note_buffer_head = 0;
static uint8_t note_buffer_tail = 0;

static int8_t padPressHistory_buffer[NUMBER_OF_PADS];
static uint8_t padPressHistory_counter = 0;

Pads::Pads()  {

    //default constructor

}

void Pads::init()   {

    initHardware();
    initVariables();
    getConfiguration();

}

void Pads::initVariables()  {

    //only init variables with custom init value (other than 0)

    for (int i=0; i<NUMBER_OF_PADS; i++)        {

        lastXValue[i] = DEFAULT_XY_VALUE;
        lastYValue[i] = DEFAULT_XY_VALUE;
        padPressHistory_buffer[i] = 0;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activeProgram = -1;
    activePreset = -1;
    octaveShiftAmount = 0;
    noteShiftAmount = 0;

}

void Pads::initHardware()   {

    initPadPins();
    setMuxInput(activePad);
    initADC(ADC_PRESCALER_128, true, ADC_V_REF_AVCC); //prescaler 128, enable interrupts, AVCC voltage reference

}

bool Pads::isPadPressed(uint8_t padNumber) {

    return bitRead(padPressed, padNumber);

}

void Pads::setPadPressed(uint8_t padNumber, bool padState) {

    bitWrite(padPressed, padNumber, padState);

}

void Pads::setFunctionLEDs(uint8_t padNumber)   {

    if (splitCounter == splitXYFunctions)  {

        //split features
        //turn off function LEDs first
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_X, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_Y, ledIntensityOff);

        //turn on feature LEDs depending on enabled features for last touched pad
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, getNoteSendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_X, getCCXsendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_Y, getCCYsendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);

    }

}

void Pads::updateLastTouchedPad()   {

    if (padID[activePad] != getPadPressHistoryIndex(lastActiveID))    {

        #if MODE_SERIAL > 0
            Serial.print(F("Last touched pad: ")); Serial.println(getPadPressHistoryIndex(lastActiveID));
            Serial.print(F("Current pad: ")); Serial.println(padID[activePad]);
        #endif

        setFunctionLEDs(padID[activePad]);
        updatePressHistory(padID[activePad]);

        if (editModeActive())
            setupPadEditMode(padID[activePad]);

    }

}

void Pads::sendXY(uint8_t pad)  {

    bool xAvailable_ = getCCXsendEnabled(pad);
    bool yAvailable_ = getCCYsendEnabled(pad);

    if (xAvailable_) {

        if (xAvailable)   {

            #if MODE_SERIAL
                Serial.print(F("X for pad "));
                Serial.print(pad);
                Serial.print(F(": "));
                #if XY_FLIP_VALUES > 0
                    Serial.println(127-midiX);
                #else
                    Serial.println(midiX);
                #endif
                Serial.print(F("X CC: "));
                Serial.println(ccXPad[pad]);
            #else
                #if XY_FLIP_VALUES > 0
                    midi.sendControlChange(midiChannel, ccXPad[pad], 127-midiX);
                #else
                    midi.sendControlChange(midiChannel, ccXPad[pad], midiX);
                #endif
                lastXMIDIvalue[pad] = midiX;
            #endif

        }

    }

    if (yAvailable_) {

        if (yAvailable)   {

            #if MODE_SERIAL
                Serial.print(F("Y for pad "));
                Serial.print(pad);
                Serial.print(F(": "));
                #if XY_FLIP_VALUES > 0
                    Serial.println(midiY);
                #else
                    Serial.println(127-midiY);
                #endif
                Serial.print(F("Y CC: "));
                Serial.println(ccYPad[pad]);
            #else
                #if XY_FLIP_VALUES > 0
                    midi.sendControlChange(midiChannel, ccYPad[pad], midiY);
                #else
                    midi.sendControlChange(midiChannel, ccYPad[pad], 127-midiY);
                #endif
                lastYMIDIvalue[pad] = midiY;
            #endif

        }

    }

    #if XY_FLIP_VALUES > 0
        handleXY(pad, 127-midiX, midiY, xAvailable_, yAvailable_);
    #else
        handleXY(pad, midiX, 127-midiY, xAvailable_, yAvailable_);
    #endif

    //record first sent x/y values
    //if they change enough, reset aftertouch gesture counter
    if ((initialXvalue[pad] == -999) || (initialYvalue[pad] == -999))   {

        initialXvalue[pad] = midiX;
        initialYvalue[pad] = midiY;

    }   else {

        if (
            (abs(initialXvalue[pad] - midiX) > XY_CHANGE_AFTERTOUCH_RESET) ||
            (abs(initialYvalue[pad] - midiY) > XY_CHANGE_AFTERTOUCH_RESET)
         )  if (!afterTouchActivated[pad]) resetAfterTouchCounters(pad);

    }

    xyAvailable = false;

}

void Pads::storeNotes(uint8_t pad)  {

    //store midi note on in circular buffer
    uint8_t i = note_buffer_head + 1;
    if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;
    pad_buffer[i] = pad;
    velocity_buffer[i] = midiVelocity;
    pad_note_timer_buffer[i] = newMillis();
    note_buffer_head = i;

    lastVelocityValue[pad] = midiVelocity;

}

void Pads::updatePressHistory(uint8_t pad) {

    //store currently pressed pad in circular buffer
    if (padPressHistory_counter >= NUMBER_OF_PADS) padPressHistory_counter = 0; //overwrite
    padPressHistory_buffer[padPressHistory_counter] = pad;

    #if MODE_SERIAL > 0
        Serial.println(F("Printing pad press history: "));
        for (int i=0; i<NUMBER_OF_PADS; i++)
            Serial.println(padPressHistory_buffer[i]);
        Serial.print(F("Latest index: "));
        Serial.println(padPressHistory_counter);
    #endif

    padPressHistory_counter++;

}

note_t Pads::getTonicFromNote(uint8_t note)    {

    if (note == BLANK_NOTE) return MIDI_NOTES;
    return (note_t)(note % MIDI_NOTES);

}

uint8_t Pads::getOctaveFromNote(uint8_t note)  {

    if (note == BLANK_NOTE) return MIDI_OCTAVE_RANGE;
    return note / MIDI_NOTES;

}

uint8_t Pads::getPadPressHistoryIndex(padHistoryID_t id)   {

    int8_t index = padPressHistory_counter;

    switch(id)  {

        case previousID:
        index -= 1;
        if (index < 0) index = NUMBER_OF_PADS-1;
        return padPressHistory_buffer[index];

        default:
        return padPressHistory_buffer[index];

    }

}

void Pads::checkMIDIdata()   {

    uint8_t pad = padID[activePad];

    //send X/Y immediately
    if (xyAvailable)
        sendXY(pad);

    //send aftertouch immediately
    if (afterTouchAvailable)
        sendAftertouch(pad);

    if (velocityAvailable)  {

        switch(midiNoteOn)  {

            case true:
            //if note on event happened, store notes in buffer first
            storeNotes(pad);
            break;

            case false:
            //note off event
            //send note off
            sendNotes(pad, 0, false);
            break;

        }   velocityAvailable = false;

    }

    //notes are stored in buffer and they're sent after PAD_NOTE_SEND_DELAY
    //to avoid glide effect while sending x/y + notes
    while (note_buffer_head != note_buffer_tail)   {

        uint8_t i = note_buffer_tail + 1;
        if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;

        //check buffer until it's empty
        uint32_t noteTime = pad_note_timer_buffer[i];
        //this is fifo (circular) buffer
        //check first element in buffer
        //if first element (note) can't pass this condition, none of the other elements can, so return
        if ((newMillis() - noteTime) < PAD_NOTE_SEND_DELAY) return;
        note_buffer_tail = i;

        sendNotes(pad_buffer[i], velocity_buffer[i], true);

    }

    //pad has been released
    //restore lcd/led states from previous pad if it's pressed

    uint8_t previousPad = getPadPressHistoryIndex(previousID);

    if (isPadPressed(previousPad))  {

        //restore data from last touched pad (display+midi cc x/cc y)
        bool ccXsendEnabled = getCCXsendEnabled(previousPad);
        bool ccYsendEnabled = getCCYsendEnabled(previousPad);
        uint8_t ccXvaluePreviousPad = getCCvalue(ccTypeX, previousPad);
        uint8_t ccYvaluePreviousPad = getCCvalue(ccTypeY, previousPad);
        uint8_t ccXvalueActivePad = getCCvalue(ccTypeX, pad);
        uint8_t ccYvalueActivePad = getCCvalue(ccTypeY, pad);

        if ((ccXvalueActivePad == ccXvaluePreviousPad) && ccXsendEnabled)
            midi.sendControlChange(midiChannel, ccXvaluePreviousPad, lastXMIDIvalue[previousPad]);
        else if ((ccYvalueActivePad == ccYvaluePreviousPad) && ccYsendEnabled)
            midi.sendControlChange(midiChannel, ccYvaluePreviousPad, lastYMIDIvalue[previousPad]);

        if (getNoteSendEnabled(previousPad))
            handleNote(previousPad, lastVelocityValue[previousPad], true);

        handleXY(previousPad, lastXValue[previousPad], lastYValue[previousPad], ccXsendEnabled, ccYsendEnabled);
        setFunctionLEDs(previousPad);

    }

}

void Pads::sendNotes(uint8_t pad, uint8_t velocity, bool state)   {

    switch(state)   {

        case true:
        //note on
        if (!noteSendEnabled[pad]) return; // no need to check
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
        //some special considerations here
        bool sendOff = true;
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

            if (sendOff)    {

                if (noteSendEnabled[pad])
                    midi.sendNoteOff(midiChannel, padNote[pad][i], 0);

            }

        }

        if (afterTouchActivated[pad])   {

            midi.sendAfterTouch(midiChannel, 0);
            afterTouchActivated[pad] = false;
            afterTouchAvailable =false;

        }

        #if MODE_SERIAL > 0
            Serial.print(F("Pad "));
            Serial.print(pad);
            Serial.println(F(" released"));
        #endif

        //check if pad data on lcd needs to be cleared
        if (!checkPadsPressed()) display.clearPadData();

        break;

    }

    //handle lcd and leds
    handleNote(pad, velocity, state);

}

void Pads::handleNote(uint8_t pad, uint8_t velocity, bool state)  {

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
        uint8_t tonicArray[NOTES_PER_PAD],
                octaveArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++) {

            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            leds.setNoteLEDstate((note_t)tonicArray[i], ledIntensityFull);
            octaveArray[i] = normalizeOctave(getOctaveFromNote(noteArray[i]));

        }

        display.displayActivePadNotes(tonicArray, octaveArray, noteCounter);
        display.displayVelocity(velocity);
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

void Pads::handleXY(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xAvailable, bool yAvailable)   {

    if (xAvailable || yAvailable)
        display.displayXYposition(xPosition, yPosition, xAvailable, yAvailable);

    //always display ccx/ccy
    display.displayXYcc(ccXPad[pad], ccYPad[pad]);

}

bool Pads::checkPadsPressed()   {

    //return true if any pad is pressed
    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i)) return true;

    return false;

}

bool Pads::noteActive(note_t note) {

    //return true if received tonic is among active notes on some pad

    for (int i=0; i<NUMBER_OF_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            if (padNote[i][j] != BLANK_NOTE)
                if (getTonicFromNote(padNote[i][j]) == note) return true;

    return false;

}

Pads pads;