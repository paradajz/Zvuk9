#ifndef MIDI_PARAMETERS_H_
#define MIDI_PARAMETERS_H_

#include "Scales.h"

#define MIN_MIDI_VALUE                      0
#define MAX_MIDI_VALUE                      127

#define MIDI_OCTAVE_RANGE                   12

#define NOTES_PER_PAD                       7

#define DEFAULT_OCTAVE                      4

#define DEFAULT_NOTE                        (DEFAULT_OCTAVE*MIDI_NOTES)
#define BLANK_NOTE                          0x80

#define DEFAULT_MIDI_CHANNEL                1

//number of bytes in system exclusive message
#define SYS_EX_ARRAY_SIZE           6

#endif /* MIDI_PARAMETERS_H_ */