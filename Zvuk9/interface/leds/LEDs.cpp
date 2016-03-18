#include "LEDs.h"
#include "../../hardware/timer/TimerObject.h"
#include "../pads/Pads.h"

uint8_t intensity2value(ledIntensity_t intensity)  {

    switch(intensity)   {

        case ledIntensityOff:
        return 0;

        case ledIntensityDim:
        return LED_INTENSITY_HALF;

        case ledIntensityFull:
        return LED_INTENSITY_FULL;

        default:
        return 0;

    }

}

const uint8_t ledNoteArray[] = {

    LED_NOTE_C,
    LED_NOTE_C_SHARP,
    LED_NOTE_D,
    LED_NOTE_D_SHARP,
    LED_NOTE_E,
    LED_NOTE_F,
    LED_NOTE_F_SHARP,
    LED_NOTE_G,
    LED_NOTE_G_SHARP,
    LED_NOTE_A,
    LED_NOTE_A_SHARP,
    LED_NOTE_B

};

LEDs::LEDs()    {

    //default constructor
    setOutputMacro(LED_ROW_1_DDR, LED_ROW_1_PIN_INDEX);
    setOutputMacro(LED_ROW_2_DDR, LED_ROW_2_PIN_INDEX);
    setOutputMacro(LED_ROW_3_DDR, LED_ROW_3_PIN_INDEX);

    setLowMacro(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setLowMacro(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setLowMacro(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setOutputMacro(DECODER_OUT_1_DDR, DECODER_OUT_1_PIN_INDEX);
    setOutputMacro(DECODER_OUT_2_DDR, DECODER_OUT_2_PIN_INDEX);
    setOutputMacro(DECODER_OUT_3_DDR, DECODER_OUT_3_PIN_INDEX);

}

void LEDs::init()   {

    //

}

void LEDs::allLEDsOff() {

    for (int i=0; i<NUMBER_OF_LEDS; i++)
        timers.setLEDstate(i, LED_INTENSITY_OFF);

}

void LEDs::allLEDsOn() {

    for (int i=0; i<NUMBER_OF_LEDS; i++)
        timers.setLEDstate(i, LED_INTENSITY_FULL);

}

void LEDs::setLEDstate(uint8_t ledNumber, ledIntensity_t state)    {

    timers.setLEDstate(ledNumber, intensity2value(state));

}

uint8_t LEDs::getLEDnumberFromTonic(note_t note)  {

    return ledNoteArray[note];

}

ledIntensity_t LEDs::getLEDstate(uint8_t ledNumber)    {

    return timers.getLEDstate(ledNumber);

}

void LEDs::setFadeSpeed(uint8_t speed)  {

    timers.setPWMsteps(speed);

}

void LEDs::tonicLEDsOff()   {

    for (int i=0; i<MIDI_NOTES; i++)
        timers.setLEDstate(ledNoteArray[i], LED_INTENSITY_OFF);

}

void LEDs::setNoteLEDstate(note_t note, ledIntensity_t state)   {

    uint8_t ledNumber = getLEDnumberFromTonic(note);
    timers.setLEDstate(ledNumber, intensity2value(state));

}

ledIntensity_t LEDs::getTonicLEDstate(note_t note)   {

    return timers.getLEDstate(getLEDnumberFromTonic(note));

}

void LEDs::displayActiveNoteLEDs(bool padEditMode, uint8_t pad) {

    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD],
            padNote,
            noteCounter = 0;

    switch(padEditMode) {

        case true:
        //indicate assigned notes in pad edit mode using note leds
        for (int i=0; i<NOTES_PER_PAD; i++) {

            padNote = pads.getPadNote(pad, i);
            if (padNote == BLANK_NOTE) continue;

            tonicArray[noteCounter] = pads.getTonicFromNote(padNote);
            octaveArray[noteCounter] = pads.getOctaveFromNote(padNote);
            noteCounter++;

        }

        //turn off all LEDs
        for (int i=0; i<MIDI_NOTES; i++)
            setNoteLEDstate((note_t)i, ledIntensityOff);

        //set dim led state for assigned notes on current pad
        for (int i=0; i<noteCounter; i++)
            setNoteLEDstate((note_t)tonicArray[i], ledIntensityDim);

        //set full led state for assigned notes on current pad if note matches current octave
        for (int i=0; i<noteCounter; i++) {

            if (octaveArray[i] == pads.getActiveOctave())
                setNoteLEDstate((note_t)tonicArray[i], ledIntensityFull);

        }
        break;

        case false:
        //first, turn off all tonic LEDs
        tonicLEDsOff();
        for (int i=0; i<MIDI_NOTES; i++)  {

            //turn tonic LED on only if corresponding note is active
            if (pads.noteActive((note_t)i))
                leds.setNoteLEDstate((note_t)i, ledIntensityDim);

        }
        break;

    }

}

LEDs leds;
