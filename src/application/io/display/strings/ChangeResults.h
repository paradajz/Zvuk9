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

/// Display strings used to indicate result of changing various parameters.
/// \ingroup displayStrings
/// @{

const char changeResult_noChange_string[] PROGMEM           = "no change in values";
const char changeResult_valueChanged_string[] PROGMEM       = "values changed";
const char changeResult_outOfRange_string[] PROGMEM         = "values out of range";
const char changeResult_notAllowed_string[] PROGMEM         = "not allowed";
const char changeResult_releasePads_string[] PROGMEM        = "pads not released";
const char changeResult_notUserScale_string[] PROGMEM       = "user scale not active";
const char changeResult_notPredefinedScale_string[] PROGMEM = "predefined scale not active";

const char settinType_singlePad_string[] PROGMEM = "Pad ";
const char settinType_allPads_string[] PROGMEM   = "All pads";

PGM_P const changeResultArray[] PROGMEM = {
    changeResult_noChange_string,
    changeResult_valueChanged_string,
    changeResult_outOfRange_string,
    changeResult_notAllowed_string,
    changeResult_releasePads_string,
    changeResult_notUserScale_string,
    changeResult_notPredefinedScale_string
};

/// @}