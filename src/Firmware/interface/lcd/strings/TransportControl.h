#pragma once

const char transportControlPlay_string[] PROGMEM = "Play";
const char transportControlStop_string[] PROGMEM = "Stop";
const char transportControlRecordOn_string[] PROGMEM = "Record on";
const char transportControlRecordOff_string[] PROGMEM = "Record off";

PGM_P const transportControlChangeArray[] PROGMEM =
{
    transportControlPlay_string,
    transportControlStop_string,
    transportControlRecordOn_string,
    transportControlRecordOff_string
};

const uint8_t transportControlChangeArray_sizes[] PROGMEM =
{
    sizeof(transportControlPlay_string) - 1,
    sizeof(transportControlStop_string) - 1,
    sizeof(transportControlRecordOn_string) - 1,
    sizeof(transportControlRecordOff_string) - 1
};
