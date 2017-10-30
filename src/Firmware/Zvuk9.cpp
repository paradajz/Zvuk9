#define FIRMWARE_VERSION_STRING     0x56
#define HARDWARE_VERSION_STRING     0x42
#define REBOOT_STRING               0x7F
#define FACTORY_RESET_STRING        0x44

#include "interface/buttons/Buttons.h"
#include "interface/encoders/Encoders.h"
#include "interface/lcd/LCD.h"
#include "interface/lcd/menu/Menu.h"
#include "interface/leds/LEDs.h"
#include "interface/pads/Pads.h"
#include "versioning/src/avr/Version.h"
#include "database/Database.h"
#include "midi/src/MIDI.h"

#ifdef DEBUG
#include "vserial/Serial.h"
#endif

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

    display.displayWelcomeMessage();

    wait_ms(1500);

    //restore led states
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    wait_ms(1000);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);
}

int main()
{
    //do not change order of initialization!

    #ifdef DEBUG
    vserial.init();
    #endif

    board.init();
    display.init();

    database.init();

    if (!database.signatureValid())
    {
        display.setDirectWriteState(true);
        strcpy_P(stringBuffer, dbInit_string);
        display.updateText(1, lcdtext_still, display.getTextCenter(ARRAY_SIZE(dbInit_string)));
        strcpy_P(stringBuffer, pleaseWait_string);
        display.updateText(2, lcdtext_still, display.getTextCenter(ARRAY_SIZE(pleaseWait_string)));
        database.factoryReset(initWipe);
        display.setDirectWriteState(false);
    }

    #ifdef USE_USB_MIDI
    midi.init(dinInterface);
    midi.init(usbInterface);
    midi.setInputChannel(1);
    midi.setNoteOffMode((noteOffType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID));
    bool runningStatus = database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID);
    runningStatus ? midi.enableRunningStatus() : midi.disableRunningStatus();
    #endif

    leds.init();
    pads.init();
    encoders.init();

    leds.displayActiveNoteLEDs();

    #ifdef START_UP_ANIMATION
    startUpAnimation();
    #else
    sei();
    #endif

    display.setupHomeScreen();

    buttons.init();

    #ifndef DEBUG
    if (checkNewRevision())
        display.displayFirmwareUpdated();
    #endif

    //flush all data from encoders
    encoders.flush();

    while (1)
    {
        pads.update();
        buttons.update();
        encoders.update();
        display.update();

        #ifdef DEBUG
        vserial.update();
        #endif

        #ifdef DBMS_ENABLE_ASYNC_UPDATE
        //write to eeprom when all pads are released
        if (!pads.getNumberOfPressedPads())
            database.checkQueue();
        #endif
    }

    return 0;
}