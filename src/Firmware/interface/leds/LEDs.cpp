#include "LEDs.h"
#include "../pads/Pads.h"
#include "../../database/Database.h"
#include "../pads/Pads.h"

const uint8_t ledNoteArray[] =
{
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

LEDs::LEDs()
{
    //default constructor
}

void LEDs::init()
{
    setFadeSpeed(DEFAULT_FADE_SPEED);
}

void LEDs::setAllOff()
{
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateOff);
}

void LEDs::setAllOn()
{
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        setLEDstate(i, ledStateFull);
}

void LEDs::setLEDstate(uint8_t ledNumber, ledState_t state)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        switch(state)
        {
            case ledStateOff:
            ledState[ledNumber] = 0;
            break;

            case ledStateDim:
            //clear intensity bit
            bitClear(ledState[ledNumber], LED_INTENSITY_BIT);
            //set active and constant on bit
            bitSet(ledState[ledNumber], LED_ACTIVE_BIT);
            bitSet(ledState[ledNumber], LED_CONSTANT_ON_BIT);
            break;

            case ledStateFull:
            //set full intensity bit
            bitSet(ledState[ledNumber], LED_INTENSITY_BIT);
            //set active and constant on bit
            bitSet(ledState[ledNumber], LED_ACTIVE_BIT);
            bitSet(ledState[ledNumber], LED_CONSTANT_ON_BIT);
            break;

            default:
            return;
        }
    }
}

uint8_t LEDs::getLEDnumberFromTonic(note_t note)
{
    return ledNoteArray[note];
}

ledState_t LEDs::getLEDstate(uint8_t ledNumber)
{
    if (!bitRead(ledState[ledNumber], LED_CONSTANT_ON_BIT))
        return ledStateOff;
    else if (bitRead(ledState[ledNumber], LED_INTENSITY_BIT))
        return ledStateFull;
    else
        return ledStateDim;

    return ledStateOff;
}

void LEDs::setFadeSpeed(uint8_t speed)
{
    pwmSteps = speed;
}

void LEDs::tonicLEDsOff()
{
    for (int i=0; i<MIDI_NOTES; i++)
        setLEDstate(ledNoteArray[i], ledStateOff);

}

void LEDs::setNoteLEDstate(note_t note, ledState_t state)
{
    uint8_t ledNumber = getLEDnumberFromTonic(note);
    setLEDstate(ledNumber, state);
}

ledState_t LEDs::getTonicLEDstate(note_t note)
{
    return getLEDstate(getLEDnumberFromTonic(note));
}

void LEDs::displayActiveNoteLEDs(bool padEditMode, uint8_t pad)
{
    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD],
            padNote,
            noteCounter = 0;

    switch(padEditMode)
    {
        case true:
        //indicate assigned notes in pad edit mode using note leds
        for (uint8_t i=0; i<NOTES_PER_PAD; i++)
        {
            padNote = pads.getPadNote(pad, i);

            if (padNote == BLANK_NOTE)
                continue;

            tonicArray[noteCounter] = pads.getTonicFromNote(padNote);
            octaveArray[noteCounter] = pads.getOctaveFromNote(padNote);
            noteCounter++;
        }

        //turn off all LEDs
        for (int i=0; i<MIDI_NOTES; i++)
            setNoteLEDstate((note_t)i, ledStateOff);

        //set dim led state for assigned notes on current pad
        for (int i=0; i<noteCounter; i++)
            setNoteLEDstate((note_t)tonicArray[i], ledStateDim);

        //set full led state for assigned notes on current pad if note matches current octave
        for (int i=0; i<noteCounter; i++)
        {
            if (octaveArray[i] == pads.getActiveOctave())
                setNoteLEDstate((note_t)tonicArray[i], ledStateFull);
        }
        break;

        case false:
        //first, turn off all tonic LEDs
        tonicLEDsOff();
        for (int i=0; i<MIDI_NOTES; i++)
        {
            //turn tonic LED on only if corresponding note is active
            if (pads.noteActive((note_t)i))
                leds.setNoteLEDstate((note_t)i, ledStateDim);
        }
        break;
    }
}

LEDs leds;
