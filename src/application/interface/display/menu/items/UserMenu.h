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

typedef enum
{
    userMenuItem_pressureSettings,
    userMenuItem_pressureSensitivity,
    userMenuItem_pressureCurve,
    userMenuItem_midiSettings,
    userMenuItem_deviceInfo,
    userMenuItem_factoryReset,

    userMenuItem_pressureSensitivity_1,
    userMenuItem_pressureSensitivity_2,
    userMenuItem_pressureSensitivity_3,

    userMenuItem_pressureCurve_linear,
    userMenuItem_pressureCurve_log,
    userMenuItem_pressureCurve_exp,

    userMenuItem_midiSettings_aftertouchType,
    userMenuItem_midiSettings_runningStatus,
    userMenuItem_midiSettings_noteOff,
    userMenuItem_midiSettings_transport,
    userMenuItem_midiSettings_pitchBend,

    userMenuItem_midiSettings_aftertouchType_channel,
    userMenuItem_midiSettings_aftertouchType_poly,

    userMenuItem_midiSettings_noteOff_channel,
    userMenuItem_midiSettings_noteOff_zero_vel,

    userMenuItem_midiSettings_runningStatus_enabled,
    userMenuItem_midiSettings_runningStatus_disabled,

    userMenuItem_midiSettings_transport_mmc,
    userMenuItem_midiSettings_transport_cc,
    userMenuItem_midiSettings_transport_mmc_cc,

    userMenuItem_midiSettings_pitchBend_1,
    userMenuItem_midiSettings_pitchBend_2,

    USER_MENU_ITEMS
} userMenuItems_t;