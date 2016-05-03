/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2016

*/

#include <avr/interrupt.h>

#include "hardware/pins/Pins.h"

#include "Modules.h"

#include "interface/pads/Pads.h"

#ifdef MODULE_BUTTONS
#include "interface/buttons/Buttons.h"
#endif

#ifdef MODULE_LEDS
#include "interface/leds/LEDs.h"
#endif

#ifdef MODULE_LCD
#include "interface/lcd/LCD.h"
#endif

#ifdef MODULE_ENCODERS
#include "interface/encoders/Encoders.h"
#endif

#include "hardware/timer/TimerObject.h"

#include "Debug.h"
#include "eeprom/EEPROMsettings.h"
#include "Types.h"
#include "hardware/reset/Reset.h"
#include <util/delay.h>
#include <avr/wdt.h>
#include "midi/MIDI.h"

#define START_UP_ANIMATION  1

//define function prototypes
void startUpAnimation();

void startUpAnimation() {

    //slow down fading for effect
    #ifdef MODULE_LEDS
    leds.setFadeSpeed(1);

    ledIntensity_t tempLedStateArray[NUMBER_OF_LEDS];

    for (int i=0; i<NUMBER_OF_LEDS; i++)    {

        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledIntensityOff);

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

//init

void initHardware() {

    //disable watchdog
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    setOutputMacro(BTLDR_BUTTON_DDR, BTLDR_BUTTON_PIN_INDEX);
    setLowMacro(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN_INDEX);

    #if MODE_SERIAL < 1
    midi.init();
    #else
    vserial.init();
    #endif

    //do not change order of initialization!
    configuration.init();
    sei();
    timers.init();

    #ifdef MODULE_LCD
    display.init();
    #endif

    #ifdef MODULE_LEDS
    leds.init();
    #endif

    pads.init();

    #ifdef MODULE_ENCODERS
    encoders.init();
    #endif

    #ifdef MODULE_LEDS
    leds.displayActiveNoteLEDs();
    #endif

    #if START_UP_ANIMATION > 0
    startUpAnimation();
    #else
    sei();
    #endif

    //setup program/preset on display
    uint8_t activeProgram = pads.getActiveProgram();
    uint8_t activePreset = pads.getActivePreset();

    #ifdef MODULE_LCD
    display.displayProgramAndPreset(activeProgram+1, activePreset);
    #endif

    #ifdef MODULE_BUTTONS
    buttons.init();
    #endif

}

int main()    {

    initHardware();

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

    }

    return 0;

}