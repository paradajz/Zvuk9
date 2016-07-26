#include "Init.h"
#include "../version/Firmware.h"

void startUpAnimation() {

    //slow down fading for effect
    #ifdef MODULE_LEDS
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
    #endif

    sei();

    #ifdef MODULE_LCD
    display.displayHelloMessage();
    #endif

    wait(600);

    #ifdef MODULE_LEDS
    //restore led states
    for (int i=0; i<NUMBER_OF_LEDS; i++)
    leds.setLEDstate(i, tempLedStateArray[i]);

    wait(1500);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);
    #endif

}

void globalInit() {

    //disable watchdog
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    setOutputMacro(BTLDR_BUTTON_DDR, BTLDR_BUTTON_PIN_INDEX);
    setLowMacro(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN_INDEX);

    #if MODE_SERIAL > 0
    vserial.init();
    #endif

    //do not change order of initialization!
    configuration.init();

    #if MODE_SERIAL < 1
    midi.init();
    #endif

    sei();
    timers.init();

    #ifdef MODULE_LCD
    display.init();
    menu.init();
    #endif

    #ifdef MODULE_LEDS
    leds.init();
    #endif

    pads.init();

    #ifdef MODULE_LEDS
    leds.displayActiveNoteLEDs();
    #endif

    #if START_UP_ANIMATION > 0
    startUpAnimation();
    #else
    sei();
    #endif

    #ifdef MODULE_LCD
    display.displayProgramAndPreset(pads.getActiveProgram()+1, pads.getActivePreset());
    #endif

    #ifdef MODULE_BUTTONS
    buttons.init();
    #endif

    if (checkNewRevision()) {

        display.displayFirmwareUpdated();

    }

}

void reboot()   {

    wdt_enable(WDTO_30MS);
    while(1) {};

}