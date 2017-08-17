#include "MenuFunctions.h"
#include "../../../leds/LEDs.h"
#include "../../../../database/Database.h"
#include "../../../../midi/src/DataTypes.h"

bool factoryReset(functionArgument argument)
{
    //clear title
    display.clearLine(0);
    display.displayFactoryResetConfirm();

    while (!display.update());

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
                    encoders.flush();
                    return false;
                }

                if (!bitRead(padsPressed, i))
                {
                    bitWrite(padsPressed, i, 1);

                    switch(i)
                    {
                        case 0:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_1, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 6:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_7, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        case 8:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_9, rowMap[LCD_ROW_FACTORY_RESET_PADS], 'x');
                        break;

                        default:
                        break;
                    }
                }
            }
            else
            {
                if (bitRead(padsPressed, i))
                {
                    bitWrite(padsPressed, i, 0);

                    switch(i)
                    {
                        case 0:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_1, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 6:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_7, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        case 8:
                        u8x8.drawGlyph(LCD_POSITION_FACTORY_RESET_PAD_9, rowMap[LCD_ROW_FACTORY_RESET_PADS], ' ');
                        break;

                        default:
                        break;
                    }
                }
            }
        }

        if (bitRead(padsPressed, 0) && bitRead(padsPressed, 6) && bitRead(padsPressed, 8))
        {
            wait_ms(1000);
            display.setDirectWriteState(true);
            u8x8.clear();
            leds.setFadeSpeed(2);
            display.displayFactoryResetStart();
            //now, turn all leds off
            leds.setAllOff();
            wait_ms(2000);
            database.factoryReset((initType_t)argument.argument1);
            u8x8.clear();
            display.displayFactoryResetEnd();
            wait_ms(2000);
            board.reboot(rebootApp);
        }
    }

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

bool checkCalibration(functionArgument argument)
{
    if (pads.getNumberOfPressedPads())
    {
        //release pads error
        menu.stopFunction();
        return false;
    }

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
        #ifdef NDEBUG
        (bool)argument.argument1 ? midi.enableRunningStatus() : midi.disableRunningStatus();
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID, argument.argument1);
        #endif
        return true;

        case false:
        #ifdef NDEBUG
        return (midi.runningStatusEnabled() == (bool)argument.argument1);
        #endif
        break;

    }

    return false;
}

bool checkNoteOffStatus(functionArgument argument)
{
    switch(argument.argument1)
    {
        case 0: //noteOffType_noteOnZeroVel:
        case 1: //noteOffType_standardNoteOff:
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
        #ifdef NDEBUG
        midi.setNoteOffMode((noteOffType_t)argument.argument1);
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID, argument.argument1);
        #endif
        return true;

        case false:
        #ifdef NDEBUG
        return (midi.getNoteOffMode() == (noteOffType_t)argument.argument1);
        #else
        return false;
        #endif
    }

    return false;
}

bool checkTransportCC(functionArgument argument)
{
    switch((transportControlType_t)argument.argument1)
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
        #ifdef NDEBUG
        buttons.setTransportControlType((transportControlType_t)argument.argument1);
        #endif
        return true;

        case false:
        #ifdef NDEBUG
        return (buttons.getTransportControlType() == (transportControlType_t)argument.argument1);
        #endif
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
