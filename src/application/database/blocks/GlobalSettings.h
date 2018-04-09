/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#pragma once

#include "../../interface/pads/curves/Curves.h"
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
