//disable internal hardware serial object
#define HardwareSerial_h

#include "interface/pads/Pads.h"
#include "interface/buttons/Buttons.h"
#include "Debug.h"
#include "eeprom/EEPROMsettings.h"
#include "interface/leds/LEDs.h"
#include "interface/encoders/Encoders.h"
#include "hardware/pins/Pins.h"
#include "Types.h"
#include "hardware/timer/TimerObject.h"
#include "hardware/reset/Reset.h"
#include <util/delay.h>
#include "midi/MIDI.h"
#include "interface/lcd/LCD.h"

//define function prototypes
void startUpAnimation();
bool sendPadMIDI();

//implementation
bool sendPadMIDI()  {

    return (!pads.editModeActive());

}

void startUpAnimation() {

    //slow down fading for effect
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

    sei();

    display.displayHelloMessage();
    newDelay(600);

    //restore led states
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    newDelay(1500);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);

}

//init

void initHardware() {

    //do not change order of initialization!
    cli();

    configuration.init();
    timers.init();

    midi.init();
    display.init();
    leds.init();
    pads.init();
    encoders.init();

    leds.displayActiveNoteLEDs();

    #if START_UP_ANIMATION > 0
        startUpAnimation();
    #else
        sei();
    #endif

    //setup program/preset on display
    uint8_t activeProgram = pads.getActiveProgram();
    uint8_t activePreset = pads.getActivePreset();

    display.displayProgramAndPreset(activeProgram+1, activePreset);

    buttons.init();

}

void setup()    {

    #if MODE_SERIAL
        Serial.begin(38400);
        _delay_ms(100);
    #endif

    initHardware();

}

void loop()     {

    pads.update();
    buttons.update();
    encoders.update();
    display.update();

}