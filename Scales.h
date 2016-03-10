#ifndef SCALES_H_
#define SCALES_H_

//scale definitions starting from lowest octave
//pentatonic scales have 5 notes per octave
//other scales have 7

#include "Arduino.h"

typedef enum {

    C,
    C_SHARP,
    D,
    D_SHARP,
    E,
    F,
    F_SHARP,
    G,
    G_SHARP,
    A,
    A_SHARP,
    B,
    MIDI_NOTES

} note_t;

enum scale_t {

    scaleNaturalMinor,
    scaleMelodicMinor,
    scaleHarmonicMinor,
    scaleMajor,
    scaleHarmonicMajor,
    scaleMinorPentatonic,
    scaleMajorPentatonic,
    scaleUser

};

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