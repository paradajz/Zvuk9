/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2017

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
#include "database/Database.h"

bool onCustom(uint8_t value)
{
    switch(value)
    {
        case FIRMWARE_VERSION_STRING:
        sysEx.addToResponse(getSWversion(version_major));
        sysEx.addToResponse(getSWversion(version_minor));
        sysEx.addToResponse(getSWversion(version_revision));
        return true;

        case HARDWARE_VERSION_STRING:
        sysEx.addToResponse(HARDWARE_VERSION_MAJOR);
        sysEx.addToResponse(HARDWARE_VERSION_MINOR);
        sysEx.addToResponse(HARDWARE_VERSION_REVISION);
        return true;

        case REBOOT_STRING:
        leds.setFadeSpeed(1);
        leds.setAllOff();
        wait_ms(1500);
        reboot();
        return true;

        case FACTORY_RESET_STRING:
        leds.setFadeSpeed(1);
        leds.setAllOff();
        wait_ms(1500);
        db.factoryReset(factoryReset_partial);
        reboot();
        return true;
    }

    return false;
}

sysExParameter_t onGet(uint8_t block, uint8_t section, uint16_t index)
{
    return db.read(block, section, index);
}

bool onSet(uint8_t block, uint8_t section, uint16_t index, sysExParameter_t newValue)
{
    return db.update(block, section, index, newValue);
}

void startUpAnimation()
{
    //slow down fading for effect
    leds.setFadeSpeed(3);

    ledState_t tempLedStateArray[MAX_NUMBER_OF_LEDS];

    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
    {
        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledStateOff);
    }

    //turn all leds on slowly
    leds.setAllOn();

    sei();

    #ifdef BOARD_R1
    display.displayHelloMessage();
    #endif

    wait_ms(2000);

    //restore led states
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    wait_ms(1000);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);
}

int main()
{
    //disable watchdog
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    #ifdef DEBUG
    vserial.init();
    #endif

    //do not change order of initialization!
    db.init();
    if (!db.checkSignature())
    {
        #ifdef BOARD_R1
        char tempBuffer[20];

        strcpy_P(tempBuffer, restoringDefaults_string);
        lcd_clrscr();
        lcd_gotoxy(0, 0);
        lcd_puts(tempBuffer);
        lcd_gotoxy(0, 1);
        wait_ms(2000);
        strcpy_P(tempBuffer, pleaseWait_string);
        lcd_puts(tempBuffer);
        wait_ms(2000);
        #endif

        db.factoryReset(factoryReset_restore);

        #ifdef BOARD_R1
        lcd_gotoxy(0,2);
        strcpy_P(tempBuffer, complete_string);
        lcd_puts(tempBuffer);
        wait_ms(2000);
        #endif
    }

    #ifdef NDEBUG
    midi.init(dinInterface);
    midi.init(usbInterface);
    midi.setInputChannel(1);
    midi.setNoteOffMode((noteOffType_t)db.read(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID));
    bool runningStatus = db.read(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID);
    runningStatus ? midi.enableRunningStatus() : midi.disableRunningStatus();
    #endif

    board.init();

    display.init();
    menu.init();
    leds.init();
    pads.init();
    encoders.init();

    leds.displayActiveNoteLEDs();

    #ifdef START_UP_ANIMATION
    startUpAnimation();
    #else
    sei();
    #endif

    display.displayProgramAndScale(pads.getActiveProgram()+1, pads.getActiveScale());

    buttons.init();

    if (checkNewRevision())
        display.displayFirmwareUpdated();

    #ifdef NDEBUG
    sysEx.setHandleGet(onGet);
    sysEx.setHandleSet(onSet);
    sysEx.setHandleCustomRequest(onCustom);

    sysEx.addCustomRequest(FIRMWARE_VERSION_STRING);
    sysEx.addCustomRequest(HARDWARE_VERSION_STRING);
    sysEx.addCustomRequest(REBOOT_STRING);
    sysEx.addCustomRequest(FACTORY_RESET_STRING);
    #endif

    while(1)
    {
        //pads.update();
        //buttons.update();
        //encoders.update();
        #ifdef BOARD_R1
        display.update();
        #endif

        #ifdef DEBUG
        vserial.update();
        #endif

        //#ifdef ENABLE_ASYNC_UPDATE
        ////write to eeprom when all pads are released
        //if (pads.allPadsReleased())
            //db.checkQueue();
        //#endif
//
        //#ifdef NDEBUG
        //if (midi.read(usbInterface))
        //{
            ////new message on usb
            //midiMessageType_t messageType = midi.getType(usbInterface);
//
            //switch(messageType)
            //{
                //case midiMessageSystemExclusive:
                //sysEx.handleSysEx(midi.getSysExArray(usbInterface), midi.getSysExArrayLength(usbInterface));
                //break;
//
                //default:
                //break;
            //}
        //}
        //#endif
    }

    return 0;
}