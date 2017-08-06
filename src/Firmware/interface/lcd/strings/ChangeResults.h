#pragma once

const char changeResult_noChange_string[] PROGMEM = "no change in values";
const char changeResult_valueChanged_string[] PROGMEM = "values changed";
const char changeResult_outOfRange_string[] PROGMEM = "values out of range";
const char changeResult_notAllowed_string[] PROGMEM = "not allowed";
const char changeResult_releasePads_string[] PROGMEM = "pads not released";
const char changeResult_notUserScale_string[] PROGMEM = "user scale not active";
const char changeResult_notPredefinedScale_string[] PROGMEM = "predefined scale not active";

const char settinType_singlePad_string[] PROGMEM = "Value applied to pad ";
const char settinType_allPads_string[] PROGMEM = "Value applied to all pads";

PGM_P const changeResultArray[] PROGMEM =
{
    changeResult_noChange_string,
    changeResult_valueChanged_string,
    changeResult_outOfRange_string,
    changeResult_notAllowed_string,
    changeResult_releasePads_string,
    changeResult_notUserScale_string,
    changeResult_notPredefinedScale_string
};

const uint8_t changeResultArray_sizes[] PROGMEM =
{
    ARRAY_SIZE_CHAR(changeResult_noChange_string),
    ARRAY_SIZE_CHAR(changeResult_valueChanged_string),
    ARRAY_SIZE_CHAR(changeResult_outOfRange_string),
    ARRAY_SIZE_CHAR(changeResult_notAllowed_string),
    ARRAY_SIZE_CHAR(changeResult_releasePads_string),
    ARRAY_SIZE_CHAR(changeResult_notUserScale_string),
    ARRAY_SIZE_CHAR(changeResult_notPredefinedScale_string),
};
