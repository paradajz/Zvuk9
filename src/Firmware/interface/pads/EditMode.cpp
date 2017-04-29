#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool editModeActivated = false;

void Pads::setEditModeState(bool state, uint8_t pad)
{
    switch(state)
    {
        case true:
        #ifdef DEBUG
        printf_P(PSTR("Editing pad %d\n"), pad);
        #endif

        editModeActivated = true;

        display.displayPadEditMode(pad + 1);
        display.displayActiveOctave(normalizeOctave(activeOctave));
        displayActivePadNotes(pad);
        leds.displayActiveNoteLEDs(true, pad);
        break;

        case false:
        editModeActivated = false;
        display.clearPadEditMode();
        display.displayProgramAndScale(activeProgram+1, activeScale);
        //after exiting from pad edit mode, restore note led states
        leds.displayActiveNoteLEDs();
        break;
    }
}

void Pads::displayActivePadNotes(uint8_t pad)
{
    //this will display all active notes currently assigned to pad while in pad edit mode
    uint8_t tonicArray[NOTES_PER_PAD],
            noteCounter = 0;

    int8_t octaveArray[NOTES_PER_PAD];

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] == BLANK_NOTE)
            continue;

        tonicArray[noteCounter] = getTonicFromNote(padNote[pad][i]);
        octaveArray[noteCounter] = normalizeOctave(getOctaveFromNote(padNote[pad][i]));
        noteCounter++;
    }

    display.displayActivePadNotes(tonicArray, octaveArray, noteCounter, true);
}

bool Pads::getEditModeState()
{
    return editModeActivated;
}