#pragma once

///
/// \brief List of possible limits for various values (min or max).
///
typedef enum
{
    limitTypeMin,
    limitTypeMax
} limitType_t;

///
/// \brief List of all possible aftertouch types.
///
typedef enum
{
    aftertouchChannel,
    aftertouchPoly
} aftertouchType_t;

///
/// \brief List of all possible pad coordinates.
///
typedef enum
{
    coordinateX,
    coordinateY,
    coordinateZ,
    PAD_COORDINATES
} padCoordinate_t;

///
/// \brief List of all possible pad pressure types.
///
typedef enum
{
    pressureVelocity,
    pressureAftertouch
} pressureType_t;

///
/// \brief List of various functions on controller which can be turned on or off.
///
typedef enum
{
    function_notes,
    function_aftertouch,
    function_x,
    function_y,
    function_split
} function_t;

///
/// \brief List of all possible results when shifting notes.
///
typedef enum
{
    noChange,
    outputChanged,
    outOfRange,
    overflow,
    notAllowed
} changeResult_t;

///
/// \brief List of all possible types of changing MIDI notes.
///
typedef enum
{
    tonicChange,
    octaveChange,
    noteShift
} noteChangeType_t;

///
/// \brief List of all possible errors when entering pad edit mode.
///
typedef enum
{
    notUserPreset,
    padNotReleased,
} padEditModeResult_t;

///
/// \brief List of all possible errors which result when all pads aren't released.
///
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

///
/// \brief List of all possible velocity sensitivity levels.
///
typedef enum
{
    velocity_soft,
    velocity_medium,
    velocity_hard
} velocitySensitivity_t;

///
/// \brief List off all possible MIDI notes.
///
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

///
/// \brief List of all possible predefined scales.
///
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