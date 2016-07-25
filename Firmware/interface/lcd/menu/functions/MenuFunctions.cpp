#include "MenuFunctions.h"

#ifdef MENU_FUNCTIONS_H_

bool factoryReset(functionArgument argument) {

    display.displayFactoryResetWarning();

    uint16_t padsPressed = 0;

    while(1)    {

        pads.update();

        for (int i=0; i<MAX_PADS; i++)
            if (pads.isPadPressed(i))   {

                if ((i != 6) && (i != 0) && (i != 8)) {

                    //flush all accumulated encoder output if there were any movements
                    encoders.flush();
                    return false;

                }

                if (!bitRead(padsPressed, i))   {

                    bitWrite(padsPressed, i, 1);

                    switch(i)   {

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

            }   else {

                if (bitRead(padsPressed, i))    {

                    bitWrite(padsPressed, i, 0);

                    switch(i)   {

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

        if (bitRead(padsPressed, 0) && bitRead(padsPressed, 6) && bitRead(padsPressed, 8))    {

            wait(500); //don't clear lcd immediately

            #ifdef MODULE_LEDS
                leds.setFadeSpeed(1);
                leds.allLEDsOff();
            #endif
            configuration.factoryReset((factoryResetType_t)argument.argument1);
            reboot();

        }

    }   return true;

}

bool deviceInfo(functionArgument argument)   {

    display.displayDeviceInfo();
    return true;

}

bool enableCalibration(functionArgument argument)     {

    switch(argument.argument1)    {

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

bool padEditMode(functionArgument argument)  {

    pads.setEditMode(true);
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    //check if last touched pad is pressed
    if (pads.isPadPressed(lastTouchedPad))   {

        #ifdef MODULE_LCD
            display.displayEditModeNotAllowed(padNotReleased);
        #endif
        pads.setEditMode(false);

    }   else {

        //normally, this is called in automatically in Pads.cpp
        //but on first occasion call it manually
        #if MODE_SERIAL > 0
            printf("Pad edit mode\n");
        #endif
        pads.setupPadEditMode(lastTouchedPad);

        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);

    }

    return true;

}

bool checkCalibration(functionArgument argument) {

    if (!pads.allPadsReleased())    {

        display.displayPadReleaseError(calibrationMode);
        menu.stopFunction();
        return false;

    }   return true;

}

bool checkPressureLevel(functionArgument argument)    {

    return true;

}

bool checkPressureCurve(functionArgument argument)    {

    return true;

}

bool checkAftertouchType(functionArgument argument)   {

    switch((aftertouchType_t)argument.argument1)    {

        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return false;

    }

    switch(argument.argument2)  {

        case true:
        //change option
        pads.setAftertouchType((aftertouchType_t)argument.argument1);
        return true;
        break;

        case false:
        //check if current aftertouch type is same as received argument
        return (pads.getAftertouchType() == (aftertouchType_t)argument.argument1);

    }   return false;

}

bool checkRunningStatus(functionArgument argument)  {

    switch(argument.argument2)  {

        case true:
        //switch option
        #if MODE_SERIAL < 1
        midi.setRunningStatus((bool)argument.argument1);
        #endif
        return true;

        case false:
        #if MODE_SERIAL < 1
        return (midi.runningStatusEnabled() == (bool)argument.argument1);
        #endif
        break;

    }   return false;

}

bool checkNoteOffStatus(functionArgument argument)    {

    switch(argument.argument1)  {

        case noteOffType_noteOnZeroVel:
        case noteOffType_standardNoteOff:
        //nothing
        break;

        default:
        return false;

    }

    switch(argument.argument2)  {

        case true:
        //switch option
        #if MODE_SERIAL < 1
        midi.setNoteOffStatus((noteOffType_t)argument.argument1);
        #endif
        return true;

        case false:
        #if MODE_SERIAL < 1
        return (midi.getNoteOffStatus() == (noteOffType_t)argument.argument1);
        #else
        return false;
        #endif

    }   return false;

}

#endif