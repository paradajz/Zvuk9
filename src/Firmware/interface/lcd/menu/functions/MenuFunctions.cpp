#include "MenuFunctions.h"
#include "../../../leds/LEDs.h"
#include "../../../../eeprom/Configuration.h"
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
                        case 0:
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_1_LOCATION, 2);
                        lcd_putc('x');
                        break;

                        case 6:
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_7_LOCATION, 2);
                        lcd_putc('x');
                        break;

                        case 8:
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_9_LOCATION, 2);
                        lcd_putc('x');
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
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_1_LOCATION, 2);
                        lcd_putc(' ');
                        break;

                        case 6:
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_7_LOCATION, 2);
                        lcd_putc(' ');
                        break;

                        case 8:
                        lcd_gotoxy(FACTORY_RESET_STRING_PAD_9_LOCATION, 2);
                        lcd_putc(' ');
                        break;

                        default:
                        break;
                    }
                }
            }
        }

        if (bitRead(padsPressed, 0) && bitRead(padsPressed, 6) && bitRead(padsPressed, 8))
        {
            wait_ms(500); //don't clear lcd immediately
            leds.setFadeSpeed(1);
            leds.allLEDsOff();
            wait_ms(2000);
            configuration.factoryReset((factoryResetType_t)argument.argument1);
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

    pads.setCalibrationMode(true, (coordinateType_t)argument.argument1);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;
}

bool padEditMode(functionArgument argument)
{
    pads.setEditMode(true);
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    //check if last touched pad is pressed
    if (pads.isPadPressed(lastTouchedPad))
    {
        display.displayEditModeNotAllowed(padNotReleased);
        pads.setEditMode(false);
    }
    else
    {
        //normally, this is called in automatically in Pads.cpp
        //but on first occasion call it manually
        #ifdef DEBUG
        printf("Pad edit mode\n");
        #endif
        pads.setupPadEditMode(lastTouchedPad);
        leds.setLEDstate(LED_OCTAVE_DOWN, ledStateFull);
        leds.setLEDstate(LED_OCTAVE_UP, ledStateFull);
    }

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
        configuration.writeParameter(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID, argument.argument1);
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
        configuration.writeParameter(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID, argument.argument1);
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
    switch((curveType_t)argument.argument1)
    {
        case curveTypeLinear:
        case curveTypeLog:
        case curveTypeExp:
        //ok
        break;

        default:
        //invalid argument
        return false;
    }

    switch(argument.argument2)
    {
        case true:
        pads.setPressureCurve((curveType_t)argument.argument1);
        return true;
        break;

        case false:
        return (pads.getPressureCurve() == (curveType_t)argument.argument1);
        break;

    }

    return false;
}
