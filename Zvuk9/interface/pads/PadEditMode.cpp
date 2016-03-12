#include "Pads.h"

void Pads::setupPadEditMode(uint8_t pad)    {

    #if MODE_SERIAL > 0
        Serial.print(F("Editing pad "));
        Serial.println(pad);
    #endif

    lcDisplay.displayPadEditMode(pad + 1);

    #if MODE_SERIAL > 0
        Serial.print(F("Active octave: "));
        Serial.println(localOctaveValue);
    #endif

    lcDisplay.displayActiveOctave(normalizeOctave(localOctaveValue));
    displayActivePadNotes(pad);
    leds.displayActiveNoteLEDs(true, pad);

}

void Pads::displayActivePadNotes(uint8_t pad) {

    //this will display all active notes currently assigned to pad while in pad edit mode

    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD];

    #if MODE_SERIAL > 0
        Serial.println(F("Assigned notes:"));
    #endif

    for (int i=0; i<NOTES_PER_PAD; i++) {

        tonicArray[i] = getTonicFromNote(padNote[pad][i]);
        octaveArray[i] = getOctaveFromNote(padNote[pad][i]);

        #if MODE_SERIAL > 0
            Serial.println(padNote[pad][i]);
        #endif

    }

    lcDisplay.displayActivePadNotes(tonicArray, octaveArray);

}

void Pads::exitPadEditMode()    {

    lcDisplay.clearPadEditMode();
    editModeActivated = false;
    //after exiting from pad edit mode, restore note led states
    leds.displayActiveNoteLEDs();

}