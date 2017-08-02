#pragma once

#include "../../interface/pads/curves/Curves.h"
#include "../../midi/src/DataTypes.h"

typedef enum
{
    globalSettingsMIDI,
    globalSettingsVelocitySensitivity,
    GLOBAL_SETTINGS_SECTIONS
} globalSettingsSection;

#define MIDI_SETTING_AFTERTOUCH_TYPE                aftertouchChannel
#define MIDI_SETTING_RUNNING_STATUS                 0
#define MIDI_SETTING_NOTE_OFF_TYPE                  1 //noteOffType_standardNoteOff
#define MIDI_SETTING_TRANSPORT_CC                   0x00

typedef enum
{
    MIDI_SETTING_AFTERTOUCH_TYPE_ID,
    MIDI_SETTING_RUNNING_STATUS_ID,
    MIDI_SETTING_NOTE_OFF_TYPE_ID,
    MIDI_SETTING_TRANSPORT_CC_ID,
    MIDI_SETTING_TRANSPORT_CC_PLAY_ID,
    MIDI_SETTING_TRANSPORT_CC_STOP_ID,
    MIDI_SETTING_TRANSPORT_CC_RECORD_ID,
    MIDI_SETTINGS
} midiSettings;

const uint8_t defaultMIDIsettingArray[MIDI_SETTINGS] =
{
    MIDI_SETTING_AFTERTOUCH_TYPE,
    MIDI_SETTING_RUNNING_STATUS,
    MIDI_SETTING_NOTE_OFF_TYPE,
    MIDI_SETTING_TRANSPORT_CC,
    0, //reserved
    0, //reserved
    0 //reserved
};

#define VELOCITY_SETTING_SENSITIVITY                0
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
