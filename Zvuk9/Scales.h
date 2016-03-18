#ifndef SCALES_H_
#define SCALES_H_

//scale definitions starting from lowest octave
//pentatonic scales have 5 notes per octave
//other scales have 7

#include "Arduino.h"
#include "Types.h"

#define NUMBER_OF_PREDEFINED_SCALES             7
#define NUMBER_OF_USER_SCALES                   10

uint8_t getNotesPerScale(scale_t scale);
note_t getScaleNote(scale_t scale, uint8_t note);

const note_t scaleNaturalMinor_notes[]       = { C, D, D_SHARP, F, G, G_SHARP, A_SHARP };
const note_t scaleMelodicMinor_notes[]       = { C, D, D_SHARP, F, G, A, B };
const note_t scaleHarmonicMinor_notes[]      = { C, D, D_SHARP, F, G, G_SHARP, B };
const note_t scaleMajor_notes[]              = { C, D, E, F, G, A, B };
const note_t scaleHarmonicMajor_notes[]      = { C, D, E, F, G, G_SHARP, B };
const note_t scaleMinorPentatonic_notes[]    = { C, D_SHARP, F, G, A_SHARP };
const note_t scaleMajorPentatonic_notes[]    = { C, D, E, G, A };

#endif