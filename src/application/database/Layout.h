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
#include "io/pads/Pads.h"

namespace SectionPrivate
{
    enum class system_t : uint32_t
    {
        uid,
        AMOUNT
    };
}

namespace
{
    //not user accessible
    LESSDB::section_t systemSections[static_cast<uint32_t>(SectionPrivate::system_t::AMOUNT)] = {
        //uid section
        {
            .numberOfParameters     = 1,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },
    };

    LESSDB::section_t globalSections[static_cast<uint32_t>(Database::Section::global_t::AMOUNT)] = {
        //globalSettingsMIDI
        {
            .numberOfParameters     = static_cast<uint32_t>(Database::midiSettings_t::AMOUNT),
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 1,
            .autoIncrement          = false,
            .address                = 0,
        },

        //globalSettingsVelocity
        {
            .numberOfParameters     = static_cast<uint32_t>(Database::velocitySettings_t::AMOUNT),
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },
    };

    LESSDB::section_t programSections[static_cast<uint32_t>(Database::Section::program_t::AMOUNT)] = {
        //programLastActiveProgramSection
        {
            .numberOfParameters     = 1,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },

        //programLastActiveScaleSection
        {
            .numberOfParameters     = Database::NUMBER_OF_PROGRAMS,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },

        //programGlobalSettingsSection
        {
            .numberOfParameters     = static_cast<uint32_t>(Database::globalProgramSettings_t::AMOUNT) * Database::NUMBER_OF_PROGRAMS,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },

        //programLocalSettingsSection
        {
            .numberOfParameters     = static_cast<uint32_t>(Database::localProgramSettings_t::AMOUNT) * NUMBER_OF_PADS * Database::NUMBER_OF_PROGRAMS,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },
    };

    LESSDB::section_t scaleSections[static_cast<uint32_t>(Database::Section::scale_t::AMOUNT)] = {
        //scalePredefinedSection
        {
            .numberOfParameters     = static_cast<uint32_t>(Database::predefinedScaleParameters_t::AMOUNT) * static_cast<uint32_t>(Pads::scale_t::AMOUNT) * Database::NUMBER_OF_PROGRAMS,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        },

        //scaleUserSection
        {
            .numberOfParameters     = NUMBER_OF_PADS * Database::NOTES_PER_PAD_USER_SCALE * Database::NUMBER_OF_USER_SCALES,
            .parameterType          = LESSDB::sectionParameterType_t::byte,
            .preserveOnPartialReset = false,
            .defaultValue           = 0,
            .autoIncrement          = false,
            .address                = 0,
        }
    };

    LESSDB::section_t padCalibrationSections[static_cast<uint32_t>(Database::Section::padCalibration_t::AMOUNT)] = {
        //padCalibrationPressureUpperSection
        {
            .numberOfParameters     = NUMBER_OF_PADS,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = true,
            .defaultValue           = MAX_ADC_VALUE,
            .autoIncrement          = false,
            .address                = 0,
        },

        //padCalibrationXlowerSection
        {
            .numberOfParameters     = NUMBER_OF_PADS,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = true,
            .defaultValue           = 175,
            .autoIncrement          = false,
            .address                = 0,
        },

        //padCalibrationXupperSection
        {
            .numberOfParameters     = NUMBER_OF_PADS,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = true,
            .defaultValue           = 835,
            .autoIncrement          = false,
            .address                = 0,
        },

        //padCalibrationYlowerSection
        {
            .numberOfParameters     = NUMBER_OF_PADS,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = true,
            .defaultValue           = 175,
            .autoIncrement          = false,
            .address                = 0,
        },

        //padCalibrationYupperSection
        {
            .numberOfParameters     = NUMBER_OF_PADS,
            .parameterType          = LESSDB::sectionParameterType_t::word,
            .preserveOnPartialReset = true,
            .defaultValue           = 835,
            .autoIncrement          = false,
            .address                = 0,
        }
    };

    LESSDB::block_t dbLayout[static_cast<uint32_t>(Database::block_t::AMOUNT) + 1] = {
        //system block
        {
            .numberOfSections = static_cast<uint32_t>(SectionPrivate::system_t::AMOUNT),
            .section          = systemSections,
            .address          = 0,
        },

        //global block
        {
            .numberOfSections = static_cast<uint32_t>(Database::Section::global_t::AMOUNT),
            .section          = globalSections,
            .address          = 0,
        },

        //program block
        {
            .numberOfSections = static_cast<uint32_t>(Database::Section::program_t::AMOUNT),
            .section          = programSections,
            .address          = 0,
        },

        //scale block
        {
            .numberOfSections = static_cast<uint32_t>(Database::Section::scale_t::AMOUNT),
            .section          = scaleSections,
            .address          = 0,
        },

        //pad calibration block
        {
            .numberOfSections = static_cast<uint32_t>(Database::Section::padCalibration_t::AMOUNT),
            .section          = padCalibrationSections,
            .address          = 0,
        },
    };
}    // namespace