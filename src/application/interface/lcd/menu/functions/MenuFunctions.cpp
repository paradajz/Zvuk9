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

#include "MenuFunctions.h"
#include "../../../../database/Database.h"
#include "../../../analog/pads/Pads.h"
#include "../../../digital/input/encoders/Encoders.h"
#include "../../../digital/input/buttons/Buttons.h"
#include "../../../digital/output/leds/LEDs.h"

bool factoryReset(functionArgument argument)
{
    display.setDirectWriteState(true);
    display.clearAll();
    display.displayFactoryResetConfirm();

    uint16_t padsPressed = 0;

    while(1)
    {
        pads.update();

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            if (pads.isPadPressed(i))
            {
                 //hardcoding!!! :( pads 1, 7 and 9 need to be pressed to invoke factory reset
                if ((i != 6) && (i != 0) && (i != 8))
                {
                    //flush all accumulated encoder output if there were any movements
                    #warning to-do
                    // encoders.flush();
                    display.setDirectWriteState(false);
                    return false;
                }

                if (!BIT_READ(padsPressed, i))
                {
                    BIT_WRITE(padsPressed, i, 1);

                    switch(i)
                    {
                        case 0:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_1, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 6:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_7, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 8:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_9, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        default:
                        break;
                    }
                }
            }
            else
            {
                if (BIT_READ(padsPressed, i))
                {
                    BIT_WRITE(padsPressed, i, 0);

                    switch(i)
                    {
                        case 0:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_1, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 6:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_7, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 8:
                        display_hw.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_9, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        default:
                        break;
                    }
                }
            }
        }

        if (BIT_READ(padsPressed, 0) && BIT_READ(padsPressed, 6) && BIT_READ(padsPressed, 8))
        {
            wait_ms(1000);
            display.clearAll();
            leds.setFadeSpeed(2);
            display.displayFactoryResetStart();
            //now, turn all leds off
            leds.setAllOff();
            wait_ms(2000);
            database.factoryReset((initType_t)argument.argument1);
            display.clearAll();
            display.displayFactoryResetEnd();
            wait_ms(2000);
            board.reboot(rebootApp);
        }
    }

    display.setDirectWriteState(false);

    return true;
}

bool deviceInfo(functionArgument argument)
{
    display.displayDeviceInfo();
    return true;
}

bool enableCalibration(functionArgument argument)
{
    if (pads.getNumberOfPressedPads())
         return false;

    switch((padCoordinate_t)argument.argument1)
    {
        case coordinateX:
        case coordinateY:
        case coordinateZ:
        //this is fine
        break;

        default:
        return false;
    }

    pads.setCalibrationMode(true, (padCoordinate_t)argument.argument1);
    display.setupCalibrationScreen((padCoordinate_t)argument.argument1, leds.getLEDstate(LED_TRANSPORT_RECORD));
    return true;
}

bool checkAftertouchType(functionArgument argument)
{
    switch((aftertouchType_t)argument.argument1)
    {
        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        //change option
        pads.setAftertouchType((aftertouchType_t)argument.argument1);
        return true;
        break;

        case false:
        //check if current aftertouch type is same as received argument
        return (pads.getAftertouchType() == (aftertouchType_t)argument.argument1);
    }

    return false;
}

bool checkRunningStatus(functionArgument argument)
{
    switch(argument.argument2)
    {
        case true:
        //switch option
        midi.setRunningStatusState((bool)argument.argument1);
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID, argument.argument1);
        return true;

        case false:
        return (midi.getRunningStatusState() == (bool)argument.argument1);
        break;
    }

    return false;
}

bool checkNoteOffStatus(functionArgument argument)
{
    switch((noteOffType_t)argument.argument1)
    {
        case noteOffType_noteOnZeroVel:
        case noteOffType_standardNoteOff:
        //nothing
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        //switch option
        midi.setNoteOffMode((noteOffType_t)argument.argument1);
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID, argument.argument1);
        return true;

        case false:
        return (database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID) == (noteOffType_t)argument.argument1);
    }

    return false;
}

bool checkTransportCC(functionArgument argument)
{
    switch((transportControlMode_t)argument.argument1)
    {
        case transportCC:
        case transportMMC:
        case transportMMC_CC:
        //nothing
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        //switch option
        buttons.setTransportControlMode((transportControlMode_t)argument.argument1);
        return true;

        case false:
        return (buttons.getTransportControlMode() == (transportControlMode_t)argument.argument1);
        break;
    }

    return false;
}

bool checkPitchBendType(functionArgument argument)
{
    switch((pitchBendType_t)argument.argument1)
    {
        case pitchBend1:
        case pitchBend2:
        //nothing
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        //switch option
        pads.setPitchBendType((pitchBendType_t)argument.argument1);
        return true;

        case false:
        return (pads.getPitchBendType() == (pitchBendType_t)argument.argument1);
        break;

    }

    return false;
}

bool checkVelocitySensitivity(functionArgument argument)
{
    switch((velocitySensitivity_t)argument.argument1)
    {
        case velocity_soft:
        case velocity_medium:
        case velocity_hard:
        //ok
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        pads.setVelocitySensitivity((velocitySensitivity_t)argument.argument1);
        return true;
        break;

        case false:
        return (pads.getVelocitySensitivity() == (velocitySensitivity_t)argument.argument1);
        break;
    }

    return false;
}

bool checkPressureCurve(functionArgument argument)
{
    switch((curve_t)argument.argument1)
    {
        case curve_linear_up:
        case curve_log_up_1:
        case curve_exp_up:
        //ok
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        pads.setVelocityCurve((curve_t)argument.argument1);
        return true;
        break;

        case false:
        return (pads.getVelocityCurve() == (curve_t)argument.argument1);
        break;

    }

    return false;
}
