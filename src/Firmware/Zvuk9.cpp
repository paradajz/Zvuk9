/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2016

*/

#include "init/Init.h"

#define FIRMWARE_VERSION_STRING 0x56
#define HARDWARE_VERSION_STRING 0x42
#define REBOOT_STRING           0x7F
#define FACTORY_RESET_STRING    0x44

bool onCustom(uint8_t value) {

    switch(value)   {

        case FIRMWARE_VERSION_STRING:
        sysEx.addToResponse(getSWversion(swVersion_major));
        sysEx.addToResponse(getSWversion(swVersion_minor));
        sysEx.addToResponse(getSWversion(swVersion_revision));
        return true;

        case HARDWARE_VERSION_STRING:
        sysEx.addToResponse(hardwareVersion.major);
        sysEx.addToResponse(hardwareVersion.minor);
        sysEx.addToResponse(hardwareVersion.revision);
        return true;

        case REBOOT_STRING:
        leds.setFadeSpeed(1);
        leds.allLEDsOff();
        wait(1500);
        reboot();
        return true; //pointless, but we're making compiler happy

        case FACTORY_RESET_STRING:
        leds.setFadeSpeed(1);
        leds.allLEDsOff();
        wait(1500);
        configuration.factoryReset(factoryReset_partial);
        reboot();
        return true;

    }   return false;

}

sysExParameter_t onGet(uint8_t block, uint8_t section, sysExParameter_t index) {

    return configuration.readParameter(block, section, index);

}

bool onSet(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue)   {

    return configuration.writeParameter(block, section, index, newValue);

}

bool onReset(uint8_t block, uint8_t section, sysExParameter_t index) {

    switch(block) {

        case CONF_BLOCK_PROGRAM:
        break;

        case CONF_BLOCK_USER_SCALE:
        break;

        case CONF_BLOCK_PAD_CALIBRATION:
        break;

        case CONF_BLOCK_MIDI:
        break;

        case CONF_BLOCK_PRESSURE_SETTINGS:
        break;

    }   return false;

}

int main()    {

    globalInit();

    #if MODE_SERIAL < 1
    sysEx.setHandleGet(onGet);
    sysEx.setHandleSet(onSet);
    sysEx.setHandleReset(onReset);
    sysEx.setHandleCustom(onCustom);

    if (!sysEx.addCustomString(FIRMWARE_VERSION_STRING))    {

        #if MODE_SERIAL > 0
        printf("Error while adding custom string for SysEx\n");
        #endif

    }

    if (!sysEx.addCustomString(HARDWARE_VERSION_STRING))    {

        #if MODE_SERIAL > 0
        printf("Error while adding custom string for SysEx\n");
        #endif

    }

    if (!sysEx.addCustomString(REBOOT_STRING))    {

        #if MODE_SERIAL > 0
        printf("Error while adding custom string for SysEx\n");
        #endif

    }

    if (!sysEx.addCustomString(FACTORY_RESET_STRING))    {

        #if MODE_SERIAL > 0
        printf("Error while adding custom string for SysEx\n");
        #endif

    }

    #endif

    while(1) {

        pads.update();

        #ifdef MODULE_BUTTONS
        buttons.update();
        #endif

        #ifdef MODULE_ENCODERS
        encoders.update();
        #endif

        #ifdef MODULE_LCD
        display.update();
        #endif

        #if MODE_SERIAL > 0
        vserial.update();
        #endif

        #ifdef ENABLE_ASYNC_UPDATE
        //write to eeprom when all pads are released
        if (pads.allPadsReleased())
            configuration.update();
        #endif

        #if MODE_SERIAL < 1
        midi.checkInput();
        #endif

    }

    return 0;

}