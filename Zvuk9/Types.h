/*

Define custom data types.

*/

#ifndef TYPES_H_
#define TYPES_H_

typedef enum {

    ccLimitTypeXmin,
    ccLimitTypeXmax,
    ccLimitTypeYmin,
    ccLimitTypeYmax

} ccLimitType_t;

typedef enum {

    ccTypeX,
    ccTypeY

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
    featureAftertouchType

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

    pressureVelocity,
    pressureAfterTouch

} pressureType_t;

typedef enum {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

} scaleType_t;

typedef enum {

    curveCoordinateX,
    curveCoordinateY,
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
    message

} lcdTextType;

typedef enum {

    changeParameters,
    changeTonic,
    enterPadEditMode

} padReleaseError_t;

typedef enum {

    noMessage,
    showMessage,
    messageDisplayed,
    clearMessage

} messageStatus_t;

typedef enum {

    lastActiveID,
    previousID

} padHistoryID_t;

typedef enum {

    aftertouchPoly,
    aftertouchChannel,
    AFTERTOUCH_TYPES

} aftertouchType_t;

#endif