#pragma once

#include "../interface/pads/DataTypes.h"
#include "../interface/pads/curves/Curves.h"
#include "../midi/DataTypes.h"
#include "IDs.h"

//parameters
#define NUMBER_OF_PROGRAMS                          10

#define DEFAULT_OCTAVE                              4
#define MIDI_OCTAVE_RANGE                           11

#define MIN_MIDI_VALUE                              0
#define MAX_MIDI_VALUE                              127

#define DEFAULT_NOTE                                (DEFAULT_OCTAVE*MIDI_NOTES)
#define BLANK_NOTE                                  128

#define NUMBER_OF_USER_SCALES                       10

#define DEFAULT_ACTIVE_PROGRAM                      0
#define DEFAULT_ACTIVE_SCALE                        0

#define DEFAULT_PAD_X_LIMIT_LOWER                   440
#define DEFAULT_PAD_X_LIMIT_UPPER                   600

#define DEFAULT_PAD_Y_LIMIT_LOWER                   450
#define DEFAULT_PAD_Y_LIMIT_UPPER                   620

#define DEFAULT_PAD_PRESSURE_LIMIT_UPPER            380

//define default global program settings
#define GLOBAL_PROGRAM_SETTING_SPLIT_STATE          0x00
#define GLOBAL_PROGRAM_SETTING_X_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL         0x01
#define GLOBAL_PROGRAM_SETTING_CC_X                 0x14
#define GLOBAL_PROGRAM_SETTING_CC_Y                 0x15
#define GLOBAL_PROGRAM_SETTING_X_MIN                0
#define GLOBAL_PROGRAM_SETTING_X_MAX                127
#define GLOBAL_PROGRAM_SETTING_Y_MIN                0
#define GLOBAL_PROGRAM_SETTING_Y_MAX                127
#define GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN         curveLinear
#define GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN         curveLinear

//put global settings in array for easier access
const uint8_t defaultGlobalProgramSettingArray[GLOBAL_PROGRAM_SETTINGS] =
{
    GLOBAL_PROGRAM_SETTING_SPLIT_STATE,
    GLOBAL_PROGRAM_SETTING_X_ENABLE,
    GLOBAL_PROGRAM_SETTING_Y_ENABLE,
    GLOBAL_PROGRAM_SETTING_NOTE_ENABLE,
    GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL,
    GLOBAL_PROGRAM_SETTING_CC_X,
    GLOBAL_PROGRAM_SETTING_CC_Y,
    GLOBAL_PROGRAM_SETTING_X_MIN,
    GLOBAL_PROGRAM_SETTING_X_MAX,
    GLOBAL_PROGRAM_SETTING_Y_MIN,
    GLOBAL_PROGRAM_SETTING_Y_MAX,
    GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN,
    GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN
};

#define LOCAL_PROGRAM_SETTING_X_ENABLE              0x01
#define LOCAL_PROGRAM_SETTING_Y_ENABLE              0x01
#define LOCAL_PROGRAM_SETTING_NOTE_ENABLE           0x01
#define LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE     0x01
#define LOCAL_PROGRAM_SETTING_MIDI_CHANNEL          0x01
#define LOCAL_PROGRAM_SETTING_CC_X                  0x14
#define LOCAL_PROGRAM_SETTING_CC_Y                  0x15
#define LOCAL_PROGRAM_SETTING_X_MIN                 0
#define LOCAL_PROGRAM_SETTING_X_MAX                 127
#define LOCAL_PROGRAM_SETTING_Y_MIN                 0
#define LOCAL_PROGRAM_SETTING_Y_MAX                 127
#define LOCAL_PROGRAM_SETTING_X_CURVE_GAIN          curveLinear
#define LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN          curveLinear

const uint8_t defaultLocalProgramSettingArray[] =
{
    LOCAL_PROGRAM_SETTING_X_ENABLE,
    LOCAL_PROGRAM_SETTING_Y_ENABLE,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    LOCAL_PROGRAM_SETTING_MIDI_CHANNEL,
    LOCAL_PROGRAM_SETTING_CC_X,
    LOCAL_PROGRAM_SETTING_CC_Y,
    LOCAL_PROGRAM_SETTING_X_MIN,
    LOCAL_PROGRAM_SETTING_X_MAX,
    LOCAL_PROGRAM_SETTING_Y_MIN,
    LOCAL_PROGRAM_SETTING_Y_MAX,
    LOCAL_PROGRAM_SETTING_X_CURVE_GAIN,
    LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN
};

#define MIDI_SETTING_AFTERTOUCH_TYPE                aftertouchChannel
#define MIDI_SETTING_RUNNING_STATUS                 0x00 //disabled
#define MIDI_SETTING_NOTE_OFF_TYPE                  noteOffType_standardNoteOff
#define MIDI_SETTING_TRANSPORT_CC                   0x00 //disabled
#define MIDI_SETTING_TRANSPORT_CC_PLAY              125
#define MIDI_SETTING_TRANSPORT_CC_STOP              126
#define MIDI_SETTING_TRANSPORT_CC_RECORD            127

const uint8_t defaultMIDIsettingArray[] =
{
    MIDI_SETTING_AFTERTOUCH_TYPE,
    MIDI_SETTING_RUNNING_STATUS,
    MIDI_SETTING_NOTE_OFF_TYPE,
    MIDI_SETTING_TRANSPORT_CC,
    MIDI_SETTING_TRANSPORT_CC_PLAY,
    MIDI_SETTING_TRANSPORT_CC_STOP,
    MIDI_SETTING_TRANSPORT_CC_RECORD
};

#define PRESSURE_SETTING_SENSITIVITY                0x00
#define PRESSURE_SETTING_CURVE                      curveLinear

const uint8_t defaultPressureSettingsArray[] =
{
    PRESSURE_SETTING_SENSITIVITY,
    PRESSURE_SETTING_CURVE
};

#define PREDEFINED_SCALE_OCTAVE                     DEFAULT_OCTAVE
#define PREDEFINED_SCALE_TONIC                      C
#define PREDEFINED_SCALE_SHIFT                      0

const uint8_t defaultPredefinedScaleParametersArray[] =
{
    PREDEFINED_SCALE_OCTAVE,
    PREDEFINED_SCALE_TONIC,
    PREDEFINED_SCALE_SHIFT
};