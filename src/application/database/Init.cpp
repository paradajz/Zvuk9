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

void Database::writeCustomValues()
{
    //we need to init each block and section with data
    initProgramSettings();
    initScales();
    initGlobalSettings();
}

void Database::initProgramSettings()
{
    //init default active program
    //programLastActiveProgramSection only has 1 parameter - index 0
    database.update(DB_BLOCK_PROGRAM, programLastActiveProgramSection, 0, DEFAULT_ACTIVE_PROGRAM);

    //init default active scales for programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
        database.update(DB_BLOCK_PROGRAM, programLastActiveScaleSection, i, DEFAULT_ACTIVE_SCALE);

    //init global settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<GLOBAL_PROGRAM_SETTINGS; j++)
            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, j+(GLOBAL_PROGRAM_SETTINGS*i), defaultGlobalProgramSettingArray[j]);
    }

    //init local settings for all programs
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<NUMBER_OF_PADS; j++)
        {
            for (int k=0; k<LOCAL_PROGRAM_SETTINGS; k++)
            {
                database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*j+k) + (LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*i), defaultLocalProgramSettingArray[k]);
            }
        }
    }
}

void Database::initScales()
{
    //init predefined scale settings
    for (int i=0; i<NUMBER_OF_PROGRAMS; i++)
    {
        for (int j=0; j<PREDEFINED_SCALES; j++)
        {
            for (int k=0; k<PREDEFINED_SCALE_PARAMETERS; k++)
            {
                database.update(DB_BLOCK_SCALE, scalePredefinedSection, (PREDEFINED_SCALE_PARAMETERS*j+k) + (PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES*i), defaultPredefinedScaleParametersArray[k]);
            }
        }
    }

    //init user scale settings
    for (int i=0; i<NUMBER_OF_USER_SCALES; i++)
    {
        for (int j=0; j<NUMBER_OF_PADS; j++)
        {
            //all blank notes
            for (int k=0; k<NOTES_PER_PAD; k++)
                database.update(DB_BLOCK_SCALE, scaleUserSection, (NOTES_PER_PAD*j+k) + (NOTES_PER_PAD*NUMBER_OF_PADS*i), BLANK_NOTE);
        }
    }
}

void Database::initGlobalSettings()
{
    //init midi settings
    for (int i=0; i<MIDI_SETTINGS; i++)
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, i, defaultMIDIsettingArray[i]);

    //init pressure settings
    for (int i=0; i<VELOCITY_SETTINGS; i++)
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, i, defaultVelocitySettingsArray[i]);
}
