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

#include "../interface/pads/Pads.h"

void Database::createLayout()
{
    DBMS::addBlocks(DB_BLOCKS);

    dbSection_t section;

    {
        //program block

        //programLastActiveProgramSection
        section.numberOfParameters = 1;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLastActiveScaleSection
        section.numberOfParameters = NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programGlobalSettingsSection
        section.numberOfParameters = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);

        //programLocalSettingsSection
        section.numberOfParameters = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_PROGRAM, section);
    }

    {
        //scale block

        //scalePredefinedSection
        section.numberOfParameters = PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);

        //scaleUserSection
        section.numberOfParameters = NUMBER_OF_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_SCALE, section);
    }

    {
        //pad calibration block

        //padCalibrationPressureUpperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = PAD_PRESSURE_UPPER_DEFAULT;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXlowerSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 1023;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationXupperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYlowerSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 1023;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);

        //padCalibrationYupperSection
        section.numberOfParameters = NUMBER_OF_PADS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = WORD_PARAMETER;
        section.preserveOnPartialReset = true;

        DBMS::addSection(DB_BLOCK_PAD_CALIBRATION, section);
    }

    {
        //global settings block

        //globalSettingsMIDI
        section.numberOfParameters = MIDI_SETTINGS;
        section.defaultValue = 1;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);

        //globalSettingsVelocity
        section.numberOfParameters = VELOCITY_SETTINGS;
        section.defaultValue = 0;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_GLOBAL_SETTINGS, section);
    }

    {
        //id block
        section.numberOfParameters = ID_OFFSET;
        section.defaultValue = UNIQUE_ID;
        section.autoIncrement = false;
        section.parameterType = BYTE_PARAMETER;
        section.preserveOnPartialReset = false;

        DBMS::addSection(DB_BLOCK_ID, section);
    }
}
