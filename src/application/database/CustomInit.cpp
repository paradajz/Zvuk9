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

#include "Database.h"
#include "Defaults.h"

namespace
{
    const uint8_t defaultMIDIsettingArray[static_cast<uint32_t>(Database::midiSettings_t::AMOUNT)] = {
        DB_DEFAULT_MIDI_SETTING_AFTERTOUCH_TYPE,
        DB_DEFAULT_MIDI_SETTING_RUNNING_STATUS,
        DB_DEFAULT_MIDI_SETTING_NOTE_OFF_TYPE,
        DB_DEFAULT_MIDI_SETTING_TRANSPORT_CC,
        DB_DEFAULT_MIDI_SETTING_PITCH_BEND_TYPE,
        0,    //reserved
        0,    //reserved
        0     //reserved
    };

    const uint8_t defaultVelocitySettingsArray[static_cast<uint32_t>(Database::velocitySettings_t::AMOUNT)] = {
        DB_DEFAULT_VELOCITY_SETTING_SENSITIVITY,
        DB_DEFAULT_VELOCITY_SETTING_CURVE
    };

    const uint8_t defaultGlobalProgramSettingArray[static_cast<uint32_t>(Database::globalProgramSettings_t::AMOUNT)] = {
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_SPLIT_STATE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_NOTE_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_CC_X,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_CC_Y,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_MIN,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_MAX,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_MIN,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_MAX,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN,
        DB_DEFAULT_GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN
    };

    const uint8_t defaultLocalProgramSettingArray[static_cast<uint32_t>(Database::localProgramSettings_t::AMOUNT)] = {
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_NOTE_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_MIDI_CHANNEL,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_CC_X,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_CC_Y,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_MIN,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_MAX,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_MIN,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_MAX,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_X_CURVE_GAIN,
        DB_DEFAULT_LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN
    };

    const uint8_t defaultPredefinedScaleParametersArray[static_cast<uint32_t>(Database::predefinedScaleParameters_t::AMOUNT)] = {
        DB_DEFAULT_OCTAVE,
        DB_DEFAULT_PREDEFINED_SCALE_TONIC,
        DB_DEFAULT_PREDEFINED_SCALE_SHIFT
    };
}    // namespace

void Database::writeDefaults()
{
    writeGlobalSettings();
    writeProgramSettings();
    writeScales();
}

void Database::writeGlobalSettings()
{
    for (int i = 0; i < static_cast<int>(Database::midiSettings_t::AMOUNT); i++)
        update(Section::global_t::midi, i, defaultMIDIsettingArray[i]);

    for (int i = 0; i < static_cast<int>(Database::velocitySettings_t::AMOUNT); i++)
        update(Section::global_t::velocitySensitivity, i, defaultVelocitySettingsArray[i]);
}

void Database::writeProgramSettings()
{
    //programLastActiveProgramSection only has 1 parameter - index 0
    update(Section::program_t::lastActiveProgram, 0, DB_DEFAULT_ACTIVE_PROGRAM);

    for (int i = 0; i < NUMBER_OF_PROGRAMS; i++)
        update(Section::program_t::lastActiveScale, i, DB_DEFAULT_ACTIVE_SCALE);

    for (int i = 0; i < NUMBER_OF_PROGRAMS; i++)
    {
        for (int j = 0; j < static_cast<int>(Database::globalProgramSettings_t::AMOUNT); j++)
            update(Section::program_t::globalSettings, j + (static_cast<int>(Database::globalProgramSettings_t::AMOUNT) * i), defaultGlobalProgramSettingArray[j]);
    }

    for (int i = 0; i < NUMBER_OF_PROGRAMS; i++)
    {
        for (int j = 0; j < NUMBER_OF_PADS; j++)
        {
            for (int k = 0; k < static_cast<int>(Database::localProgramSettings_t::AMOUNT); k++)
            {
                update(Section::program_t::localSettings, (static_cast<int>(Database::localProgramSettings_t::AMOUNT) * j + k) + (static_cast<int>(Database::localProgramSettings_t::AMOUNT) * NUMBER_OF_PADS * i), defaultLocalProgramSettingArray[k]);
            }
        }
    }
}

void Database::writeScales()
{
    for (int i = 0; i < NUMBER_OF_PROGRAMS; i++)
    {
        for (int j = 0; j < static_cast<int>(Pads::scale_t::AMOUNT); j++)
        {
            for (int k = 0; k < static_cast<int>(Database::predefinedScaleParameters_t::AMOUNT); k++)
            {
                update(Section::scale_t::predefined, (static_cast<int>(Database::predefinedScaleParameters_t::AMOUNT) * j + k) + (static_cast<int>(Database::predefinedScaleParameters_t::AMOUNT) * static_cast<int>(Pads::scale_t::AMOUNT) * i), defaultPredefinedScaleParametersArray[k]);
            }
        }
    }

    //init user scale settings
    for (int i = 0; i < NUMBER_OF_USER_SCALES; i++)
    {
        for (int j = 0; j < NUMBER_OF_PADS; j++)
        {
            //all blank notes
            for (int k = 0; k < NOTES_PER_PAD_USER_SCALE; k++)
                update(Section::scale_t::user, (NOTES_PER_PAD_USER_SCALE * j + k) + (NOTES_PER_PAD_USER_SCALE * NUMBER_OF_PADS * i), BLANK_NOTE);
        }
    }
}