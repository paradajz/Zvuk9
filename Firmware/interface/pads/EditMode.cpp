#include "Pads.h"

bool editModeActivated;

void Pads::setupPadEditMode(uint8_t pad)    {

    #if MODE_SERIAL > 0
        vserial.print("Editing pad ");
        vserial.println(pad);
    #endif

    #ifdef MODULE_LCD
        display.displayPadEditMode(pad + 1);
    #endif

    #if MODE_SERIAL > 0
        vserial.print("Active octave: ");
        vserial.println(activeOctave);
    #endif

    #ifdef MODULE_LCD
        display.displayActiveOctave(normalizeOctave(activeOctave));
    #endif

    displayActivePadNotes(pad);

    #ifdef MODULE_LEDS
        leds.displayActiveNoteLEDs(true, pad);
    #endif

}

void Pads::displayActivePadNotes(uint8_t pad) {

    //this will display all active notes currently assigned to pad while in pad edit mode

    uint8_t tonicArray[NOTES_PER_PAD],
            noteCounter = 0;

    int8_t octaveArray[NOTES_PER_PAD];

    #if MODE_SERIAL > 0
        vserial.println("Assigned notes:");
    #endif

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[pad][i] == BLANK_NOTE) continue;

        tonicArray[noteCounter] = getTonicFromNote(padNote[pad][i]);
        octaveArray[noteCounter] = normalizeOctave(getOctaveFromNote(padNote[pad][i]));
        noteCounter++;

        #if MODE_SERIAL > 0
            vserial.println(padNote[pad][i]);
        #endif

    }

    #ifdef MODULE_LCD
        display.displayActivePadNotes(tonicArray, octaveArray, noteCounter, true);
    #endif

}

void Pads::exitPadEditMode()    {

    editModeActivated = false;
    #ifdef MODULE_LCD
    display.clearPadEditMode();
    display.displayProgramAndPreset(activeProgram+1, activePreset);
    #endif
    //after exiting from pad edit mode, restore note led states
    #ifdef MODULE_LEDS
    leds.displayActiveNoteLEDs();
    #endif

}

bool Pads::editModeActive() {

    return editModeActivated;

}

void Pads::setEditMode(bool state)    {

    editModeActivated = state;

}