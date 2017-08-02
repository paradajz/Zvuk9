#pragma once

#include "DataTypes.h"

const note_t scaleNaturalMinor_notes[]       = { C, D, D_SHARP, F, G, G_SHARP, A_SHARP };
const note_t scaleMelodicMinor_notes[]       = { C, D, D_SHARP, F, G, A, B };
const note_t scaleHarmonicMinor_notes[]      = { C, D, D_SHARP, F, G, G_SHARP, B };
const note_t scaleMajor_notes[]              = { C, D, E, F, G, A, B };
const note_t scaleHarmonicMajor_notes[]      = { C, D, E, F, G, G_SHARP, B };
const note_t scaleMinorPentatonic_notes[]    = { C, D_SHARP, F, G, A_SHARP };
const note_t scaleMajorPentatonic_notes[]    = { C, D, E, G, A };

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