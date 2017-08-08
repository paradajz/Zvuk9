#include "Pads.h"

///
/// \ingroup interfacePads
/// @{

///
/// \brief Default constructor.
/// Initializes variables to their default states.
///
Pads::Pads()
{
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        lastXMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastYMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastAftertouchValue[i] = DEFAULT_XY_AT_VALUE;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;
    }

    activeProgram = -1;
    activeScale = -1;
    activePadEditOctave = DEFAULT_OCTAVE;
}

///
/// \brief Initialize all parameters and settings.
///
void Pads::init()
{
    getConfiguration();
    curves.init();
}

Pads pads;

/// @}