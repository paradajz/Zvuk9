#include "MenuFunctions.h"

#ifdef MENU_FUNCTIONS_H_

bool factoryReset(uint8_t type) {

    #ifdef MODULE_LEDS
        leds.setFadeSpeed(1);
        leds.allLEDsOff();
    #endif
    configuration.factoryReset((factoryResetType_t)type);
    reboot();
    return true;

}

bool deviceInfo(uint8_t arg)   {

    display.displayDeviceInfo();
    return true;

}

bool enableCalibration(uint8_t type)     {

    switch(type)    {

        case coordinateX:
        case coordinateY:
        case coordinateZ:
        //nothing
        break;

        default:
        //wrong type
        return false;

    }

    pads.setCalibrationMode(true, (coordinateType_t)type);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;

}

bool padEditMode(uint8_t arg)  {

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

bool changeOctaveButtons(uint8_t arg)   {

    switch((shiftMode_t)arg)    {

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

    pads.setNoteShiftMode((shiftMode_t)arg);
    display.displayShiftMode((shiftMode_t)arg);

    menu.exitMenu();
    return true;


}

bool checkCalibration(uint8_t arg) {

    if (!pads.allPadsReleased())    {

        display.displayPadReleaseError(calibrationMode);
        menu.stopFunction();
        return false;

    }   return true;

}

#endif