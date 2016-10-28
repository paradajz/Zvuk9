#pragma once

#include <avr/pgmspace.h>

const char tonicC_string[] PROGMEM = "C";
const char tonicCSharp_string[] PROGMEM = "C#";
const char tonicD_string[] PROGMEM = "D";
const char tonicDSharp_string[] PROGMEM = "D#";
const char tonicE_string[] PROGMEM = "E";
const char tonicF_string[] PROGMEM = "F";
const char tonicFSharp_string[] PROGMEM = "F#";
const char tonicG_string[] PROGMEM = "G";
const char tonicGSharp_string[] PROGMEM = "G#";
const char tonicA_string[] PROGMEM = "A";
const char tonicASharp_string[] PROGMEM = "A#";
const char tonicB_strig[] PROGMEM = "B";

const char noNotes_string[] PROGMEM = "<no assigned notes>";

const char tonic_string[] PROGMEM = "Tonic ";
const char octave_string[] PROGMEM = "Octave ";
const char activeOctave_string[] PROGMEM = "Active octave: ";

const char padEditMode_string[] PROGMEM = "Pad edit mode";

const char notesShifted_string[] PROGMEM = "Notes shifted";
const char notesShiftLevel_string[] PROGMEM = "Current level: ";

const char velocity_string[] PROGMEM = "v";
const char velocityClear_string[] PROGMEM = "    ";
const char notesClear_editMode_string[] PROGMEM = "                         ";
const char notesClear_string[] PROGMEM = "     ";

const char noteShift_string[] PROGMEM = "n";
const char noteShiftClear_string[] PROGMEM = "   ";

PGM_P const noteNameArray[] PROGMEM =
{
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

const uint8_t noteNameArray_sizes[] PROGMEM =
{
    sizeof(tonicC_string) - 1,
    sizeof(tonicCSharp_string) - 1,
    sizeof(tonicD_string) - 1,
    sizeof(tonicDSharp_string) - 1,
    sizeof(tonicE_string) - 1,
    sizeof(tonicF_string) - 1,
    sizeof(tonicFSharp_string) - 1,
    sizeof(tonicG_string) - 1,
    sizeof(tonicGSharp_string) - 1,
    sizeof(tonicA_string) - 1,
    sizeof(tonicASharp_string) - 1,
    sizeof(tonicB_strig) - 1
};
