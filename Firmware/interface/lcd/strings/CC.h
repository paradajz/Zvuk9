#ifndef CC_STRINGS_H_
#define CC_STRINGS_H_

#include <avr/pgmspace.h>

const char curveCoordinateX_string[] PROGMEM = "X curve=";
const char curveCoordinateY_string[] PROGMEM = "Y curve=";

const char curveTypeLinear_string[] PROGMEM = "linear";
const char curveTypeWideMiddle_string[] PROGMEM = "wide middle";
const char curveTypeWideEnds_string[] PROGMEM = "wide ends";

const char xMax_string[] PROGMEM = "X max=";
const char xMin_string[] PROGMEM = "X min=";
const char yMax_string[] PROGMEM = "Y max=";
const char yMin_string[] PROGMEM = "Y min=";

const char xCC_string[] PROGMEM = "X CC=";
const char yCC_string[] PROGMEM = "Y CC=";

const char xCCid_string[] PROGMEM = "ccx";
const char yCCid_string[] PROGMEM = "ccy";

const char xyPositionClear_string[] PROGMEM = "      ";
const char xyCCclear_string[] PROGMEM = "      ";

PGM_P const curveCoordinateArray[] PROGMEM = {

    curveCoordinateX_string,
    curveCoordinateY_string

};

const uint8_t curveCoordinateArray_sizes[] PROGMEM = {

    sizeof(curveCoordinateX_string) - 1,
    sizeof(curveCoordinateY_string) - 1

};

PGM_P const curveNameArray[] PROGMEM = {

    curveTypeLinear_string,
    curveTypeWideMiddle_string,
    curveTypeWideEnds_string

};

const uint8_t curveNameArray_sizes[] PROGMEM = {

    sizeof(curveTypeLinear_string) - 1,
    sizeof(curveTypeWideMiddle_string) - 1,
    sizeof(curveTypeWideEnds_string) - 1

};

PGM_P const ccLimitArray[] PROGMEM = {

    xMin_string,
    xMax_string,
    yMin_string,
    yMax_string

};

const uint8_t ccLimitArray_sizes[] PROGMEM = {

    sizeof(xMin_string) - 1,
    sizeof(xMax_string) - 1,
    sizeof(yMin_string) - 1,
    sizeof(yMax_string) - 1

};

PGM_P const ccArray[] PROGMEM = {

    xCC_string,
    yCC_string

};

const uint8_t ccArray_sizes[] PROGMEM = {

    sizeof(xCC_string) - 1,
    sizeof(yCC_string) - 1

};

#endif