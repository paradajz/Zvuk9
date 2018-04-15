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

#include "Functions.h"
#include "../../../../database/Database.h"
#include "../../../analog/pads/Pads.h"
#include "../../../digital/input/encoders/Encoders.h"
#include "../../../digital/input/buttons/Buttons.h"
#include "../../../digital/output/leds/LEDs.h"

///
/// \brief Used to perform factory reset.
/// Before factory reset is performed, certain pads need to be pressed
/// for confirmation.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool factoryReset(uint8_t argument)
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
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_1, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 6:
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_7, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 8:
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_9, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], 'x');
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
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_1, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 6:
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_7, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 8:
                        display_hw.drawGlyph(DISPLAY_POSITION_FACTORY_RESET_PAD_9, rowMap[DISPLAY_ROW_FACTORY_RESET_PADS], ' ');
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
            display.displayFactoryResetStart();
            //now, turn all leds off
            leds.setAllOff();
            wait_ms(2000);
            database.factoryReset((initType_t)argument);
            display.clearAll();
            display.displayFactoryResetEnd();
            wait_ms(2000);
            board.reboot();
        }
    }

    display.setDirectWriteState(false);

    return true;
}

///
/// \brief Used to display software and hardware info.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool deviceInfo(uint8_t argument)
{
    display.displayDeviceInfo();
    return true;
}

///
/// \brief Used to initiate setting up of calibration mode.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool enableCalibration(uint8_t argument)
{
    if (pads.getNumberOfPressedPads())
         return false;

    switch((padCoordinate_t)argument)
    {
        case coordinateX:
        case coordinateY:
        case coordinateZ:
        //this is fine
        break;

        default:
        return false;
    }

    pads.setCalibrationMode(true, (padCoordinate_t)argument);
    display.setupCalibrationScreen((padCoordinate_t)argument, leds.getLEDstate(LED_TRANSPORT_RECORD));
    return true;
}

///
/// \brief Used to either check or change aftertouch type.
/// When itemFuncChangeVal is set to false, aftertouch type
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, aftertouch type is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkAftertouchType(uint8_t argument)
{
    switch((aftertouchType_t)argument)
    {
        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return false;
    }

    switch(itemFuncChangeVal)
    {
        case true:
        //change option
        pads.setAftertouchType((aftertouchType_t)argument);
        return true;
        break;

        case false:
        //check if current aftertouch type is same as received argument
        return (pads.getAftertouchType() == (aftertouchType_t)argument);
    }

    return false;
}

///
/// \brief Used to either check or change running status.
/// When itemFuncChangeVal is set to false, running status
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, running status is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkRunningStatus(uint8_t argument)
{
    switch(itemFuncChangeVal)
    {
        case true:
        //switch option
        midi.setRunningStatusState((bool)argument);
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID, argument);
        return true;

        case false:
        return (midi.getRunningStatusState() == (bool)argument);
        break;
    }

    return false;
}

///
/// \brief Used to either check or change MIDI note off type.
/// When itemFuncChangeVal is set to false, MIDI note off type
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, MIDI note off type is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkNoteOffStatus(uint8_t argument)
{
    switch((noteOffType_t)argument)
    {
        case noteOffType_noteOnZeroVel:
        case noteOffType_standardNoteOff:
        //nothing
        break;

        default:
        //invalid argument
        return false;
    }

    switch(itemFuncChangeVal)
    {
        case true:
        //switch option
        midi.setNoteOffMode((noteOffType_t)argument);
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID, argument);
        return true;

        case false:
        return (database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID) == (noteOffType_t)argument);
    }

    return false;
}

///
/// \brief Used to either check or change type of transport controls.
/// When itemFuncChangeVal is set to false, type of transport controls
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, type of transport controls is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkTransportCC(uint8_t argument)
{
    switch((transportControlMode_t)argument)
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

    switch(itemFuncChangeVal)
    {
        case true:
        //switch option
        buttons.setTransportControlMode((transportControlMode_t)argument);
        return true;

        case false:
        return (buttons.getTransportControlMode() == (transportControlMode_t)argument);
        break;
    }

    return false;
}

///
/// \brief Used to either check or change pitch bend type.
/// When itemFuncChangeVal is set to false, pitch bend type
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, pitch bend type is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkPitchBendType(uint8_t argument)
{
    switch((pitchBendType_t)argument)
    {
        case pitchBend1:
        case pitchBend2:
        //nothing
        break;

        default:
        //invalid argument
        return false;
    }

    switch(itemFuncChangeVal)
    {
        case true:
        //switch option
        pads.setPitchBendType((pitchBendType_t)argument);
        return true;

        case false:
        return (pads.getPitchBendType() == (pitchBendType_t)argument);
        break;

    }

    return false;
}

///
/// \brief Used to either check or change velocity sensitivity.
/// When itemFuncChangeVal is set to false, velocity sensitivity
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, velocity sensitivity is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkVelocitySensitivity(uint8_t argument)
{
    switch((velocitySensitivity_t)argument)
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

    switch(itemFuncChangeVal)
    {
        case true:
        pads.setVelocitySensitivity((velocitySensitivity_t)argument);
        return true;
        break;

        case false:
        return (pads.getVelocitySensitivity() == (velocitySensitivity_t)argument);
        break;
    }

    return false;
}

///
/// \brief Used to either check or change pressure curve.
/// When itemFuncChangeVal is set to false, pressure curve
/// is compared to function argument.
/// When itemFuncChangeVal is set to true, pressure curve is
/// changed to function argument value.
/// @param [in] argument    Function argument defined in menu layout.
/// \returns                True on success, false otherwise.
///
bool checkPressureCurve(uint8_t argument)
{
    switch((curve_t)argument)
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

    switch(itemFuncChangeVal)
    {
        case true:
        pads.setVelocityCurve((curve_t)argument);
        return true;
        break;

        case false:
        return (pads.getVelocityCurve() == (curve_t)argument);
        break;

    }

    return false;
}
