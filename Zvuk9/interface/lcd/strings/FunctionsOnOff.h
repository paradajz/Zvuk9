#ifndef FUNCTIONSONOFF_STRINGS_H_
#define FUNCTIONSONOFF_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM offNotes_string[] PROGMEM = "Notes off";
const char PROGMEM offX_string[] PROGMEM = "X off";
const char PROGMEM offY_string[] PROGMEM = "Y off";

const char PROGMEM onNotes_string[] PROGMEM = "Notes on";
const char PROGMEM onX_string[] PROGMEM = "X on";
const char PROGMEM onY_string[] PROGMEM = "Y on";

const char PROGMEM splitXY_string[] PROGMEM = "Split X, Y";
const char PROGMEM splitXYFunctions_string[] PROGMEM = "+ functions";
const char PROGMEM splitOff_string[] PROGMEM = "Split off";

const char PROGMEM aftertouchOff_string[] PROGMEM = "Aftertouch off";
const char PROGMEM aftertouchChannel_string[] PROGMEM = "Channel aftertouch";
const char PROGMEM aftertouchPoly_string[] PROGMEM = "Poly aftertouch";

PGM_P const offArray[] PROGMEM = {

    offNotes_string,
    offX_string,
    offY_string

};

PGM_P const onArray[] PROGMEM = {

    onNotes_string,
    onX_string,
    onY_string

};

PGM_P const splitArray[] PROGMEM = {

    splitOff_string,
    splitXY_string,
    splitXYFunctions_string,

};

PGM_P const aftertouchArray[] PROGMEM = {

    aftertouchOff_string,
    aftertouchChannel_string,
    aftertouchPoly_string,

};

#endif