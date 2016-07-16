#include "MenuFunctions.h"

#ifdef MENU_FUNCTIONS_H_

bool factoryReset(functionArgument argument) {

    #ifdef MODULE_LEDS
        leds.setFadeSpeed(1);
        leds.allLEDsOff();
    #endif
    configuration.factoryReset((factoryResetType_t)argument.argument1);
    reboot();
    return true;

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

    //check if last touched pad is pressed
    if (pads.isPadPressed(pads.getLastTouchedPad()))   {

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
        pads.setupPadEditMode(pads.getLastTouchedPad());

        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);

    }

    return true;

}

bool changeOctaveButtons(functionArgument argument)   {

    switch((shiftMode_t)argument.argument1)    {

        case shiftMode_note:
        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim);
        break;


        case shiftMode_octave:
        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);
        break;

        case shiftMode_channel:
        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);
        break;

        default:
        return false;

    }

    pads.setNoteShiftMode((shiftMode_t)argument.argument1);
    display.displayShiftMode((shiftMode_t)argument.argument1);

    menu.exitMenu();
    return true;


}

bool checkCalibration(functionArgument argument) {

    if (!pads.allPadsReleased())    {

        display.displayPadReleaseError(calibrationMode);
        menu.stopFunction();
        return false;

    }   return true;

}

bool checkRunningStatus(functionArgument argument)    {

    return true;

}

bool checkPressureLevel(functionArgument argument)    {

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

        default:
        return false;

    }

}

bool checkPressureCurve(functionArgument argument)    {

    return true;

}

bool checkNoteOffStatus(functionArgument argument)    {

    return true;

}

#endif