#include "Pads.h"

void Pads::setupPadEditMode(uint8_t pad)    {

    lcDisplay.displayPadEditMode(pad + 1);
    lcDisplay.displayActiveOctave(localOctaveValue);
    displayActivePadNotes(pad);
    leds.displayActiveNoteLEDs(true, pad);

}

void Pads::displayActivePadNotes(uint8_t pad) {

    //this will display all active notes currently assigned to pad while in pad edit mode

    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD];

    for (int i=0; i<NOTES_PER_PAD; i++) {

        tonicArray[i] = getTonicFromNote(padNote[pad][i]);
        octaveArray[i] = getOctaveFromNote(padNote[pad][i]);

    }

    lcDisplay.displayActivePadNotes(tonicArray, octaveArray);

}

void Pads::exitPadEditMode()    {

    lcDisplay.clearPadEditMode();
    editModeActivated = false;
    //after exiting from pad edit mode, restore note led states
    leds.displayActiveNoteLEDs();

}