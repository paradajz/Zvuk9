#include "MenuFunctions.h"

#ifdef MENU_FUNCTIONS_H_

void factory_reset(factoryResetType_t type) {

    #ifdef MODULE_LEDS
    leds.setFadeSpeed(1);
    leds.allLEDsOff();
    #endif
    configuration.factoryReset(type);
    reboot();

}

bool deviceInfo()   {

    display.displayDeviceInfo();
    return true;

}

bool partialReset() {

    factory_reset(factoryReset_partial);
    return true;

}

bool fullReset()    {

    factory_reset(factoryReset_restore);
    return true;

}

bool enableCalibration_x()    {

    pads.setCalibrationMode(true, coordinateX);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;

}

bool enableCalibration_y()    {

    pads.setCalibrationMode(true, coordinateY);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;

}

bool enableCalibration_z()  {

    pads.setCalibrationMode(true, coordinateZ);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);
    return true;

}

bool padEditMode()  {

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

bool octaveShiftMode()  {

    pads.setNoteShiftMode(shiftMode_octave);
    display.displayShiftMode(shiftMode_octave);

    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);

    menu.exitMenu();
    return true;

}

bool noteShiftMode()    {

    pads.setNoteShiftMode(shiftMode_note);
    display.displayShiftMode(shiftMode_note);

    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim);

    menu.exitMenu();
    return true;

}

bool channelShiftMode()    {

    pads.setNoteShiftMode(shiftMode_channel);
    display.displayShiftMode(shiftMode_channel);

    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim);

    menu.exitMenu();
    return true;

}

bool checkCalibration() {

    if (!pads.allPadsReleased())    {

        display.displayPadReleaseError(calibrationMode);
        menu.stopFunction();
        return false;

    }   return true;

}
#endif