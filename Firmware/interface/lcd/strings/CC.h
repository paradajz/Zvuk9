#ifndef CC_STRINGS_H_
#define CC_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM curveCoordinateX_string[] PROGMEM = "X curve=";
const char PROGMEM curveCoordinateY_string[] PROGMEM = "Y curve=";

const char PROGMEM curveTypeLinear_string[] PROGMEM = "linear";
const char PROGMEM curveTypeWideMiddle_string[] PROGMEM = "wide middle";
const char PROGMEM curveTypeWideEnds_string[] PROGMEM = "wide ends";

const char PROGMEM xMax_string[] PROGMEM = "X max=";
const char PROGMEM xMin_string[] PROGMEM = "X min=";
const char PROGMEM yMax_string[] PROGMEM = "Y max=";
const char PROGMEM yMin_string[] PROGMEM = "Y min=";

const char PROGMEM xCC_string[] PROGMEM = "X CC=";
const char PROGMEM yCC_string[] PROGMEM = "Y CC=";

const char PROGMEM xCCid_string[] PROGMEM = "ccx";
const char PROGMEM yCCid_string[] PROGMEM = "ccy";


const char PROGMEM xyPositionClear_string[] PROGMEM = "      ";
const char PROGMEM xyCCclear_string[] PROGMEM = "      ";

PGM_P const curveCoordinateArray[] PROGMEM = {

    curveCoordinateX_string,
    curveCoordinateY_string

};

const uint8_t curveCoordinateArray_sizes[] PROGMEM = {

    sizeof(curveCoordinateX_string),
    sizeof(curveCoordinateY_string)

};

PGM_P const curveNameArray[] PROGMEM = {

    curveTypeLinear_string,
    curveTypeWideMiddle_string,
    curveTypeWideEnds_string

};

const uint8_t curveNameArray_sizes[] PROGMEM = {

    sizeof(curveTypeLinear_string),
    sizeof(curveTypeWideMiddle_string),
    sizeof(curveTypeWideEnds_string)

};

PGM_P const ccLimitArray[] PROGMEM = {

    xMin_string,
    xMax_string,
    yMin_string,
    yMax_string

};

const uint8_t ccLimitArray_sizes[] PROGMEM = {

    sizeof(xMin_string),
    sizeof(xMax_string),
    sizeof(yMin_string),
    sizeof(yMax_string)

};

PGM_P const ccArray[] PROGMEM = {

    xCC_string,
    yCC_string

};

const uint8_t ccArray_sizes[] PROGMEM = {

    sizeof(xCC_string),
    sizeof(yCC_string)

};

#endif