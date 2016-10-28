#pragma once

#include <avr/pgmspace.h>

const char offNotes_string[] PROGMEM = "Notes off";
const char offAftertouch_string[] PROGMEM = "Aftertouch off";
const char offX_string[] PROGMEM = "X off";
const char offY_string[] PROGMEM = "Y off";
const char offSplit_string[] PROGMEM = "Split off";

const char onNotes_string[] PROGMEM = "Notes on";
const char onAftertouch_string[] PROGMEM = "Aftertouch on";
const char onX_string[] PROGMEM = "X on";
const char onY_string[] PROGMEM = "Y on";
const char onSplit_string[] PROGMEM = "Split on";

const char aftertouchPoly_string[] PROGMEM = "Key aftertouch";
const char aftertouchChannel_string[] PROGMEM = "Channel aftertouch";

PGM_P const offArray[] PROGMEM =
{
    offNotes_string,
    offAftertouch_string,
    offX_string,
    offY_string,
    offSplit_string
};

const uint8_t offArray_sizes[] PROGMEM =
{
    sizeof(offNotes_string) - 1,
    sizeof(offAftertouch_string) - 1,
    sizeof(offX_string) - 1,
    sizeof(offY_string) - 1,
    sizeof(offSplit_string) - 1
};

PGM_P const onArray[] PROGMEM =
{
    onNotes_string,
    onAftertouch_string,
    onX_string,
    onY_string,
    onSplit_string
};

const uint8_t onArray_sizes[] PROGMEM =
{
    sizeof(onNotes_string) - 1,
    sizeof(onAftertouch_string) - 1,
    sizeof(onX_string) - 1,
    sizeof(onY_string) - 1,
    sizeof(onSplit_string) - 1,
};
