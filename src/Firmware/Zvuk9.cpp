/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2016

*/

#define FIRMWARE_VERSION_STRING     0x56
#define HARDWARE_VERSION_STRING     0x42
#define REBOOT_STRING               0x7F
#define FACTORY_RESET_STRING        0x44

#include "sysex/SysEx.h"
#include "interface/buttons/Buttons.h"
#include "interface/encoders/Encoders.h"
#include "interface/lcd/LCD.h"
#include "interface/lcd/menu/Menu.h"
#include "interface/leds/LEDs.h"
#include "interface/pads/Pads.h"
#include "version/Firmware.h"
#include "version/Hardware.h"
#include "eeprom/Configuration.h"

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
        wait_ms(1500);
        reboot();
        return true;

        case FACTORY_RESET_STRING:
        leds.setFadeSpeed(1);
        leds.allLEDsOff();
        wait_ms(1500);
        configuration.factoryReset(factoryReset_partial);
        reboot();
        return true;

    }   return false;

}

sysExParameter_t onGet(uint8_t block, uint8_t section, uint16_t index) {

    return configuration.readParameter(block, section, index);

}

bool onSet(uint8_t block, uint8_t section, uint16_t index, sysExParameter_t newValue)   {

    return configuration.writeParameter(block, section, index, newValue);

}

void startUpAnimation() {

    //slow down fading for effect
    leds.setFadeSpeed(1);

    ledState_t tempLedStateArray[NUMBER_OF_LEDS];

    for (int i=0; i<NUMBER_OF_LEDS; i++)    {

        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledStateOff);

    }

    //turn all leds on slowly
    leds.allLEDsOn();

    sei();

    display.displayHelloMessage();

    wait_ms(600);

    //restore led states
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    wait_ms(1500);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);

}

int main()    {

    //disable watchdog
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    #ifdef DEBUG
    vserial.init();
    #endif

    //do not change order of initialization!
    //configuration.init();

    #ifdef NDEBUG
    midi.init(dinInterface);
    midi.init(usbInterface);
    midi.setInputChannel(1);
    #endif

    board.init();

    display.init();
    menu.init();

    leds.init();

    pads.init();

    leds.displayActiveNoteLEDs();

    #ifdef START_UP_ANIMATION
    startUpAnimation();
    #else
    sei();
    #endif

    display.displayProgramAndScale(pads.getActiveProgram()+1, pads.getActiveScale());

    buttons.init();

    if (checkNewRevision()) {

        display.displayFirmwareUpdated();

    }

    #ifdef NDEBUG
    sysEx.setHandleGet(onGet);
    sysEx.setHandleSet(onSet);
    sysEx.setHandleCustomRequest(onCustom);

    sysEx.addCustomRequest(FIRMWARE_VERSION_STRING);
    sysEx.addCustomRequest(HARDWARE_VERSION_STRING);
    sysEx.addCustomRequest(REBOOT_STRING);
    sysEx.addCustomRequest(FACTORY_RESET_STRING);
    #endif

    while(1) {

        //printf("testing\n");
        //wait_ms(1000);

        //pads.update();
        buttons.update();
        encoders.update();
        display.update();
        #ifdef DEBUG
        vserial.update();
        #endif

        #ifdef ENABLE_ASYNC_UPDATE
        //write to eeprom when all pads are released
        if (pads.allPadsReleased())
            configuration.update();
        #endif

        #ifdef NDEBUG
        if (midi.read(usbInterface))   {   //new message on usb

            midiMessageType_t messageType = midi.getType(usbInterface);

            switch(messageType) {

                case midiMessageSystemExclusive:
                sysEx.handleSysEx(midi.getSysExArray(usbInterface), midi.getSysExArrayLength(usbInterface));
                break;

                default:
                break;

            }

        }
        #endif

    }

    return 0;

}