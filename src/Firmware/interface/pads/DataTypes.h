#pragma once

typedef enum
{
    limitTypeMin,
    limitTypeMax
} limitType_t;

typedef enum
{
    aftertouchChannel,
    aftertouchPoly
} aftertouchType_t;

typedef enum
{
    coordinateX,
    coordinateY,
    coordinateZ,
    PAD_COORDINATES
} padCoordinate_t;

typedef enum
{
    pressureVelocity,
    pressureAftertouch
} pressureType_t;

typedef enum
{
    function_notes,
    function_aftertouch,
    function_x,
    function_y,
    function_split
} function_t;

typedef enum
{
    noChange,
    outputChanged,
    outOfRange,
    overflow,
    notAllowed
} changeResult_t;

typedef enum
{
    tonicChange,
    octaveChange,
    noteShift,
    noteAdded,
    noteRemoved
} noteChangeType_t;

typedef enum
{
    notUserPreset,
    padNotReleased,
    padEditEnter,
    padEditExit
} padEditModeResult_t;

typedef enum
{
    changeProgram,
    changePreset,
    changeCCnumber,
    changeCClimit,
    changeCurve,
    calibrationMode,
    enterPadEditMode
} padReleaseError_t;

typedef enum
{
    velocity_soft,
    velocity_medium,
    velocity_hard
} velocitySensitivity_t;

typedef enum
{
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

typedef enum
{
    scaleNaturalMinor,
    scaleMelodicMinor,
    scaleHarmonicMinor,
    scaleMajor,
    scaleHarmonicMajor,
    scaleMinorPentatonic,
    scaleMajorPentatonic,
    PREDEFINED_SCALES
} scale_t;