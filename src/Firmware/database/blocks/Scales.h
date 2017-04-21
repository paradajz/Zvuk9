#pragma once

typedef enum
{
    scalePredefinedSection,
    scaleUserSection,
    SCALE_SECTIONS
} userScaleSection;

#define NUMBER_OF_USER_SCALES                       10

#define DEFAULT_OCTAVE                              4
#define PREDEFINED_SCALE_OCTAVE                     DEFAULT_OCTAVE
#define DEFAULT_NOTE                                (DEFAULT_OCTAVE*MIDI_NOTES)
#define BLANK_NOTE                                  128
#define PREDEFINED_SCALE_TONIC                      C
#define PREDEFINED_SCALE_SHIFT                      0

typedef enum
{
    PREDEFINED_SCALE_OCTAVE_ID,
    PREDEFINED_SCALE_TONIC_ID,
    PREDEFINED_SCALE_SHIFT_ID,
    PREDEFINED_SCALE_PARAMETERS
} predefinedScaleParameters;

const uint8_t defaultPredefinedScaleParametersArray[PREDEFINED_SCALE_PARAMETERS] =
{
    PREDEFINED_SCALE_OCTAVE,
    PREDEFINED_SCALE_TONIC,
    PREDEFINED_SCALE_SHIFT
};
