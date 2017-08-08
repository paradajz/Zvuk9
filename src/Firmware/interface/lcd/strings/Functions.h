#pragma once

const char function_midiChannel[] PROGMEM = "Change MIDI channel with encoder";
const char function_play[] PROGMEM = "Play";
const char function_stop[] PROGMEM = "Stop";
const char function_record[] PROGMEM = "Record";
const char function_tonic[] PROGMEM = "Tonic changed!";
const char function_split[] PROGMEM = "Split mode";
const char function_x[] PROGMEM = "X sending";
const char function_y[] PROGMEM = "Y sending";
const char function_aftertouch[] PROGMEM = "Aftertouch sending";
const char function_notes[] PROGMEM = "Note sending";
const char function_octave[] PROGMEM = "Octave changed!";
const char function_noteShift[] PROGMEM = "Note shift level changed!";
const char function_xCC[] PROGMEM = "X CC changed!";
const char function_yCC[] PROGMEM = "Y CC changed!";
const char function_xMin[] PROGMEM = "X min changed!";
const char function_xMax[] PROGMEM = "X max changed!";
const char function_yMin[] PROGMEM = "Y min changed!";
const char function_yMax[] PROGMEM = "Y max changed!";
const char function_xCurve[] PROGMEM = "X curve changed!";
const char function_yCurve[] PROGMEM = "Y curve changed!";

const char function_value[] PROGMEM = "New value: ";

PGM_P const functionArray[] PROGMEM =
{
    NULL,
    NULL,
    function_midiChannel,
    function_play,
    function_stop,
    function_record,
    function_tonic,
    function_split,
    function_x,
    function_y,
    function_aftertouch,
    function_notes,
    function_octave,
    function_noteShift,
    NULL,
    function_xCC,
    function_yCC,
    function_xMin,
    function_xMax,
    function_yMin,
    function_yMax,
    function_xCurve,
    function_yCurve,
    NULL
};

const uint8_t functionArray_sizes[] PROGMEM =
{
    0,
    0,
    ARRAY_SIZE_CHAR(function_midiChannel),
    ARRAY_SIZE_CHAR(function_play),
    ARRAY_SIZE_CHAR(function_stop),
    ARRAY_SIZE_CHAR(function_record),
    ARRAY_SIZE_CHAR(function_tonic),
    ARRAY_SIZE_CHAR(function_split),
    ARRAY_SIZE_CHAR(function_x),
    ARRAY_SIZE_CHAR(function_y),
    ARRAY_SIZE_CHAR(function_aftertouch),
    ARRAY_SIZE_CHAR(function_notes),
    ARRAY_SIZE_CHAR(function_octave),
    ARRAY_SIZE_CHAR(function_noteShift),
    0,
    ARRAY_SIZE_CHAR(function_xCC),
    ARRAY_SIZE_CHAR(function_yCC),
    ARRAY_SIZE_CHAR(function_xMin),
    ARRAY_SIZE_CHAR(function_xMax),
    ARRAY_SIZE_CHAR(function_yMin),
    ARRAY_SIZE_CHAR(function_yMax),
    ARRAY_SIZE_CHAR(function_xCurve),
    ARRAY_SIZE_CHAR(function_yCurve),
    0
};
