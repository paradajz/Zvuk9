/*

Define custom data types.

*/

#ifndef TYPES_H_
#define TYPES_H_

#include "interface/leds/LEDsettings.h"

typedef enum {

    ccLimitTypeXmin = 0x00,
    ccLimitTypeXmax = 0x01,
    ccLimitTypeYmin = 0x02,
    ccLimitTypeYmax = 0x03

} ccLimitType_t;

typedef enum {

    ccTypeX = 0x00,
    ccTypeY = 0x01

} ccType_t;

typedef enum {

    ledIntensityOff = 0x00,
    ledIntensityDim = HALF_INTENSITY,
    ledIntensityFull = FULL_INTENSITY

} ledIntensity_t;

typedef enum {

    splitOff = 0x00,
    splitXY = 0x01,
    splitXYFunctions = 0x02,
    splitEnd

} splitState_t;

typedef enum {

    featureNotes,
    featureAftertouch,
    featureX,
    featureY,
    featureSplit,

} functionsOnOff_t;

typedef enum {

    transportStop,
    transportPlay,
    transportRecord

} transportControl_t;

typedef enum {

    noChange,
    outputChanged,
    outOfRange,
    notAllowed

} changeOutput_t;

typedef enum {

    noteChange,
    octaveChange,
    noteUpOrDown,
    ccChange,
    ccLimitChange,
    curveChange,
    midiChannelChange

} changeType_t;

typedef enum {

    coordinateZ,
    coordinateX,
    coordinateY

} coordinateType_t;

typedef enum {

    noUserPreset,
    padNotReleased

} padEditError_t;

typedef enum {

    regularLine,
    messageLine,
    scrollLine

} lcdLineType_t;

typedef enum {

    pressureVelocity,
    pressureAfterTouch

} pressureType_t;

typedef enum {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

} scaleType_t;

typedef enum {

    curveCoordinateX = 0x00,
    curveCoordinateY = 0x01,
    curveCoordinateInvalid

} curveCoordinate_t;

typedef enum {

    curveTypeLinear,
    curveTypeWideMiddle,
    curveTypeWideEnds,
    curveTypeInvalid

} curveType_t;

typedef enum {

    C,
    C_SHARP,
    D,
    D_SHARP,
    E,
    F,
    F_SHARP,
    G,
    G_SHARP,
    A,
    A_SHARP,
    B,
    MIDI_NOTES

} note_t;

typedef enum {

    scaleNaturalMinor,
    scaleMelodicMinor,
    scaleHarmonicMinor,
    scaleMajor,
    scaleHarmonicMajor,
    scaleMinorPentatonic,
    scaleMajorPentatonic,
    scaleUser

} scale_t;

typedef enum {

    userMenu,
    serviceMenu,
    noMenu

} menuType_t;

#endif /* TYPES_H_ */