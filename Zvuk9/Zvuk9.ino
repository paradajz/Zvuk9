//disable internal hardware serial object
#define HardwareSerial_h

#include "interface/lcd/LCD.h"
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

bool serviceMenuActive = false;

//define function prototypes
void startUpAnimation();
bool sendPadMIDI();

//implementation
bool sendPadMIDI()  {

    return (!pads.editModeActive() && !serviceMenuActive);

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

    lcDisplay.displayHelloMessage();
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

    //buttons use TWI, and we need to setup port expanders
    //the process requires interrupts to be enabled so init buttons first
    //and later reconfigure timers
    buttons.init();
    cli();

    configuration.init();
    timers.init();

    midi.init();
    lcDisplay.init();
    leds.init();
    pads.init();
    encoders.init();

    //setup program/preset on display
    uint8_t activeProgram = pads.getActiveProgram();
    uint8_t activePreset = pads.getActivePreset();

    lcDisplay.setProgram(activeProgram+1);
    lcDisplay.setPreset(activePreset);

    leds.displayActiveNoteLEDs();

    #if START_UP_ANIMATION > 0
        startUpAnimation();
    #else
        sei();
    #endif

}

void setup()    {

    #if MODE_SERIAL
        Serial.begin(38400);
        _delay_ms(100);
    #endif

    initHardware();

    uint32_t currentTime = newMillis();

    //read buttons for 0.1 seconds
    do {

        //read all buttons without activating callbacks
        buttons.update(false);

    }   while ((newMillis() - currentTime) < 100);

    if (buttons.getButtonPressed(BUTTON_TRANSPORT_PLAY) && buttons.getButtonPressed(BUTTON_TRANSPORT_STOP)) {

        //we should activate service menu now
        #if MODE_SERIAL > 0
            Serial.println(F("Activating user menu"));
        #endif

        lcDisplay.displayServiceMenu();
        serviceMenuActive = true;

    }

}

void loop()     {

    pads.update(sendPadMIDI());
    buttons.update();
    encoders.update();
    lcDisplay.update();

    if (checkReset(buttons.getButtonPressed(BUTTON_TRANSPORT_PLAY)))    {

        lcDisplay.displayUserMessage("Press STOP to reset");
        leds.allLEDsOn();
        lcDisplay.update();

        do {

            buttons.update();

        } while (!buttons.getButtonPressed(BUTTON_TRANSPORT_STOP));

        leds.allLEDsOff();
        newDelay(50);
        _reboot_Teensyduino_();

    }

}