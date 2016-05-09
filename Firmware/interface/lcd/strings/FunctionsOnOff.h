#ifndef FUNCTIONSONOFF_STRINGS_H_
#define FUNCTIONSONOFF_STRINGS_H_

#include <avr/pgmspace.h>

const char offNotes_string[] PROGMEM = "Notes off";
const char offAftertouch_string[] PROGMEM = "Aftertouch off";
const char offX_string[] PROGMEM = "X off";
const char offY_string[] PROGMEM = "Y off";

const char onNotes_string[] PROGMEM = "Notes on";
const char onAftertouch_string[] PROGMEM = "Aftertouch on";
const char onX_string[] PROGMEM = "X on";
const char onY_string[] PROGMEM = "Y on";

const char splitOn_string[] PROGMEM = "Split on";
const char splitOff_string[] PROGMEM = "Split off";

const char aftertouchPoly_string[] PROGMEM = "Key aftertouch";
const char aftertouchChannel_string[] PROGMEM = "Channel aftertouch";

PGM_P const offArray[] PROGMEM = {

    offNotes_string,
    offAftertouch_string,
    offX_string,
    offY_string

};

const uint8_t offArray_sizes[] PROGMEM = {

    sizeof(offNotes_string) - 1,
    sizeof(offAftertouch_string) - 1,
    sizeof(offX_string) - 1,
    sizeof(offY_string) - 1

};

PGM_P const onArray[] PROGMEM = {

    onNotes_string,
    onAftertouch_string,
    onX_string,
    onY_string

};

const uint8_t onArray_sizes[] PROGMEM = {

    sizeof(onNotes_string) - 1,
    sizeof(onAftertouch_string) - 1,
    sizeof(onX_string) - 1,
    sizeof(onY_string) - 1

};

PGM_P const splitArray[] PROGMEM = {

    splitOff_string,
    splitOn_string

};

const uint8_t splitArray_sizes[] PROGMEM = {

    sizeof(splitOff_string) - 1,
    sizeof(splitOn_string) - 1

};

PGM_P const aftertouchTypeArray[] PROGMEM = {

    aftertouchPoly_string,
    aftertouchChannel_string

};

const uint8_t aftertouchTypeArray_sizes[] PROGMEM = {

    sizeof(aftertouchPoly_string) - 1,
    sizeof(aftertouchChannel_string) - 1

};

#endif