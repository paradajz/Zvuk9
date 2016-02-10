/*

Define custom data types.

*/

#ifndef TYPES_H_
#define TYPES_H_

#include "interface/leds/LEDsettings.h"

enum ccLimitType_t {

    ccLimitTypeXmin = 0x00,
    ccLimitTypeXmax = 0x01,
    ccLimitTypeYmin = 0x02,
    ccLimitTypeYmax = 0x03

};

enum ccType_t {

    ccTypeX = 0x00,
    ccTypeY = 0x01

};

enum ledIntensity_t   {

    ledIntensityOff = 0x00,
    ledIntensityDim = HALF_INTENSITY,
    ledIntensityFull = FULL_INTENSITY

};

enum splitState_t {

    splitOff = 0x00,
    splitXY = 0x01,
    splitXYFunctions = 0x02,
    splitEnd

};

enum functionsOnOff_t    {

    featureNotes,
    featureAftertouch,
    featureX,
    featureY,
    featureSplit,

};

enum transportControl_t {

    transportStop,
    transportPlay,
    transportRecord

};

enum changeOutput_t {

    noChange,
    outputChanged,
    outOfRange,
    notAllowed

};

enum changeType_t {

    noteChange,
    octaveChange,
    noteUpOrDown,
    ccChange,
    ccLimitChange,
    curveChange,
    midiChannelChange

};

enum coordinateType_t {

    coordinateZ,
    coordinateX,
    coordinateY

};

enum padEditError_t   {

    noUserPreset,
    padNotReleased

};

enum lcdLineType_t {

    regularLine,
    messageLine,
    scrollLine

};

enum pressureType_t   {

    pressureVelocity,
    pressureAfterTouch

};

enum scaleType_t {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

};

enum tonic_t {

    tonicC,
    tonicCSharp,
    tonicD,
    tonicDSharp,
    tonicE,
    tonicF,
    tonicFSharp,
    tonicG,
    tonicGSharp,
    tonicA,
    tonicASharp,
    tonicB,
    tonicInvalid

};

enum curveCoordinate_t  {

    curveCoordinateX = 0x00,
    curveCoordinateY = 0x01,
    curveCoordinateInvalid

};

enum curveType_t {

    curveTypeLinear,
    curveTypeWideMiddle,
    curveTypeWideEnds,
    curveTypeInvalid

};

enum buttonState_t    {

    buttonReleased,
    buttonPressed

};

#endif /* TYPES_H_ */