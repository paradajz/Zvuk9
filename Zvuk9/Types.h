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

    ledIntensityOff,
    ledIntensityDim,
    ledIntensityFull

} ledIntensity_t;

typedef enum {

    splitOff,
    splitXY,
    splitXYFunctions,
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
    overflow,
    notAllowed

} changeOutput_t;

typedef enum {

    tonicChange,
    octaveChange,
    noteShift,
    noteAdded,
    noteRemoved

} noteChangeType_t;

typedef enum {

    coordinateZ,
    coordinateX,
    coordinateY

} coordinateType_t;

typedef enum {

    notUserPreset,
    padNotReleased,
    padEditEnter,
    padEditExit

} padEditModeResult_t;

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

typedef enum {

    text,
    message_std,
    message_keep

} lcdTextType;

#endif