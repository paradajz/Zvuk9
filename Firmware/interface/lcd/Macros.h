#ifndef MACROS_H_
#define MACROS_H_

#define OCTAVE_NORMALIZATION            -2

#define progmemCharArraySize(x) (sizeof(x) / sizeof(x[0]) -1)
#define normalizeOctave(octave) ((octave)+(OCTAVE_NORMALIZATION))


#endif