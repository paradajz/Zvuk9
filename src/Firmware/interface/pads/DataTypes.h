#pragma once

///
/// \ingroup interfacePads
/// @{

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
/// \brief List of all possible results changing various values.
///
typedef enum
{
    noChange,
    valueChanged,
    outOfRange,
    notAllowed,
    releasePads,
    notUserScale,
    notPredefinedScale
} changeResult_t;

///
/// \brief List of all functions available via buttons and encoders.
///
typedef enum
{
    functionProgram,
    functionScale,
    functionChannel,
    functionPlay,
    functionStop,
    functionRecord,
    functionNotes,
    functionOnOffSplit,
    functionOnOffX,
    functionOnOffY,
    functionOnOffAftertouch,
    functionOnOffNotes,
    functionOctave,
    functionNoteShift,
    functionPadEditMode,
    functionXCC,
    functionYCC,
    functionXPitchBend,
    functionYPitchBend,
    functionXMinLimit,
    functionXMaxLimit,
    functionYMinLimit,
    functionYMaxLimit,
    functionXCurve,
    functionYCurve,
    functionMenu
} function_t;

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

///
/// \brief List of all possible pitch bend types.
///
typedef enum
{
    pitchBend1,
    pitchBend2
} pitchBendType_t;

///
/// \brief List of all possible scaling types when scaling values read from sensors.
///
typedef enum
{
    rawScale,
    midiScale_7b,
    midiScale_14b
} valueScaleType_t;

/// @}