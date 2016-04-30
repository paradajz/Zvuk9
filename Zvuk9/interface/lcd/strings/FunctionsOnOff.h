#ifndef FUNCTIONSONOFF_STRINGS_H_
#define FUNCTIONSONOFF_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM offNotes_string[] PROGMEM = "Notes off";
const char PROGMEM offAftertouch_string[] PROGMEM = "Aftertouch off";
const char PROGMEM offX_string[] PROGMEM = "X off";
const char PROGMEM offY_string[] PROGMEM = "Y off";

const char PROGMEM onNotes_string[] PROGMEM = "Notes on";
const char PROGMEM onAftertouch_string[] PROGMEM = "Aftertouch on";
const char PROGMEM onX_string[] PROGMEM = "X on";
const char PROGMEM onY_string[] PROGMEM = "Y on";

const char PROGMEM splitOn_string[] PROGMEM = "Split on";
const char PROGMEM splitOff_string[] PROGMEM = "Split off";

const char PROGMEM aftertouchPoly_string[] PROGMEM = "Key aftertouch";
const char PROGMEM aftertouchChannel_string[] PROGMEM = "Channel aftertouch";

PGM_P const offArray[] PROGMEM = {

    offNotes_string,
    offAftertouch_string,
    offX_string,
    offY_string

};

const uint8_t offArray_sizes[] PROGMEM = {

    sizeof(offNotes_string),
    sizeof(offAftertouch_string),
    sizeof(offX_string),
    sizeof(offY_string)

};

PGM_P const onArray[] PROGMEM = {

    onNotes_string,
    onAftertouch_string,
    onX_string,
    onY_string

};

const uint8_t onArray_sizes[] PROGMEM = {

    sizeof(onNotes_string),
    sizeof(onAftertouch_string),
    sizeof(onX_string),
    sizeof(onY_string)

};

PGM_P const splitArray[] PROGMEM = {

    splitOff_string,
    splitOn_string

};

const uint8_t splitArray_sizes[] PROGMEM = {

    sizeof(splitOff_string),
    sizeof(splitOn_string)

};

PGM_P const aftertouchTypeArray[] PROGMEM = {

    aftertouchPoly_string,
    aftertouchChannel_string

};

const uint8_t aftertouchTypeArray_sizes[] PROGMEM = {

    sizeof(aftertouchPoly_string),
    sizeof(aftertouchChannel_string)

};

#endif