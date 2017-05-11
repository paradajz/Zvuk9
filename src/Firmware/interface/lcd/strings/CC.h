#pragma once

const char xMax_string[] PROGMEM = "X max=";
const char xMin_string[] PROGMEM = "X min=";
const char yMax_string[] PROGMEM = "Y max=";
const char yMin_string[] PROGMEM = "Y min=";

const char xCC_string[] PROGMEM = "X CC=";
const char yCC_string[] PROGMEM = "Y CC=";

const char xCCid_string[] PROGMEM = "CCx";
const char yCCid_string[] PROGMEM = "CCy";

const char xPosition_string[] PROGMEM = "x";
const char yPosition_string[] PROGMEM = "y";

const char xyPositionClear_string[] PROGMEM = "    ";
const char xyCCclear_string[] PROGMEM = "      ";

PGM_P const ccArray[] PROGMEM =
{
    //match order with coordinateType_t enum
    xCC_string,
    yCC_string
};

const uint8_t ccArray_sizes[] PROGMEM =
{
    //match order with coordinateType_t enum
    sizeof(xCC_string) - 1,
    sizeof(yCC_string) - 1
};
