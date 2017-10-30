/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

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
