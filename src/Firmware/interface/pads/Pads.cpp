#include "Pads.h"
#include <avr/cpufunc.h>
#include <util/delay.h>

Pads::Pads()  {

    //default constructor

    for (int i=0; i<MAX_PADS; i++)        {

        lastXMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastYMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastAftertouchValue[i] = DEFAULT_XY_AT_VALUE;
        padPressHistory_buffer[i] = 0;
        octaveShiftAmount[i] = 0;
        noteShiftAmount[i] = 0;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activeProgram = -1;
    activeScale = -1;
    activeOctave = 0;
    octaveShiftPadBuffer = 0;
    scaleShiftPadBuffer = 0;
    calibrationEnabled = false;

    for (int i=0; i<PAD_NOTE_BUFFER_SIZE; i++)  {

        pad_note_timer_buffer[i]    = 0;
        velocity_buffer[i]          = 0;
        pad_buffer[i]               = 0;

    }

    note_buffer_head = 0;
    note_buffer_tail = 0;

}

void Pads::init()   {

    initHardware();
    getConfiguration();

}

Pads pads;