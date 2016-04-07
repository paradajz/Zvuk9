#include "Pads.h"
#include <avr/cpufunc.h>

Pads::Pads()  {

    //default constructor
    //only init variables with custom init value (other than 0)

    for (int i=0; i<NUMBER_OF_PADS; i++)        {

        lastXMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastYMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastAftertouchValue[i] = DEFAULT_XY_AT_VALUE;
        padPressHistory_buffer[i] = 0;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activeProgram = -1;
    activePreset = -1;
    octaveShiftAmount = 0;
    noteShiftAmount = 0;

}

void Pads::init()   {

    initHardware();
    getConfiguration();

}

Pads pads;