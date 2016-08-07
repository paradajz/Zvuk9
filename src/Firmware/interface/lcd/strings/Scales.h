#ifndef SCALES_STRINGS_H_
#define SCALES_STRINGS_H_

#include <avr/pgmspace.h>

const char presetNaturalMinor_string[] PROGMEM = "Natural Minor";
const char presetMelodicMinor_string[] PROGMEM = "Melodic Minor";
const char presetHarmonicMinor_string[] PROGMEM = "Harmonic Minor";
const char presetMajor_string[] PROGMEM = "Major";
const char presetHarmonicMajor_string[] PROGMEM = "Harmonic Major";
const char presetMinorPentatonic_string[] PROGMEM = "Minor Pentatonic";
const char presetMajorPentatonic_string[] PROGMEM = "Major Pentatonic";
const char presetUser_string[] PROGMEM = "User preset";

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

    sizeof(presetNaturalMinor_string) - 1,
    sizeof(presetMelodicMinor_string) - 1,
    sizeof(presetHarmonicMinor_string) - 1,
    sizeof(presetMajor_string) - 1,
    sizeof(presetHarmonicMajor_string) - 1,
    sizeof(presetMinorPentatonic_string) - 1,
    sizeof(presetMajorPentatonic_string) - 1,
    sizeof(presetUser_string) - 1

};

#endif