#ifndef DATATYPES_H_
#define DATATYPES_H_

typedef enum {

    ccLimitTypeMin,
    ccLimitTypeMax

} ccLimitType_t;

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
    PREDEFINED_SCALES

} scale_t;

typedef enum {

    curveTypeLinear = 5,
    curveTypeWideMiddle = 1,
    curveTypeWideEnds = 9

} curveType_t;

typedef enum {

    aftertouchChannel,
    aftertouchPoly

} aftertouchType_t;

typedef enum {

    coordinateX,
    coordinateY,
    coordinateZ,

} coordinateType_t;

typedef enum {

    pressureVelocity,
    pressureAftertouch

} pressureType_t;

typedef enum {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

} scaleType_t;

typedef enum {

    onOff_notes,
    onOff_aftertouch,
    onOff_x,
    onOff_y,
    onOff_split

} onOff_t;

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

    notUserPreset,
    padNotReleased,
    padEditEnter,
    padEditExit

} padEditModeResult_t;

typedef enum {

    changeProgram,
    changePreset,
    changeCCnumber,
    changeCClimit,
    changeCurve,
    calibrationMode,
    enterPadEditMode

} padReleaseError_t;

typedef enum {

    lower,
    upper

} calibrationDirection;

#endif