/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2016

*/

#include "init/Init.h"
#include "sysex/SysEx.h"

void onReboot()  {

    //ledsOff_slow();
    ////this will reset the board into bootloader mode
    //reboot(BTLDR_REBOOT);

}

void onFactoryReset()   {

    //ledsOff_slow();
    //configuration.factoryReset();
    //reboot(APP_REBOOT);

}

uint8_t onGet(uint8_t messageType, uint8_t messageSubtype, uint8_t parameter) {

    switch(messageType) {

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

    } return INVALID_VALUE;

}

bool onSet(uint8_t messageType, uint8_t messageSubtype, uint8_t parameter, uint8_t newParameter)   {

    switch(messageType) {

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

bool onReset(uint8_t messageType, uint8_t messageSubtype, uint8_t parameter) {

    switch(messageType) {

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

    sysEx.setHandleReboot(onReboot);
    sysEx.setHandleGet(onGet);
    sysEx.setHandleSet(onSet);
    sysEx.setHandleReset(onReset);
    sysEx.setHandleFactoryReset(onFactoryReset);

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

        midi.checkInput();

    }

    return 0;

}