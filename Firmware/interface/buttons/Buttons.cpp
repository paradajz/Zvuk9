#include "Buttons.h"

#include <util/delay.h>

#ifdef BUTTONS_H_

#include "../../hardware/i2c/i2c_master.h"
#include "../pads/Pads.h"

#ifdef MODULE_LCD
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#endif

#ifdef MODULE_LEDS
#include "../leds/LEDs.h"
#endif

#include "../../midi/MIDI.h"
#include "../../hardware/reset/Reset.h"

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         10
#define STOP_DISABLE_TIMEOUT        500
#define RESET_ENABLE_TIMEOUT        4000

#define SOFT_REBOOT                 1

#define EDIT_MODE_COUNTER           2

//MCP23017 address bytes
const uint8_t expanderAddress[] = { 0x21, 0x20 };   //chip address
const uint8_t gpioAddress[]     = { 0x12, 0x13 };   //input/output
const uint8_t iodirAddress[]    = { 0x00, 0x01 };   //port A/port B
const uint8_t gppuAddress[]     = { 0x0C, 0x0D };   //interrupt/pull-up

//shift new values from button in this variable
//if it's 0xFF or buttonDebounceCompare, its reading is stable
const uint8_t buttonDebounceCompare = 0b11110000;

Buttons::Buttons()  {

    //default constructor
    lastCheckTime               = 0;
    lastButtonDataPress         = 0;
    mcpData                     = 0;
    modifierActive              = false;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

        previousButtonState[i] = buttonDebounceCompare;
        buttonEnabled[i] = true;

    }

}

uint8_t read_I2C_reg(uint8_t address, uint8_t reg)   {

    uint8_t value;

    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_stop();

    i2c_start((address << 1) + I2C_READ);
    value = i2c_read_nack();
    i2c_stop();

    return value;
}

void write_I2C_reg(uint8_t address, uint8_t reg, uint8_t value)  {

    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();

}

void Buttons::init()  {

    i2c_init();

    //ensure that we know the configuration
    write_I2C_reg(expanderAddress[0], 0x0A, 0x00);         //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[0], 0x05, 0x00);         //IOCON=0x00 if BANK=1
    write_I2C_reg(expanderAddress[1], 0x0A, 0x00);         //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[1], 0x05, 0x00);         //IOCON=0x00 if BANK=1

    write_I2C_reg(expanderAddress[0], iodirAddress[0], 0xFF);   //expander 1, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[0], iodirAddress[1], 0xFF);   //expander 1, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[1], iodirAddress[0], 0xFF);   //expander 2, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[1], iodirAddress[1], 0xFF);   //expander 2, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[0], gppuAddress[0], 0xFF);    //expander 1, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[0], gppuAddress[1], 0xFF);    //expander 1, turn on pull-ups, PORTB

    write_I2C_reg(expanderAddress[1], gppuAddress[0], 0xFF);    //expander 2, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[1], gppuAddress[1], 0xFF);    //expander 2, turn on pull-ups, PORTB

    uint32_t currentTime = rTimeMillis();

    //read buttons for 0.1 seconds
    do {

        //read all buttons without activating event handlers
        buttons.update(false);

    }   while ((rTimeMillis() - currentTime) < 100);

    if (buttons.getButtonState(BUTTON_TRANSPORT_PLAY) && buttons.getButtonState(BUTTON_TRANSPORT_STOP)) {

        //we should activate service menu now
        #if MODE_SERIAL > 0
            vserial.println("Activating user menu");
        #endif

        #ifdef MODULE_LCD
            menu.displayMenu(serviceMenu);
        #endif

        buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
        buttonEnabled[BUTTON_TRANSPORT_PLAY] = false;

    }

}

void Buttons::update(bool processingEnabled)    {

    if (!((rTimeMillis() - lastCheckTime) > EXPANDER_CHECK_TIME)) return;

     if (readStates())   {

         for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

             //invert button state because of pull-ups
             uint8_t state = !((mcpData >> i) & 0x01);
             bool debounced = buttonDebounced(i, state);

             if (debounced) {

                if (state == getButtonState(i)) continue;

                //update previous button state with current one
                setButtonState(i, state);
                if (processingEnabled) processButton(i, state);

             }

         }   mcpData = 0;

     }

     //we know stop button is modifier
     if (getButtonState(BUTTON_TRANSPORT_STOP) && buttonEnabled[BUTTON_TRANSPORT_STOP])   {

        //measure the time the button is pressed
        if (!stopDisableTimeout) stopDisableTimeout = rTimeMillis();
        else if ((rTimeMillis() - stopDisableTimeout) > STOP_DISABLE_TIMEOUT) {

            buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
            stopDisableTimeout = 0;
            #ifdef MODULE_LCD
            display.displayModifierEnabled();
            #endif
            modifierActive = true;

            #ifdef MODULE_LEDS
                if (!pads.editModeActive() && (pads.getActivePreset() < NUMBER_OF_PREDEFINED_SCALES)) {

                    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim);
                    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);

                } leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityFull);
            #endif

        }

     }  else stopDisableTimeout = 0;

     #if SOFT_REBOOT > 0
         if (buttons.getButtonState(BUTTON_TRANSPORT_PLAY) && buttonEnabled[BUTTON_TRANSPORT_PLAY])    {

            if (!resetActivationTimeout) resetActivationTimeout = rTimeMillis();
            else if ((rTimeMillis() - resetActivationTimeout) > RESET_ENABLE_TIMEOUT)   {

                #ifdef MODULE_LEDS
                    //slowly turn off all the leds
                    leds.setFadeSpeed(1);
                    leds.allLEDsOff();
                    _delay_ms(2500);
                #endif
                #ifdef MODULE_LCD
                    display.displayDFUmode();
                #endif
                bootloaderReboot();

            }

         }  else resetActivationTimeout = 0;
     #endif

     lastCheckTime = rTimeMillis();

}

bool Buttons::getButtonState(uint8_t buttonNumber) {

    return bitRead(lastButtonDataPress, buttonNumber);

}

bool Buttons::readStates()   {

    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[0]);     //expander A, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[1]);     //expander A, GPIOB
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[0]);     //expander B, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[1]);     //expander B, GPIOB

    return true;

}

bool Buttons::buttonDebounced(uint8_t buttonNumber, uint8_t state)  {

    //shift new button reading into previousButtonState
    previousButtonState[buttonNumber] = (previousButtonState[buttonNumber] << 1) | state | buttonDebounceCompare;

    //if button is debounced, return true
    return ((previousButtonState[buttonNumber] == buttonDebounceCompare) || (previousButtonState[buttonNumber] == 0xFF));

}

void Buttons::processButton(uint8_t buttonNumber, bool state)    {

    if (buttonEnabled[buttonNumber])    {

        if (state)    {

            switch(buttonNumber)    {

                case BUTTON_ON_OFF_AFTERTOUCH:
                case BUTTON_ON_OFF_NOTES:
                case BUTTON_ON_OFF_X:
                case BUTTON_ON_OFF_Y:
                case BUTTON_ON_OFF_SPLIT:
                handleOnOffEvent(buttonNumber);
                break;

                case BUTTON_NOTE_C_SHARP:
                case BUTTON_NOTE_D_SHARP:
                case BUTTON_NOTE_F_SHARP:
                case BUTTON_NOTE_G_SHARP:
                case BUTTON_NOTE_A_SHARP:
                case BUTTON_NOTE_C:
                case BUTTON_NOTE_D:
                case BUTTON_NOTE_E:
                case BUTTON_NOTE_F:
                case BUTTON_NOTE_G:
                case BUTTON_NOTE_A:
                case BUTTON_NOTE_B:
                note_t note = getTonicFromButton(buttonNumber);
                handleTonicEvent(note);
                break;

            }

        }

        switch (buttonNumber)   {

            case BUTTON_OCTAVE_DOWN:
            handleOctaveEvent(false, state);
            break;

            case BUTTON_OCTAVE_UP:
            handleOctaveEvent(true, state);
            break;

            case BUTTON_TRANSPORT_STOP:
            case BUTTON_TRANSPORT_PLAY:
            case BUTTON_TRANSPORT_RECORD:
            handleTransportControlEvent(buttonNumber, state);
            break;

        }

    }   else {

        //hack!
        //we know stop button is modifier
        if (buttonNumber == BUTTON_TRANSPORT_STOP)  {

            //in this case it's disabled
            //check if it's released
            if (!state) {

                //restore led states
                #ifdef MODULE_LEDS
                    leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);
                    leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                    leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityOff);
                #endif
                modifierActive = false;

            }

        }

    }

    //resume all callbacks
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

        if (!buttonEnabled[i] && !getButtonState(i)) {

            buttonEnabled[i] = true;

        }

    }

}

void Buttons::setButtonState(uint8_t buttonNumber, uint8_t state) {

    bitWrite(lastButtonDataPress, buttonNumber, state);

}

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)   {

    switch(buttonNumber)    {

        case BUTTON_NOTE_C:
        return C;

        case BUTTON_NOTE_C_SHARP:
        return C_SHARP;

        case BUTTON_NOTE_D:
        return D;

        case BUTTON_NOTE_D_SHARP:
        return D_SHARP;

        case BUTTON_NOTE_E:
        return E;

        case BUTTON_NOTE_F:
        return F;

        case BUTTON_NOTE_F_SHARP:
        return F_SHARP;

        case BUTTON_NOTE_G:
        return G;

        case BUTTON_NOTE_G_SHARP:
        return G_SHARP;

        case BUTTON_NOTE_A:
        return A;

        case BUTTON_NOTE_A_SHARP:
        return A_SHARP;

        case BUTTON_NOTE_B:
        return B;

    }   return MIDI_NOTES;   //impossible case

}

void Buttons::handleOnOffEvent(uint8_t buttonNumber)    {

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    functionsOnOff_t lcdMessageType;
    ledIntensity_t ledState = ledIntensityOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        pads.notesOnOff();
        lcdMessageType = featureNotes;
        ledNumber = LED_ON_OFF_NOTES;
        if (pads.getNoteSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        if (modifierActive) {

            //change aftertouch type
            lcdMessageType = featureAftertouchType;
            pads.changeAftertouchType();
            //split and pad number are irrelevant here
            #ifdef MODULE_LCD
                display.displayOnOff(lcdMessageType, false, (uint8_t)pads.getAftertouchType(), 0);
            #endif
            return;

        }   else {

            pads.aftertouchOnOff();
            lcdMessageType = featureAftertouch;
            ledNumber = LED_ON_OFF_AFTERTOUCH;
            if (pads.getAfterTouchSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;

        }
        break;

        case BUTTON_ON_OFF_X:
        pads.xOnOff();
        lcdMessageType = featureX;
        ledNumber = LED_ON_OFF_X;
        if (pads.getCCXsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_Y:
        pads.yOnOff();
        lcdMessageType = featureY;
        ledNumber = LED_ON_OFF_Y;
        if (pads.getCCYsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        pads.splitOnOff();
        lcdMessageType = featureSplit;
        ledNumber = LED_ON_OFF_SPLIT;
        pads.getSplitState() ? ledState = ledIntensityFull : ledState = ledIntensityOff;
        break;

        default:
        return;

    }

    #ifdef MODULE_LEDS
        leds.setLEDstate(ledNumber, ledState);
    #endif

    #ifdef MODULE_LCD
        display.displayOnOff(lcdMessageType, pads.getSplitState(), (uint8_t)ledState, lastTouchedPad+1);
    #endif

}

void Buttons::handleTransportControlEvent(uint8_t buttonNumber, bool state)  {

    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    transportControl_t type = transportStop;
    bool displayState = true;

    switch(buttonNumber)    {

        case BUTTON_TRANSPORT_PLAY:
        if (state) {

            sysExArray[4] = 0x02;
            type = transportPlay;
            #if MODE_SERIAL > 0
                vserial.println("Transport Control Play");
            #endif
            #ifdef MODULE_LEDS
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityFull);
            #endif

        } else return;

        break;

        case BUTTON_TRANSPORT_STOP:
        if (!state)    {

            sysExArray[4] = 0x01;
            type = transportStop;
            #if MODE_SERIAL > 0
                vserial.println("Transport Control Stop");
            #endif
            #ifdef MODULE_LEDS
                leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityOff);
                leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityOff);
            #endif

        } else {

            #if MODE_SERIAL > 0
                vserial.println("Modifier active");
            #endif

            stopDisableTimeout = rTimeMillis();
            return;

        }
        break;

        case BUTTON_TRANSPORT_RECORD:
        if (state) {

            #ifdef MODULE_LEDS
                ledIntensity_t recordState = leds.getLEDstate(LED_TRANSPORT_RECORD);
                if (recordState == ledIntensityFull) {

                    sysExArray[4] = 0x07;
                    recordState = ledIntensityOff;
                    #if MODE_SERIAL > 0
                    vserial.println("Transport Control Record Stop");
                    #endif
                    displayState = false;

                    }   else if (recordState == ledIntensityOff) {

                    sysExArray[4] = 0x06;
                    recordState = ledIntensityFull;
                    #if MODE_SERIAL > 0
                    vserial.println("Transport Control Record Start");
                    #endif
                    displayState = true;

                }

                type = transportRecord;
                leds.setLEDstate(LED_TRANSPORT_RECORD, recordState);
            #endif

        } else return;

        break;

    }

    #if MODE_SERIAL < 1
        midi.sendSysEx(sysExArray, SYS_EX_ARRAY_SIZE);
    #endif

    #ifdef MODULE_LCD
        display.displayTransportControl(type, displayState);
    #endif

}

void Buttons::handleTonicEvent(note_t note) {

    if (!pads.editModeActive())   {

        //don't allow change of tonic while pads are pressed
        for (int i=0; i<MAX_PADS; i++)
        if (pads.isPadPressed(i))  {

            #ifdef MODULE_LCD
                display.displayPadReleaseError(changeTonic);
            #endif
            return;

        }

        changeOutput_t result = pads.setTonic(note);
        note_t activeTonic = pads.getActiveTonic();

        switch(result)  {

            case outputChanged:
            #ifdef MODULE_LEDS
                leds.displayActiveNoteLEDs();
            #endif
            break;

            default:
            break;

        }

        //always do this
        #ifdef MODULE_LCD
            display.displayNoteChange(result, tonicChange, activeTonic);
        #endif

    }   else {

        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        pads.assignPadNote(pad, note);
        pads.displayActivePadNotes(pad);
        #ifdef MODULE_LEDS
            leds.displayActiveNoteLEDs(true, pad);
        #endif

    }

}

void Buttons::handleOctaveEvent(bool direction, bool state)   {

    if (buttons.getButtonState(BUTTON_OCTAVE_DOWN) && buttons.getButtonState(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode

        if (pads.isUserScale(pads.getActivePreset()))    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.isPadPressed(pads.getLastTouchedPad()))   {

                    #ifdef MODULE_LCD
                        display.displayEditModeNotAllowed(padNotReleased);
                    #endif
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #if MODE_SERIAL > 0
                        vserial.println("----------------------------------------");
                        vserial.println("Pad edit mode");
                        vserial.println("----------------------------------------");
                    #endif
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                }

            }   else pads.exitPadEditMode();

        }   else {

            #ifdef MODULE_LCD
                display.displayEditModeNotAllowed(notUserPreset);
            #endif

        }

        //restore leds
        #ifdef MODULE_LEDS
            leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
            leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);
        #endif
        //stop buttons temporarily
        buttonEnabled[BUTTON_OCTAVE_UP] = false;
        buttonEnabled[BUTTON_OCTAVE_DOWN] = false;

    }   else {

        bool editMode = pads.editModeActive();

        switch(editMode)    {

            case true:
            switch(state)   {

                case false:
                pads.changeActiveOctave(direction);
                #ifdef MODULE_LCD
                    display.displayActiveOctave(normalizeOctave(pads.getActiveOctave()));
                #endif
                #ifdef MODULE_LEDS
                    leds.displayActiveNoteLEDs(true, pads.getLastTouchedPad());
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                #endif
                break;

                case true:
                #ifdef MODULE_LEDS
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
                #endif
                break;

            }
            break;

            case false:
            if (!modifierActive || pads.isUserScale(pads.getActivePreset()))   {

                //shift entire octave

                //shift all notes up or down
                if (!state)    {

                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    uint8_t activeOctave = pads.getActiveOctave();
                    #ifdef MODULE_LCD
                        display.displayNoteChange(shiftResult, octaveChange, normalizeOctave(activeOctave));
                    #endif
                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                    #endif

                    }   else {

                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                    #endif

                }

            }   else {

                //shift single note, but only in predefined presets
                if (state)    {

                    if (pads.isPadPressed(pads.getLastTouchedPad()))   {

                        #ifdef MODULE_LCD
                            display.displayEditModeNotAllowed(padNotReleased);
                        #endif

                        return;

                    }

                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                    #endif

                    //stop button is modifier
                    //disable it on release
                    buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
                    changeOutput_t shiftResult = pads.shiftNote(direction);
                    #ifdef MODULE_LCD
                        display.displayNoteChange(shiftResult, noteShift, direction);
                    #endif

                }   else {

                    #ifdef MODULE_LEDS
                        direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);
                    #endif

                }

            }
            break;

        }

    }

}

bool Buttons::modifierEnabled() {

    return modifierActive;

}

Buttons buttons;
#endif