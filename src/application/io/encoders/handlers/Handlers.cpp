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

#include "Handlers.h"
#include "../../../../analog/pads/Pads.h"
#include "../../../../digital/input/buttons/Buttons.h"
#include "../../../../digital/input/encoders/Encoders.h"
#include "../../../../display/menu/Menu.h"
#include "pins/map/Encoders.h"
#include "pins/map/Buttons.h"

/// Handler for program encoder.
/// @param [in] id      Encoder index.
/// @param [in] steps   Amount of steps to move.
///                     Negative number represents inverted direction.
void handleProgram(uint8_t id, int8_t steps)
{
    if (pads.getEditModeState())
        return;

    if (menu.isMenuDisplayed())
    {
        menu.changeOption(steps > 0);
        return;
    }

    //allow only 1 step change
    steps = steps > 0 ? 1 : -1;

    int8_t newProgram = pads.getProgram() + steps;

    //rollover
    if (newProgram == NUMBER_OF_PROGRAMS)
        newProgram = 0;
    else if (newProgram < 0)
        newProgram = (NUMBER_OF_PROGRAMS - 1);

    changeResult_t result = pads.setProgram(newProgram);

    if (result == valueChanged)
    {
        //display scale on display
        display.displayProgramInfo(pads.getProgram() + 1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());

        //scale is changed as well
        pads.setActiveNoteLEDs(false, 0);
    }
    else if (result != noChange)
    {
        display.displayError(functionProgram, result);
    }
}

/// Handler for scale encoder.
/// @param [in] id      Encoder index.
/// @param [in] steps   Amount of steps to move.
///                     Negative number represents inverted direction.
void handleScale(uint8_t id, int8_t steps)
{
    if (pads.getEditModeState())
        return;

    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    //allow only 1 step change
    steps = steps > 0 ? 1 : -1;

    if (encoders.getMIDIchannelPresetEncMode())
    {
        //change midi channel
        buttons.setButtonEnableState(BUTTON_ON_OFF_SPLIT, false);

        int8_t newMIDIchannel = pads.getMIDIchannel(lastTouchedPad) + steps;

        //rollover
        if (newMIDIchannel < 1)
            newMIDIchannel = 16;
        else if (newMIDIchannel > 16)
            newMIDIchannel = 1;

        changeResult_t result = pads.setMIDIchannel(lastTouchedPad, newMIDIchannel);

        if (result == valueChanged)
        {
            display.displayChangeResult(functionChannel, pads.getMIDIchannel(pads.getLastTouchedPad()), pads.getSplitState() ? singlePadSetting : globalSetting);
        }
        else if (result != noChange)
        {
            display.displayError(functionChannel, result);
        }
    }
    else
    {
        int8_t newScale = pads.getScale() + steps;

        //rollover
        if (newScale == (PREDEFINED_SCALES + NUMBER_OF_USER_SCALES))
            newScale = 0;
        else if (newScale < 0)
            newScale = (PREDEFINED_SCALES + NUMBER_OF_USER_SCALES - 1);

        changeResult_t result = pads.setScale(newScale);

        if (result == valueChanged)
        {
            pads.setActiveNoteLEDs(false, 0);
            //display scale on display
            display.displayProgramInfo(pads.getProgram() + 1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
        }
        else if (result != noChange)
        {
            display.displayError(functionScale, result);
        }
    }
}

/// Handler for CC encoders.
/// @param [in] id      Encoder index.
/// @param [in] steps   Amount of steps to move.
///                     Negative number represents inverted direction.
void handleCC(uint8_t id, int8_t steps)
{
    if (pads.getEditModeState())
        return;

    padCoordinate_t coordinate     = coordinateX;
    uint8_t         lastTouchedPad = pads.getLastTouchedPad();

    switch (id)
    {
    case X_CC_ENCODER:
        coordinate = coordinateX;
        break;

    case Y_CC_ENCODER:
        coordinate = coordinateY;
        break;
    }

    changeResult_t result = pads.setCCvalue(coordinate, pads.getCCvalue(lastTouchedPad, coordinate) + steps);

    switch (result)
    {
    case valueChanged:
    case noChange:
        if (pads.getPitchBendState(lastTouchedPad, coordinate))
            display.displayChangeResult(coordinate == coordinateX ? functionXPitchBend : functionYPitchBend, 0, pads.getSplitState() ? singlePadSetting : allPadsSetting);
        else
            display.displayChangeResult(coordinate == coordinateX ? functionXCC : functionYCC, pads.getCCvalue(lastTouchedPad, coordinate), pads.getSplitState() ? singlePadSetting : allPadsSetting);
        break;

    default:
        display.displayError(coordinate == coordinateX ? functionXCC : functionYCC, result);
        break;
    }
}

/// Handler for limit encoders (X/Y min/max).
/// @param [in] id      Encoder index.
/// @param [in] steps   Amount of steps to move.
///                     Negative number represents inverted direction.
void handleLimit(uint8_t id, int8_t steps)
{
    if (pads.getEditModeState())
        return;

    if (menu.isMenuDisplayed() && !pads.getNumberOfPressedPads())
        return;

    if (!pads.isCalibrationEnabled() && menu.isMenuDisplayed())
        return;

    padCoordinate_t coordinate     = coordinateX;
    limitType_t     limit          = limitTypeMin;
    uint8_t         lastTouchedPad = pads.getLastTouchedPad();
    function_t      function;

    switch (id)
    {
    case X_MIN_ENCODER:
        coordinate = coordinateX;
        function   = functionXMinLimit;
        limit      = limitTypeMin;

        if (pads.isCalibrationEnabled() && (pads.getCalibrationMode() == coordinateX))
        {
            steps = steps > 0 ? -1 : 1;
            pads.calibrateXY(lastTouchedPad, coordinateX, limitTypeMin, pads.getCalibrationLimit(lastTouchedPad, coordinateX, limitTypeMin) + steps, true);
        }
        break;

    case X_MAX_ENCODER:
        coordinate = coordinateX;
        limit      = limitTypeMax;
        function   = functionXMaxLimit;

        if (pads.isCalibrationEnabled() && (pads.getCalibrationMode() == coordinateX))
        {
            steps = steps > 0 ? -1 : 1;
            pads.calibrateXY(lastTouchedPad, coordinateX, limitTypeMax, pads.getCalibrationLimit(lastTouchedPad, coordinateX, limitTypeMax) + steps, true);
        }
        break;

    case Y_MIN_ENCODER:
        coordinate = coordinateY;
        limit      = limitTypeMin;
        function   = functionYMinLimit;

        if (pads.isCalibrationEnabled() && (pads.getCalibrationMode() == coordinateY))
        {
            steps = steps > 0 ? -1 : 1;
            pads.calibrateXY(lastTouchedPad, coordinateY, limitTypeMin, pads.getCalibrationLimit(lastTouchedPad, coordinateY, limitTypeMin) + steps, true);
        }
        break;

    case Y_MAX_ENCODER:
        coordinate = coordinateY;
        limit      = limitTypeMax;
        function   = functionYMaxLimit;

        if (pads.isCalibrationEnabled() && (pads.getCalibrationMode() == coordinateY))
        {
            steps = steps > 0 ? -1 : 1;
            pads.calibrateXY(lastTouchedPad, coordinateY, limitTypeMax, pads.getCalibrationLimit(lastTouchedPad, coordinateY, limitTypeMax) + steps, true);
        }
        break;

    default:
        return;
    }

    if (!pads.isCalibrationEnabled())
    {
        changeResult_t result = pads.setCClimit(coordinate, limit, pads.getCClimit(lastTouchedPad, coordinate, limit) + steps);

        switch (result)
        {
        case noChange:
        case valueChanged:
            display.displayChangeResult(function, pads.getCClimit(lastTouchedPad, coordinate, limit), pads.getSplitState() ? singlePadSetting : allPadsSetting);
            break;

        default:
            display.displayError(function, result);
            break;
        }
    }
}

/// Handler for curve encoders.
/// @param [in] id      Encoder index.
/// @param [in] steps   Amount of steps to move.
///                     Negative number represents inverted direction.
void handleCurve(uint8_t id, int8_t steps)
{
    if (pads.getEditModeState())
        return;

    padCoordinate_t coordinate     = coordinateX;
    uint8_t         lastTouchedPad = pads.getLastTouchedPad();

    switch (id)
    {
    case X_CURVE_ENCODER:
        coordinate = coordinateX;
        break;

    case Y_CURVE_ENCODER:
        coordinate = coordinateY;
        break;
    }

    //only one step change
    steps = steps > 0 ? 1 : -1;

    changeResult_t result = pads.setCCcurve(coordinate, pads.getCCcurve(lastTouchedPad, coordinate) + steps);

    switch (result)
    {
    case noChange:
    case valueChanged:
        display.displayChangeResult(coordinate == coordinateX ? functionXCurve : functionYCurve, pads.getCCcurve(lastTouchedPad, coordinate), pads.getSplitState() ? singlePadSetting : allPadsSetting);
        break;

    default:
        display.displayError(coordinate == coordinateX ? functionXCurve : functionYCurve, result);
        break;
    }
}
