#pragma once

const char presetNaturalMinor_string[] PROGMEM = "Natural Minor";
const char presetMelodicMinor_string[] PROGMEM = "Melodic Minor";
const char presetHarmonicMinor_string[] PROGMEM = "Harmonic Minor";
const char presetMajor_string[] PROGMEM = "Major";
const char presetHarmonicMajor_string[] PROGMEM = "Harmonic Major";
const char presetMinorPentatonic_string[] PROGMEM = "Minor Pentatonic";
const char presetMajorPentatonic_string[] PROGMEM = "Major Pentatonic";
const char presetUser_string[] PROGMEM = "User scale";

PGM_P const presetNameArray[] PROGMEM =
{
    presetNaturalMinor_string,
    presetMelodicMinor_string,
    presetHarmonicMinor_string,
    presetMajor_string,
    presetHarmonicMajor_string,
    presetMinorPentatonic_string,
    presetMajorPentatonic_string,
    presetUser_string
};

const uint8_t presetNameArray_sizes[] PROGMEM =
{
    ARRAY_SIZE_CHAR(presetNaturalMinor_string),
    ARRAY_SIZE_CHAR(presetMelodicMinor_string),
    ARRAY_SIZE_CHAR(presetHarmonicMinor_string),
    ARRAY_SIZE_CHAR(presetMajor_string),
    ARRAY_SIZE_CHAR(presetHarmonicMajor_string) ,
    ARRAY_SIZE_CHAR(presetMinorPentatonic_string),
    ARRAY_SIZE_CHAR(presetMajorPentatonic_string),
    ARRAY_SIZE_CHAR(presetUser_string)
};
