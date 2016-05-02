#ifndef TRANSPORTCONTROL_STRINGS_H_
#define TRANSPORTCONTROL_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM transportControlPlay_string[] PROGMEM = "Play";
const char PROGMEM transportControlStop_string[] PROGMEM = "Stop";
const char PROGMEM transportControlRecordOn_string[] PROGMEM = "Record on";
const char PROGMEM transportControlRecordOff_string[] PROGMEM = "Record off";

PGM_P const transportControlChangeArray[] PROGMEM = {

    transportControlPlay_string,
    transportControlStop_string,
    transportControlRecordOn_string,
    transportControlRecordOff_string

};

const uint8_t transportControlChangeArray_sizes[] PROGMEM = {

    sizeof(transportControlPlay_string),
    sizeof(transportControlStop_string),
    sizeof(transportControlRecordOn_string),
    sizeof(transportControlRecordOff_string)

};

#endif