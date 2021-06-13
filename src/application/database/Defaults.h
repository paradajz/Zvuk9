#pragma once

#include "shared/Types.h"
#include "io/pads/Pads.h"
#include "io/buttons/Buttons.h"
#include "midi/src/MIDI.h"

#define DB_DEFAULT_ACTIVE_PROGRAM                             0
#define DB_DEFAULT_ACTIVE_SCALE                               0
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_SPLIT_STATE         0x00
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_ENABLE            0x01
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_ENABLE            0x01
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_NOTE_ENABLE         0x01
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE   0x01
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE 0
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE 0
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL        0x01
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_CC_X                0x14
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_CC_Y                0x15
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_MIN               0
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_MAX               127
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_MIN               0
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_MAX               127
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN        static_cast<uint8_t>(Pads::curve_t::linear_up)
#define DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN        static_cast<uint8_t>(Pads::curve_t::linear_up)
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_ENABLE             0x01
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE  0
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE  0
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_MIDI_CHANNEL         0x01
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_CC_X                 0x14
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_CC_Y                 0x15
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_MIN                0
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_MAX                127
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_MIN                0
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_MAX                127
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_CURVE_GAIN         static_cast<uint8_t>(Pads::curve_t::linear_up)
#define DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN         static_cast<uint8_t>(Pads::curve_t::linear_up)
#define DB_DEFAULT_OCTAVE                                     4
#define DB_DEFAULT_PREDEFINED_SCALE_TONIC                     static_cast<uint8_t>(MIDI::note_t::C)
#define DB_DEFAULT_PREDEFINED_SCALE_SHIFT                     0
#define DB_DEFAULT_MIDI_SETTING_AFTERTOUCH_TYPE               static_cast<uint8_t>(Pads::aftertouchType_t::channel)
#define DB_DEFAULT_MIDI_SETTING_RUNNING_STATUS                0
#define DB_DEFAULT_MIDI_SETTING_NOTE_OFF_TYPE                 static_cast<uint8_t>(MIDI::noteOffType_t::standardNoteOff)
#define DB_DEFAULT_MIDI_SETTING_TRANSPORT_CC                  static_cast<uint8_t>(IO::Buttons::transportControlMode_t::MMC)
#define DB_DEFAULT_MIDI_SETTING_PITCH_BEND_TYPE               static_cast<uint8_t>(Pads::pitchBendType_t::type1)
#define DB_DEFAULT_VELOCITY_SETTING_SENSITIVITY               static_cast<uint8_t>(Pads::velocitySensitivity_t::soft)
#define DB_DEFAULT_VELOCITY_SETTING_CURVE                     static_cast<uint8_t>(Pads::curve_t::linear_up)