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
        lastXCCvalue[i] = DEFAULT_XY_AT_VALUE;
        lastYCCvalue[i] = DEFAULT_XY_AT_VALUE;
        lastXPitchBendValue[i] = DEFAULT_PITCH_BEND_VALUE;
        lastYPitchBendValue[i] = DEFAULT_PITCH_BEND_VALUE;
        initialXposition[i] = DEFAULT_INITIAL_XY_VALUE;
        initialYposition[i] = DEFAULT_INITIAL_XY_VALUE;

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