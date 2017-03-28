#include "MenuFunctions.h"
#include "../../../leds/LEDs.h"
#include "../../../../database/Database.h"
#include "../../../../midi/DataTypes.h"

bool factoryReset(functionArgument argument)
{
    display.displayFactoryResetWarning();

    uint16_t padsPressed = 0;

    while(1)
    {
        pads.update();

        for (int i=0; i<MAX_PADS; i++)
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
                        //case 0:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_1_LOCATION, 2);
                        //lcd_putc('x');
                        //break;
//
                        //case 6:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_7_LOCATION, 2);
                        //lcd_putc('x');
                        //break;
//
                        //case 8:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_9_LOCATION, 2);
                        //lcd_putc('x');
                        //break;

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
                        //case 0:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_1_LOCATION, 2);
                        //lcd_putc(' ');
                        //break;
//
                        //case 6:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_7_LOCATION, 2);
                        //lcd_putc(' ');
                        //break;
//
                        //case 8:
                        //lcd_gotoxy(FACTORY_RESET_STRING_PAD_9_LOCATION, 2);
                        //lcd_putc(' ');
                        //break;

                        default:
                        break;
                    }
                }
            }
        }

        if (bitRead(padsPressed, 0) && bitRead(padsPressed, 6) && bitRead(padsPressed, 8))
        {
            leds.setFadeSpeed(1);
            //first, set all full leds to dim state
            for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
            {
                if (leds.getLEDstate(i) == ledStateFull)
                    leds.setLEDstate(i, ledStateDim);
            }
            wait_ms(1500);
            //now, turn all leds off
            leds.setAllOff();
            wait_ms(1000);
            db.factoryReset((factoryResetType_t)argument.argument1);
            reboot();
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
    switch(argument.argument1)
    {
        case coordinateX:
        case coordinateY:
        case coordinateZ:
        //nothing
        break;

        default:
        //wrong type
        return false;
    }

    pads.setCalibrationMode(true, (padCoordinate_t)argument.argument1);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;
}

bool checkCalibration(functionArgument argument)
{
    if (!pads.allPadsReleased())
    {
        display.displayPadReleaseError(calibrationMode);
        menu.stopFunction();
        return false;
    }

    return true;
}

bool checkPressureLevel(functionArgument argument)
{
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
        db.update(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID, argument.argument1);
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
        #ifdef NDEBUG
        midi.setNoteOffMode((noteOffType_t)argument.argument1);
        db.update(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID, argument.argument1);
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
        db.update(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID, argument.argument1);
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

bool checkPressureSensitivity(functionArgument argument)
{
    switch((pressureSensitivity_t)argument.argument1)
    {
        case pressure_soft:
        case pressure_medium:
        case pressure_hard:
        //ok
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        pads.setPressureSensitivity((pressureSensitivity_t)argument.argument1);
        return true;
        break;

        case false:
        return (pads.getPressureSensitivity() == (pressureSensitivity_t)argument.argument1);
        break;
    }

    return false;
}

bool checkPressureCurve(functionArgument argument)
{
    switch((curve_t)argument.argument1)
    {
        case curveLinear:
        case curveLog:
        case curveExp:
        //ok
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        pads.setPressureCurve((curve_t)argument.argument1);
        return true;
        break;

        case false:
        return (pads.getPressureCurve() == (curve_t)argument.argument1);
        break;

    }

    return false;
}
