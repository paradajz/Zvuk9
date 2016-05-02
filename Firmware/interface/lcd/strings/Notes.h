#ifndef NOTES_STRINGS_H_
#define NOTES_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM tonicC_string[] PROGMEM = "C";
const char PROGMEM tonicCSharp_string[] PROGMEM = "C#";
const char PROGMEM tonicD_string[] PROGMEM = "D";
const char PROGMEM tonicDSharp_string[] PROGMEM = "D#";
const char PROGMEM tonicE_string[] PROGMEM = "E";
const char PROGMEM tonicF_string[] PROGMEM = "F";
const char PROGMEM tonicFSharp_string[] PROGMEM = "F#";
const char PROGMEM tonicG_string[] PROGMEM = "G";
const char PROGMEM tonicGSharp_string[] PROGMEM = "G#";
const char PROGMEM tonicA_string[] PROGMEM = "A";
const char PROGMEM tonicASharp_string[] PROGMEM = "A#";
const char PROGMEM tonicB_strig[] PROGMEM = "B";

const char PROGMEM tonic_string[] PROGMEM = "Tonic ";
const char PROGMEM octave_string[] PROGMEM = "Octave ";
const char PROGMEM activeOctave_string[] PROGMEM = "Active octave: ";

const char PROGMEM noteDown_string[] PROGMEM = "One note down";
const char PROGMEM noteUp_string[] PROGMEM = "One note up";

PGM_P const noteNameArray[] PROGMEM = {

    tonicC_string,
    tonicCSharp_string,
    tonicD_string,
    tonicDSharp_string,
    tonicE_string,
    tonicF_string,
    tonicFSharp_string,
    tonicG_string,
    tonicGSharp_string,
    tonicA_string,
    tonicASharp_string,
    tonicB_strig

};

const uint8_t noteNameArray_sizes[] PROGMEM = {

    sizeof(tonicC_string),
    sizeof(tonicCSharp_string),
    sizeof(tonicD_string),
    sizeof(tonicDSharp_string),
    sizeof(tonicE_string),
    sizeof(tonicF_string),
    sizeof(tonicFSharp_string),
    sizeof(tonicG_string),
    sizeof(tonicGSharp_string),
    sizeof(tonicA_string),
    sizeof(tonicASharp_string),
    sizeof(tonicB_strig)

};

#endif