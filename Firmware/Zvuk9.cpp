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

    menu.init();

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

}

void calibratePads()    {

    //see default values in eeprom/Defaults.h from line 160

    //pads.calibrate(xCalibration, lower, 0, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 1, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 2, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 3, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 4, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 5, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 6, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 7, DEFAULT_PAD_X_LIMIT_LOWER);
    //pads.calibrate(xCalibration, lower, 8, DEFAULT_PAD_X_LIMIT_LOWER);
//
    //pads.calibrate(xCalibration, upper, 0, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 1, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 2, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 3, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 4, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 5, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 6, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 7, DEFAULT_PAD_X_LIMIT_UPPER);
    //pads.calibrate(xCalibration, upper, 8, DEFAULT_PAD_X_LIMIT_UPPER);
//
    //pads.calibrate(yCalibration, lower, 0, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 1, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 2, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 3, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 4, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 5, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 6, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 7, DEFAULT_PAD_Y_LIMIT_LOWER);
    //pads.calibrate(yCalibration, lower, 8, DEFAULT_PAD_Y_LIMIT_LOWER);
//
    //pads.calibrate(yCalibration, upper, 0, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 1, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 2, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 3, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 4, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 5, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 6, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 7, DEFAULT_PAD_Y_LIMIT_UPPER);
    //pads.calibrate(yCalibration, upper, 8, DEFAULT_PAD_Y_LIMIT_UPPER);
//
    //pads.calibrate(pressureCalibration, lower, 0, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 1, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 2, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 3, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 4, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 5, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 6, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 7, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
    //pads.calibrate(pressureCalibration, lower, 8, DEFAULT_PAD_PRESSURE_LIMIT_LOWER);
//
    //pads.calibrate(pressureCalibration, upper, 0, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 1, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 2, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 3, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 4, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 5, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 6, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 7, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);
    //pads.calibrate(pressureCalibration, upper, 8, DEFAULT_PAD_PRESSURE_LIMIT_UPPER);

}

int main()    {

    initHardware();
    calibratePads();

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