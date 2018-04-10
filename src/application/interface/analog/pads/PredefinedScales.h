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

#include "DataTypes.h"

///
/// \ingroup interfacePads
/// @{

///
/// \brief Various predefined scales defined as arrays of MIDI notes.
/// @{

const note_t scaleNaturalMinor_notes[]       = { C, D, D_SHARP, F, G, G_SHARP, A_SHARP };
const note_t scaleMelodicMinor_notes[]       = { C, D, D_SHARP, F, G, A, B };
const note_t scaleHarmonicMinor_notes[]      = { C, D, D_SHARP, F, G, G_SHARP, B };
const note_t scaleMajor_notes[]              = { C, D, E, F, G, A, B };
const note_t scaleHarmonicMajor_notes[]      = { C, D, E, F, G, G_SHARP, B };
const note_t scaleMinorPentatonic_notes[]    = { C, D_SHARP, F, G, A_SHARP };
const note_t scaleMajorPentatonic_notes[]    = { C, D, E, G, A };

/// @}

///
/// \brief Predefined arrays stored into array for easier access.
///
const note_t *scale_notes[PREDEFINED_SCALES] =
{
    scaleNaturalMinor_notes,
    scaleMelodicMinor_notes,
    scaleHarmonicMinor_notes,
    scaleMajor_notes,
    scaleHarmonicMajor_notes,
    scaleMinorPentatonic_notes,
    scaleMajorPentatonic_notes
};

///
/// \brief Sizes of each predefined scale stored into array for easier manipulation.
///
const uint8_t scale_notes_sizes[PREDEFINED_SCALES] =
{
    ARRAY_SIZE(scaleNaturalMinor_notes),
    ARRAY_SIZE(scaleMelodicMinor_notes),
    ARRAY_SIZE(scaleHarmonicMinor_notes),
    ARRAY_SIZE(scaleMajor_notes),
    ARRAY_SIZE(scaleHarmonicMajor_notes),
    ARRAY_SIZE(scaleMinorPentatonic_notes),
    ARRAY_SIZE(scaleMajorPentatonic_notes)
};

/// @}
