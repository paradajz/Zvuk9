#pragma once

#include "../../interface/pads/curves/Curves.h"
#include "../../midi/src/DataTypes.h"
#include "../../interface/pads/DataTypes.h"
#include "../../interface/buttons/DataTypes.h"

typedef enum
{
    globalSettingsMIDI,
    globalSettingsVelocitySensitivity,
    GLOBAL_SETTINGS_SECTIONS
} globalSettingsSection;

#define MIDI_SETTING_AFTERTOUCH_TYPE                aftertouchChannel
#define MIDI_SETTING_RUNNING_STATUS                 0 //disabled
#define MIDI_SETTING_NOTE_OFF_TYPE                  noteOffType_standardNoteOff
#define MIDI_SETTING_TRANSPORT_CC                   transportMMC
#define MIDI_SETTING_PITCH_BEND_TYPE                pitchBend1

typedef enum
{
    MIDI_SETTING_AFTERTOUCH_TYPE_ID,
    MIDI_SETTING_RUNNING_STATUS_ID,
    MIDI_SETTING_NOTE_OFF_TYPE_ID,
    MIDI_SETTING_TRANSPORT_CC_ID,
    MIDI_SETTING_PITCH_BEND_TYPE_ID,
    MIDI_SETTING_RESERVED_1_ID,
    MIDI_SETTING_RESERVED_2_ID,
    MIDI_SETTING_RESERVED_3_ID,
    MIDI_SETTINGS
} midiSettings;

const uint8_t defaultMIDIsettingArray[MIDI_SETTINGS] =
{
    MIDI_SETTING_AFTERTOUCH_TYPE,
    MIDI_SETTING_RUNNING_STATUS,
    MIDI_SETTING_NOTE_OFF_TYPE,
    MIDI_SETTING_TRANSPORT_CC,
    MIDI_SETTING_PITCH_BEND_TYPE,
    0,  //reserved
    0,  //reserved
    0   //reserved
};

#define VELOCITY_SETTING_SENSITIVITY                velocity_soft
#define VELOCITY_SETTING_CURVE                      curve_linear_up

typedef enum
{
    VELOCITY_SETTING_SENSITIVITY_ID,
    VELOCITY_SETTING_CURVE_ID,
    VELOCITY_SETTINGS
} velocitySettings;

const uint8_t defaultVelocitySettingsArray[VELOCITY_SETTINGS] =
{
    VELOCITY_SETTING_SENSITIVITY,
    VELOCITY_SETTING_CURVE
};
