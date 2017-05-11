#pragma once

#include "../../interface/pads/curves/Curves.h"
#include "../../midi/src/DataTypes.h"

typedef enum
{
    globalSettingsMIDI,
    globalSettingsPressure,
    GLOBAL_SETTINGS_SECTIONS
} globalSettingsSection;

#define MIDI_SETTING_AFTERTOUCH_TYPE                aftertouchChannel
#define MIDI_SETTING_RUNNING_STATUS                 0
#define MIDI_SETTING_NOTE_OFF_TYPE                  noteOffType_standardNoteOff
#define MIDI_SETTING_TRANSPORT_CC                   0x00
#define MIDI_SETTING_TRANSPORT_CC_PLAY              125
#define MIDI_SETTING_TRANSPORT_CC_STOP              126
#define MIDI_SETTING_TRANSPORT_CC_RECORD            127

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
    MIDI_SETTING_TRANSPORT_CC_PLAY,
    MIDI_SETTING_TRANSPORT_CC_STOP,
    MIDI_SETTING_TRANSPORT_CC_RECORD
};

#define PRESSURE_SETTING_SENSITIVITY                0
#define PRESSURE_SETTING_CURVE                      curve_linear_up_1

typedef enum
{
    PRESSURE_SETTING_SENSITIVITY_ID,
    PRESSURE_SETTING_CURVE_ID,
    PRESSURE_SETTINGS
} pressureSettings;

const uint8_t defaultPressureSettingsArray[PRESSURE_SETTINGS] =
{
    PRESSURE_SETTING_SENSITIVITY,
    PRESSURE_SETTING_CURVE
};
