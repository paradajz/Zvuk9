#pragma once

#define OCTAVE_NORMALIZATION    -2

#define progmemCharArraySize(x) (sizeof(x) / sizeof(x[0]) -1)
#define progmemArraySize(x)     (sizeof(x) / sizeof(x[0]))
#define normalizeOctave(octave) ((octave)+(OCTAVE_NORMALIZATION))
