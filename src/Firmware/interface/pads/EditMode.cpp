#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool Pads::setEditModeState(bool state, int8_t pad)
{
    assert(PAD_CHECK(pad));

    switch(state)
    {
        case true:
        #ifdef DEBUG
        printf_P(PSTR("Editing pad %d\n"), pad);
        #endif

        editModeActivated = true;

        display.setupPadEditScreen(pad+1, getActiveOctave(), true);
        leds.displayActiveNoteLEDs(true, pad);
        break;

        case false:
        editModeActivated = false;
        display.setupHomeScreen();
        //after exiting from pad edit mode, restore note led states
        leds.displayActiveNoteLEDs();
        break;
    }

    return true;
}