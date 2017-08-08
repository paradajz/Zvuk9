#pragma once

#include "DataTypes.h"

///
/// \ingroup interfacePads
/// @{

///
/// \brief Various predefined scales defined as arrays of MIDI notes.
/// @{

const note_t scaleNaturalMinor_notes[]       = { C, D, D_SHARP, F, G, G_SHARP, A_SHARP };
const note_t scaleMelodicMinor_notes[]       = { C, D, D_SHARP, F, G, A, B };
const note_t scaleHarmonicMinor_notes[]      = { C, D, D_SHARP, F, G, G_SHARP, B };
const note_t scaleMajor_notes[]              = { C, D, E, F, G, A, B };
const note_t scaleHarmonicMajor_notes[]      = { C, D, E, F, G, G_SHARP, B };
const note_t scaleMinorPentatonic_notes[]    = { C, D_SHARP, F, G, A_SHARP };
const note_t scaleMajorPentatonic_notes[]    = { C, D, E, G, A };

/// @}

///
/// \brief Predefined arrays stored into array for easier access.
///
const note_t *scale_notes[PREDEFINED_SCALES] =
{
    scaleNaturalMinor_notes,
    scaleMelodicMinor_notes,
    scaleHarmonicMinor_notes,
    scaleMajor_notes,
    scaleHarmonicMajor_notes,
    scaleMinorPentatonic_notes,
    scaleMajorPentatonic_notes
};

///
/// \brief Sizes of each predefined scale stored into array for easier manipulation.
///
const uint8_t scale_notes_sizes[PREDEFINED_SCALES] =
{
    ARRAY_SIZE(scaleNaturalMinor_notes),
    ARRAY_SIZE(scaleMelodicMinor_notes),
    ARRAY_SIZE(scaleHarmonicMinor_notes),
    ARRAY_SIZE(scaleMajor_notes),
    ARRAY_SIZE(scaleHarmonicMajor_notes),
    ARRAY_SIZE(scaleMinorPentatonic_notes),
    ARRAY_SIZE(scaleMajorPentatonic_notes)
};

/// @}