#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool editModeActivated;

void Pads::setEditModeState(bool state, uint8_t pad)
{
    switch(state)
    {
        case true:
        #ifdef DEBUG
        printf_P(PSTR("Editing pad %d\n"), pad);
        #endif

        editModeActivated = true;

        display.displayPadEditMode();
        display.displayActiveOctave();
        display.displayActivePadNotes();
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

bool Pads::getEditModeState()
{
    return editModeActivated;
}