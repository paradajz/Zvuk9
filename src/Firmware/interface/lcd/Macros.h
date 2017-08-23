#pragma once

///
/// \brief Actual octave ranges from 0 to 11.
/// This macro subtracts calculated octave by OCTAVE_NORMALIZATION value and displays
/// normalized value on display.
///
#define normalizeOctave(octave)     ((octave)+(OCTAVE_NORMALIZATION))