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

#define NUMBER_OF_PROGRAMS                          15
#define DEFAULT_ACTIVE_PROGRAM                      0
#define DEFAULT_ACTIVE_SCALE                        0

typedef enum
{
    programLastActiveProgramSection,
    programLastActiveScaleSection,
    programGlobalSettingsSection,
    programLocalSettingsSection,
    PROGRAM_SECTIONS
} programSections;

//define default global program settings
#define GLOBAL_PROGRAM_SETTING_SPLIT_STATE          0x00
#define GLOBAL_PROGRAM_SETTING_X_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_Y_ENABLE             0x01
#define GLOBAL_PROGRAM_SETTING_NOTE_ENABLE          0x01
#define GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE    0x01
#define GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE  0
#define GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE  0
#define GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL         0x01
#define GLOBAL_PROGRAM_SETTING_CC_X                 0x14
#define GLOBAL_PROGRAM_SETTING_CC_Y                 0x15
#define GLOBAL_PROGRAM_SETTING_X_MIN                0
#define GLOBAL_PROGRAM_SETTING_X_MAX                127
#define GLOBAL_PROGRAM_SETTING_Y_MIN                0
#define GLOBAL_PROGRAM_SETTING_Y_MAX                127
#define GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN         curve_linear_up
#define GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN         curve_linear_up

typedef enum
{
    //list of IDs for global program settings for access
    GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID,
    GLOBAL_PROGRAM_SETTING_X_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID,
    GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
    GLOBAL_PROGRAM_SETTING_CC_X_ID,
    GLOBAL_PROGRAM_SETTING_CC_Y_ID,
    GLOBAL_PROGRAM_SETTING_X_MIN_ID,
    GLOBAL_PROGRAM_SETTING_X_MAX_ID,
    GLOBAL_PROGRAM_SETTING_Y_MIN_ID,
    GLOBAL_PROGRAM_SETTING_Y_MAX_ID,
    GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
    GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
    GLOBAL_PROGRAM_SETTINGS
} globalProgramSettings;

//put global settings in array for easier access
const uint8_t defaultGlobalProgramSettingArray[GLOBAL_PROGRAM_SETTINGS] =
{
    GLOBAL_PROGRAM_SETTING_SPLIT_STATE,
    GLOBAL_PROGRAM_SETTING_X_ENABLE,
    GLOBAL_PROGRAM_SETTING_Y_ENABLE,
    GLOBAL_PROGRAM_SETTING_NOTE_ENABLE,
    GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE,
    GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE,
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
#define LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE   0
#define LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE   0
#define LOCAL_PROGRAM_SETTING_MIDI_CHANNEL          0x01
#define LOCAL_PROGRAM_SETTING_CC_X                  0x14
#define LOCAL_PROGRAM_SETTING_CC_Y                  0x15
#define LOCAL_PROGRAM_SETTING_X_MIN                 0
#define LOCAL_PROGRAM_SETTING_X_MAX                 127
#define LOCAL_PROGRAM_SETTING_Y_MIN                 0
#define LOCAL_PROGRAM_SETTING_Y_MAX                 127
#define LOCAL_PROGRAM_SETTING_X_CURVE_GAIN          curve_linear_up
#define LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN          curve_linear_up

typedef enum
{
    LOCAL_PROGRAM_SETTING_X_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_Y_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID,
    LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
    LOCAL_PROGRAM_SETTING_CC_X_ID,
    LOCAL_PROGRAM_SETTING_CC_Y_ID,
    LOCAL_PROGRAM_SETTING_X_MIN_ID,
    LOCAL_PROGRAM_SETTING_X_MAX_ID,
    LOCAL_PROGRAM_SETTING_Y_MIN_ID,
    LOCAL_PROGRAM_SETTING_Y_MAX_ID,
    LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
    LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
    LOCAL_PROGRAM_SETTINGS
} localProgramSettings;

const uint8_t defaultLocalProgramSettingArray[] =
{
    LOCAL_PROGRAM_SETTING_X_ENABLE,
    LOCAL_PROGRAM_SETTING_Y_ENABLE,
    LOCAL_PROGRAM_SETTING_NOTE_ENABLE,
    LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
    LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE,
    LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE,
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
