/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY, without even the implied warranty of
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

#include "dbms/src/DataTypes.h"
#include "functions/MenuFunctions.h"
#include "items/ServiceMenu.h"
#include "items/UserMenu.h"
#include "DataTypes.h"

///
/// \ingroup interfaceDisplay
/// @{

///
/// \brief User menu layout.
///
static menuItem_t userMenuLayout[USER_MENU_ITEMS] =
{
    {
        .stringPointer = menuOption_velocitySettings_string,
        .level = 1,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = menuOption_pressureSensitivity_string,
        .level = 11,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = pressure_sensitivity_1_string,
        .level = 111,
        .function = checkVelocitySensitivity,
        .argument =
        {
            .argument1 = (uint8_t)velocity_soft,
        .   argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = pressure_sensitivity_2_string,
        .level = 112,
        .function = checkVelocitySensitivity,
        .argument =
        {
            .argument1 = (uint8_t)velocity_medium,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = pressure_sensitivity_3_string,
        .level = 113,
        .function = checkVelocitySensitivity,
        .argument =
        {
            .argument1 = (uint8_t)velocity_hard,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = menuOption_pressureCurve_string,
        .level = 12,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = pressure_curve_linear_string,
        .level = 121,
        .function = checkPressureCurve,
        .argument =
        {
            .argument1 = (uint8_t)curve_linear_up,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = pressure_curve_log_string,
        .level = 122,
        .function = checkPressureCurve,
        .argument =
        {
            .argument1 = (uint8_t)curve_log_up_1,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = pressure_curve_inv_exp_string,
        .level = 123,
        .function = checkPressureCurve,
        .argument =
        {
            .argument1 = (uint8_t)curve_exp_up,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = menuOption_midiSettings_string,
        .level = 2,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = midiSettings_atouchType_string,
        .level = 21,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = aftertouchType_channel_string,
        .level = 211,
        .function = checkAftertouchType,
        .argument =
        {
            .argument1 = (uint8_t)aftertouchChannel,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = aftertouchType_poly_string,
        .level = 212,
        .function = checkAftertouchType,
        .argument =
        {
            .argument1 = (uint8_t)aftertouchPoly,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_runningStatus_string,
        .level = 22,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = enabled_string_string,
        .level = 221,
        .function = checkRunningStatus,
        .argument =
        {
            .argument1 = 1,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = disabled_string_string,
        .level = 222,
        .function = checkRunningStatus,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_noteOff_string,
        .level = 23,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = noteOff_offChannel_string,
        .level = 231,
        .function = checkNoteOffStatus,
        .argument =
        {
            .argument1 = noteOffType_standardNoteOff,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = noteOff_zeroVel_string,
        .level = 232,
        .function = checkNoteOffStatus,
        .argument =
        {
            .argument1 = noteOffType_noteOnZeroVel,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_transport_string,
        .level = 24,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = midiSettings_transport_MMC_string,
        .level = 241,
        .function = checkTransportCC,
        .argument =
        {
            .argument1 = transportMMC,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_transport_CC_string,
        .level = 242,
        .function = checkTransportCC,
        .argument =
        {
            .argument1 = transportCC,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_transport_MMC_CC_string,
        .level = 243,
        .function = checkTransportCC,
        .argument =
        {
            .argument1 = transportMMC_CC,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_pitchBend_string,
        .level = 25,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = midiSettings_pitchBend_1_string,
        .level = 251,
        .function = checkPitchBendType,
        .argument =
        {
            .argument1 = pitchBend1,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = midiSettings_pitchBend_2_string,
        .level = 252,
        .function = checkPitchBendType,
        .argument =
        {
            .argument1 = pitchBend2,
            .argument2 = 0,
        },
        .checkable = true,
    },

    {
        .stringPointer = menuOption_deviceInfo_string,
        .level = 3,
        .function = deviceInfo,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = menuOption_factoryReset_string,
        .level = 4,
        .function = factoryReset,
        .argument =
        {
            .argument1 = (uint8_t)initPartial,
            .argument2 = 0,
        },
        .checkable = false,
    }
};

///
/// \brief Service menu layout.
///
static menuItem_t serviceMenuLayout[USER_MENU_ITEMS] =
{
    {
        .stringPointer = menuOption_padCalibration_string,
        .level = 1,
        .function = NULL,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = calibration_x_string,
        .level = 11,
        .function = enableCalibration,
        .argument =
        {
            .argument1 = (uint8_t)coordinateX,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = calibration_y_string,
        .level = 12,
        .function = enableCalibration,
        .argument =
        {
            .argument1 = (uint8_t)coordinateY,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = calibration_pressure_string,
        .level = 13,
        .function = enableCalibration,
        .argument =
        {
            .argument1 = (uint8_t)coordinateZ,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = menuOption_deviceInfo_string,
        .level = 2,
        .function = deviceInfo,
        .argument =
        {
            .argument1 = 0,
            .argument2 = 0,
        },
        .checkable = false,
    },

    {
        .stringPointer = menuOption_factoryReset_string,
        .level = 3,
        .function = factoryReset,
        .argument =
        {
            .argument1 = (uint8_t)initFull,
            .argument2 = 0,
        },
        .checkable = false,
    }
};

/// @}