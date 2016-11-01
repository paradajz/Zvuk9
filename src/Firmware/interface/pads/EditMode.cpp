#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool editModeActivated = false;

void Pads::setupPadEditMode(uint8_t pad)
{
    #ifdef DEBUG
    printf("Editing pad %d\n", pad);
    #endif

    display.displayPadEditMode(pad + 1);
    display.displayActiveOctave(normalizeOctave(activeOctave));
    displayActivePadNotes(pad);
    leds.displayActiveNoteLEDs(true, pad);
}

void Pads::displayActivePadNotes(uint8_t pad)
{
    //this will display all active notes currently assigned to pad while in pad edit mode
    uint8_t tonicArray[NOTES_PER_PAD],
            noteCounter = 0;

    int8_t octaveArray[NOTES_PER_PAD];

    //#ifdef DEBUG
    //printf("Assigned notes:\n");
    //#endif

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] == BLANK_NOTE)
            continue;

        tonicArray[noteCounter] = getTonicFromNote(padNote[pad][i]);
        octaveArray[noteCounter] = normalizeOctave(getOctaveFromNote(padNote[pad][i]));
        noteCounter++;

        //#ifdef DEBUG
        //printf("%d\n", padNote[pad][i]);
        //#endif
    }

    display.displayActivePadNotes(tonicArray, octaveArray, noteCounter, true);
}

void Pads::exitPadEditMode()
{
    editModeActivated = false;
    display.clearPadEditMode();
    display.displayProgramAndScale(activeProgram+1, activeScale);
    //after exiting from pad edit mode, restore note led states
    leds.displayActiveNoteLEDs();
}

bool Pads::editModeActive()
{
    return editModeActivated;
}

void Pads::setEditMode(bool state)
{
    editModeActivated = state;
}
