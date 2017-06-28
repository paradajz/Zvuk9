#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool Pads::setEditModeState(bool state, int8_t pad)
{
    if ((pad < 0) || (pad >= NUMBER_OF_PADS))
        return false;

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

    return true;
}