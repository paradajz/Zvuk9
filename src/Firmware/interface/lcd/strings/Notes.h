#pragma once

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

const char padEditOctave_string[] PROGMEM = "Octave ";
const char assignedNotes_string[] PROGMEM = "Assigned notes:";

const char velocity_string[] PROGMEM = "v: ";
const char aftertouch_string[] PROGMEM = "at: ";

const char noteShift_string[] PROGMEM = "n";

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
    ARRAY_SIZE_CHAR(tonicC_string),
    ARRAY_SIZE_CHAR(tonicCSharp_string),
    ARRAY_SIZE_CHAR(tonicD_string),
    ARRAY_SIZE_CHAR(tonicDSharp_string),
    ARRAY_SIZE_CHAR(tonicE_string),
    ARRAY_SIZE_CHAR(tonicF_string),
    ARRAY_SIZE_CHAR(tonicFSharp_string),
    ARRAY_SIZE_CHAR(tonicG_string),
    ARRAY_SIZE_CHAR(tonicGSharp_string),
    ARRAY_SIZE_CHAR(tonicA_string),
    ARRAY_SIZE_CHAR(tonicASharp_string),
    ARRAY_SIZE_CHAR(tonicB_strig)
};
