#ifndef PADAMOUNT_STRINGS_H_
#define PADAMOUNT_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM padAmountSingle_string[] = "pad ";
const char PROGMEM padAmountAll_string[] = "all pads";

PGM_P const padAmountArray[] PROGMEM = {

    padAmountAll_string,
    padAmountSingle_string

};

#endif