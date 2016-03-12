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
bool checkOctaveUpDownEnabled();
void padsOctaveUpDown(uint8_t direction, bool buttonState);
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

bool checkOctaveUpDownEnabled() {

    //HORRIBLE HACK!!!!!
    //please fix me someday

    //this function will check whether octave up/down is enabled
    //there are two scenarios when those two buttons should be disabled:
    //1) Pad edit mode is activated
    //2) Pad edit mode is disabled

    //in both cases octave/up down functions should be disabled until both buttons
    //have been released

    //check should start when pad edit mode is activated or deactivated and stopped when
    //both buttons are released

    static bool lastPadEditMode = false;
    bool currentPadEditMode = pads.editModeActive();
    static bool checkEnabled = false;
    bool returnValue = true;

    if (lastPadEditMode != currentPadEditMode) {

        lastPadEditMode = currentPadEditMode;
        checkEnabled = true;

    }

    if (checkEnabled) {

        //checking has been started
        if (!buttons.getButtonPressed(BUTTON_OCTAVE_DOWN) && !buttons.getButtonPressed(BUTTON_OCTAVE_UP))   {

            //both buttons have been released, stop with the checking
            //use returnValue as return variable since we don't want
            //to enable octave up/down immediately after release,
            //only when one of octave buttons has been pushed again
            if (!checkEnabled) returnValue = true; else returnValue = false;
            checkEnabled = false;

        }

    }

    //if checking is in progress, return false
    if (checkEnabled) return false;

    //if checking has been finished, return returnValue
    return returnValue;

}

void padsOctaveUpDown(uint8_t direction, bool buttonState)    {

    if (buttons.getButtonPressed(BUTTON_OCTAVE_DOWN) && buttons.getButtonPressed(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode

        if (pads.getActivePreset() >= NUMBER_OF_PREDEFINED_SCALES)    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    checkOctaveUpDownEnabled();
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                }

            }   else pads.exitPadEditMode();

        }   else {

                //used to "cheat" checkOctaveUpDownEnabled function
                //this will cause reset of function counters so that buttons are disabled
                //after LCD shows message that pad editing isn't allowed
                pads.setEditMode(!pads.editModeActive());
                checkOctaveUpDownEnabled();
                pads.setEditMode(!pads.editModeActive());
                lcDisplay.displayEditModeNotAllowed(noUserPreset);

            }

    }

    if (!pads.editModeActive() && checkOctaveUpDownEnabled())    {

        if (!buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))   {

            //shift entire octave

            //shift all notes up or down
            if (!buttonState)    {

                changeOutput_t shiftResult = pads.shiftOctave(direction);
                int8_t activeOctave = pads.getActiveOctave();
                lcDisplay.displayNoteChange(shiftResult, octaveChange, activeOctave);
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);

            }   else {

                //direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);

            }

        }   else {

            //shift single note
            if (buttonState)    {

                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    return;

                }

                //stop button is modifier
                //disable it on release
                buttons.pauseCallback(BUTTON_TRANSPORT_STOP);
                changeOutput_t shiftResult = pads.shiftNote(direction);
                lcDisplay.displayNoteChange(shiftResult, noteUpOrDown, direction);

            }

        }

    }   else if (pads.editModeActive() && checkOctaveUpDownEnabled()) {

        //do not shift octaves while pad is pressed
        if (pads.getPadPressed(pads.getLastTouchedPad()))   {

            lcDisplay.displayEditModeNotAllowed(padNotReleased);

        }   else if (!buttonState)   {

            pads.changeActiveOctave(direction);
            lcDisplay.displayActiveOctave(pads.getActiveOctave());
            leds.displayActiveNoteLEDs(true, pads.getLastTouchedPad());

        }

    }

}

//callbacks

void configureButtonCallbacks() {

    buttons.setHandleOctaveUpDownPress(padsOctaveUpDown);

}

void configureCallbacks()   {

    configureButtonCallbacks();

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

    configureCallbacks();
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