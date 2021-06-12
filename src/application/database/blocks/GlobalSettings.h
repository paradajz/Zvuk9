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

#include "../../interface/analog/pads/curves/DataTypes.h"
#include "../../interface/analog/pads/DataTypes.h"
#include "../../interface/digital/input/buttons/DataTypes.h"

/// \defgroup dbGlobalSettings Global settings
/// \ingroup database
/// @{

/// List of all sections in global settings database block.
typedef enum
{
    globalSettingsMIDI,
    globalSettingsVelocitySensitivity,
    GLOBAL_SETTINGS_SECTIONS
} dbSection_global_t;

/// Default MIDI aftertouch type.
#define MIDI_SETTING_AFTERTOUCH_TYPE aftertouchChannel

/// Default state of MIDI running status.
#define MIDI_SETTING_RUNNING_STATUS 0

/// Default MIDI note off type.
#define MIDI_SETTING_NOTE_OFF_TYPE noteOffType_standardNoteOff

/// Default MIDI transport message send type.
#define MIDI_SETTING_TRANSPORT_CC transportMMC

/// Default MIDI pitch bend type.
#define MIDI_SETTING_PITCH_BEND_TYPE pitchBend1

/// List of all elements in global MIDI database section.
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

/// Array of default (initial) values in global MIDI database section.
const uint8_t defaultMIDIsettingArray[MIDI_SETTINGS] = {
    MIDI_SETTING_AFTERTOUCH_TYPE,
    MIDI_SETTING_RUNNING_STATUS,
    MIDI_SETTING_NOTE_OFF_TYPE,
    MIDI_SETTING_TRANSPORT_CC,
    MIDI_SETTING_PITCH_BEND_TYPE,
    0,    //reserved
    0,    //reserved
    0     //reserved
};

/// Default velocity sensitivity setting.
#define VELOCITY_SETTING_SENSITIVITY velocity_soft

/// Default velocity curve.
#define VELOCITY_SETTING_CURVE curve_linear_up

/// List of all elements in global velocity database section.
typedef enum
{
    VELOCITY_SETTING_SENSITIVITY_ID,
    VELOCITY_SETTING_CURVE_ID,
    VELOCITY_SETTINGS
} velocitySettings;

/// Array of default (initial) values in global velocity database section.
const uint8_t defaultVelocitySettingsArray[VELOCITY_SETTINGS] = {
    VELOCITY_SETTING_SENSITIVITY,
    VELOCITY_SETTING_CURVE
};

/// @}
