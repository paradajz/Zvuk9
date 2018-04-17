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
#include "../interface/analog/pads/Pads.h"

///
/// \ingroup database
/// @{

///
/// \brief Database blocks.
/// @{

static dbSection_t programSections[PROGRAM_SECTIONS] =
{
    //programLastActiveProgramSection
    {
        .numberOfParameters = 1,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },

    //programLastActiveScaleSection
    {
        .numberOfParameters = NUMBER_OF_PROGRAMS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },

    //programGlobalSettingsSection
    {
        .numberOfParameters = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },

    //programLocalSettingsSection
    {
        .numberOfParameters = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*NUMBER_OF_PROGRAMS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },
};

static dbSection_t scaleSections[SCALE_SECTIONS] =
{
    //scalePredefinedSection
    {
        .numberOfParameters = PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*NUMBER_OF_PROGRAMS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },

    //scaleUserSection
    {
        .numberOfParameters = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    }
};

static dbSection_t padCalibrationSections[PAD_CALIBRATION_SECTIONS] =
{
    //padCalibrationPressureUpperSection
    {
        .numberOfParameters = NUMBER_OF_PADS,
        .parameterType = WORD_PARAMETER,
        .preserveOnPartialReset = true,
        .defaultValue = 500,
        .autoIncrement = false,
        .address = 0
    },

    //padCalibrationXlowerSection
    {
        .numberOfParameters = NUMBER_OF_PADS,
        .parameterType = WORD_PARAMETER,
        .preserveOnPartialReset = true,
        .defaultValue = 130,
        .autoIncrement = false,
        .address = 0
    },

    //padCalibrationXupperSection
    {
        .numberOfParameters = NUMBER_OF_PADS,
        .parameterType = WORD_PARAMETER,
        .preserveOnPartialReset = true,
        .defaultValue = 880,
        .autoIncrement = false,
        .address = 0
    },

    //padCalibrationYlowerSection
    {
        .numberOfParameters = NUMBER_OF_PADS,
        .parameterType = WORD_PARAMETER,
        .preserveOnPartialReset = true,
        .defaultValue = 130,
        .autoIncrement = false,
        .address = 0
    },

    //padCalibrationYupperSection
    {
        .numberOfParameters = NUMBER_OF_PADS,
        .parameterType = WORD_PARAMETER,
        .preserveOnPartialReset = true,
        .defaultValue = 880,
        .autoIncrement = false,
        .address = 0
    }
};

static dbSection_t globalSections[GLOBAL_SETTINGS_SECTIONS] =
{
    //globalSettingsMIDI
    {
        .numberOfParameters = MIDI_SETTINGS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 1,
        .autoIncrement = false,
        .address = 0
    },

    //globalSettingsVelocity
    {
        .numberOfParameters = VELOCITY_SETTINGS,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = false,
        .defaultValue = 0,
        .autoIncrement = false,
        .address = 0
    },
};

static dbSection_t idSections[1] =
{
    {
        .numberOfParameters = NUM_OF_UID_BYTES,
        .parameterType = BYTE_PARAMETER,
        .preserveOnPartialReset = 0,
        .defaultValue = UNIQUE_ID,
        .autoIncrement = false,
        .address = 0
    },
};

/// @}

///
/// \brief Database layout containing references to all blocks.
/// @{

dbBlock_t dbLayout[DB_BLOCKS] =
{
    //program block
    {
        .address = 0,
        .numberOfSections = PROGRAM_SECTIONS,
        .section = programSections,
    },

    //scale block
    {
        .address = 0,
        .numberOfSections = SCALE_SECTIONS,
        .section = scaleSections,
    },

    //pad calibration block
    {
        .address = 0,
        .numberOfSections = PAD_CALIBRATION_SECTIONS,
        .section = padCalibrationSections,
    },

    //global block
    {
        .address = 0,
        .numberOfSections = GLOBAL_SETTINGS_SECTIONS,
        .section = globalSections,
    },

    //id block
    {
        .address = 0,
        .numberOfSections = 1,
        .section = idSections,
    },
};

/// @}

/// @}