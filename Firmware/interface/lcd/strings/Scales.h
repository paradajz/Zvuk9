#ifndef SCALES_STRINGS_H_
#define SCALES_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM presetNaturalMinor_string[] PROGMEM = "Natural Minor";
const char PROGMEM presetMelodicMinor_string[] PROGMEM = "Melodic Minor";
const char PROGMEM presetHarmonicMinor_string[] PROGMEM = "Harmonic Minor";
const char PROGMEM presetMajor_string[] PROGMEM = "Major";
const char PROGMEM presetHarmonicMajor_string[] PROGMEM = "Harmonic Major";
const char PROGMEM presetMinorPentatonic_string[] PROGMEM = "Minor Pentatonic";
const char PROGMEM presetMajorPentatonic_string[] PROGMEM = "Major Pentatonic";
const char PROGMEM presetUser_string[] PROGMEM = "User preset ";

PGM_P const presetNameArray[] PROGMEM = {

    presetNaturalMinor_string,
    presetMelodicMinor_string,
    presetHarmonicMinor_string,
    presetMajor_string,
    presetHarmonicMajor_string,
    presetMinorPentatonic_string,
    presetMajorPentatonic_string,
    presetUser_string

};

const uint8_t presetNameArray_sizes[] PROGMEM = {

    sizeof(presetNaturalMinor_string),
    sizeof(presetMelodicMinor_string),
    sizeof(presetHarmonicMinor_string),
    sizeof(presetMajor_string),
    sizeof(presetHarmonicMajor_string),
    sizeof(presetMinorPentatonic_string),
    sizeof(presetMajorPentatonic_string),
    sizeof(presetUser_string)

};

#endif