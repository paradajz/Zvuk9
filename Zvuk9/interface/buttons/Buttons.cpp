#include "Buttons.h"
#include "../../hardware/i2c/i2c_master.h"
#include "../pads/Pads.h"
#include "../lcd/LCD.h"
#include "../leds/LEDs.h"
#include "../../midi/MIDI.h"
#include "../../hardware/reset/Reset.h"
#include "../lcd/menu/Menu.h"

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         10
#define STOP_DISABLE_TIMEOUT        500
#define RESET_ENABLE_TIMEOUT        3000

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

    uint32_t currentTime = newMillis();

    //read buttons for 0.1 seconds
    do {

        //read all buttons without activating event handlers
        buttons.update(false);

    }   while ((newMillis() - currentTime) < 100);

    if (buttons.getButtonState(BUTTON_TRANSPORT_PLAY) && buttons.getButtonState(BUTTON_TRANSPORT_STOP)) {

        //we should activate service menu now
        #if MODE_SERIAL > 0
            Serial.println(F("Activating user menu"));
        #endif

        menu.displayMenu(serviceMenu);
        buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
        buttonEnabled[BUTTON_TRANSPORT_PLAY] = false;

    }

}

void Buttons::update(bool processingEnabled)    {

    if (!((newMillis() - lastCheckTime) > EXPANDER_CHECK_TIME)) return;

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
        if (!stopDisableTimeout) stopDisableTimeout = newMillis();
        else if ((newMillis() - stopDisableTimeout) > STOP_DISABLE_TIMEOUT) {

            buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
            stopDisableTimeout = 0;
            lcDisplay.displayUserMessage(2, "Stop disabled", true);

        }

     }  else stopDisableTimeout = 0;

     #if SOFT_REBOOT > 0
         if (buttons.getButtonState(BUTTON_TRANSPORT_PLAY) && buttonEnabled[BUTTON_TRANSPORT_PLAY])    {

            if (!resetActivationTimeout) resetActivationTimeout = newMillis();
            else if ((newMillis() - resetActivationTimeout) > RESET_ENABLE_TIMEOUT)   {

                lcDisplay.displayUserMessage(1, "Press STOP to reset");
                leds.allLEDsOn();
                lcDisplay.update();

                do {

                    buttons.update();

                }   while (!getButtonState(BUTTON_TRANSPORT_STOP));

                leds.allLEDsOff();
                newDelay(50);
                _reboot_Teensyduino_();

            }

         }  else resetActivationTimeout = 0;
     #endif

     lastCheckTime = newMillis();

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

                //remove modifier message
                lcDisplay.clearMessage(true);
                //restore led states
                leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);
                leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityOff);

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
        pads.aftertouchOnOff();
        lcdMessageType = featureAftertouch;
        ledNumber = LED_ON_OFF_AFTERTOUCH;
        if (pads.getAfterTouchSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
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
        pads.setSplit();
        lcdMessageType = featureSplit;
        ledNumber = LED_ON_OFF_SPLIT;
        ledState = pads.getSplitStateLEDvalue();
        break;

        default:
        return;

    }

    leds.setLEDstate(ledNumber, ledState);
    lcDisplay.displayOnOffMessage(lcdMessageType, pads.getSplitState(), (ledState == ledIntensityFull), lastTouchedPad+1);

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
                Serial.println(F("Transport Control Play"));
            #endif
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityFull);

        } else return;

        break;

        case BUTTON_TRANSPORT_STOP:
        if (!state)    {

            sysExArray[4] = 0x01;
            type = transportStop;
            #if MODE_SERIAL > 0
                Serial.println(F("Transport Control Stop"));
            #endif
            leds.setLEDstate(LED_TRANSPORT_PLAY, ledIntensityOff);
            leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);
            leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
            leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityOff);
            //force message clear without restoring state
            lcDisplay.clearMessage(true);

        } else {

            #if MODE_SERIAL > 0
                Serial.println(F("Modifier active"));
            #endif

            lcDisplay.displayUserMessage(1, "Modifier active", true);
            lcDisplay.displayUserMessage(2, "Stop enabled", true);
            leds.setLEDstate(LED_TRANSPORT_STOP, ledIntensityFull);

            if (!pads.editModeActive() && (pads.getActivePreset() < NUMBER_OF_PREDEFINED_SCALES)) {

                leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim);
                leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);

            }
            stopDisableTimeout = newMillis();
            return;

        }
        break;

        case BUTTON_TRANSPORT_RECORD:
        if (state) {

            ledIntensity_t recordState = leds.getLEDstate(LED_TRANSPORT_RECORD);
            if (recordState == ledIntensityFull) {

                sysExArray[4] = 0x07;
                recordState = ledIntensityOff;
                #if MODE_SERIAL > 0
                    Serial.println(F("Transport Control Record Stop"));
                #endif
                displayState = false;

                }   else if (recordState == ledIntensityOff) {

                sysExArray[4] = 0x06;
                recordState = ledIntensityFull;
                #if MODE_SERIAL > 0
                    Serial.println(F("Transport Control Record Start"));
                #endif
                displayState = true;

            }

            type = transportRecord;
            leds.setLEDstate(LED_TRANSPORT_RECORD, recordState);

        } else return;

        break;

    }

    #if MODE_SERIAL < 1
        midi.sendSysEx(sysExArray, SYS_EX_ARRAY_SIZE);
    #endif

    lcDisplay.displayTransportControlMessage(type, displayState);

}

void Buttons::handleTonicEvent(note_t note) {

    if (!pads.editModeActive())   {

        //don't allow change of tonic while pads are pressed
        for (int i=0; i<NUMBER_OF_PADS; i++)
        if (pads.getPadPressed(i))  {

            lcDisplay.displayUserMessage(1, "Release pad to");
            lcDisplay.displayUserMessage(2, "change tonic");
            return;

        }

        changeOutput_t result = pads.setTonic(note);

        if (result == outputChanged)
            leds.displayActiveNoteLEDs();

        note_t activeTonic = pads.getActiveTonic();
        lcDisplay.displayNoteChange(result, noteChange, activeTonic);

        }   else {

        //add note to pad
        uint8_t pad = pads.getLastTouchedPad();
        lcDisplay.displayPadEditResult(pads.assignPadNote(pad, note));
        pads.displayActivePadNotes(pad);
        leds.displayActiveNoteLEDs(true, pad);

    }

}

void Buttons::handleOctaveEvent(bool direction, bool state)   {

    if (buttons.getButtonState(BUTTON_OCTAVE_DOWN) && buttons.getButtonState(BUTTON_OCTAVE_UP))   {

        //try to enter pad edit mode

        if (pads.getActivePreset() >= NUMBER_OF_PREDEFINED_SCALES)    {

            pads.setEditMode(!pads.editModeActive());

            if (pads.editModeActive())  {

                //check if last touched pad is pressed
                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    pads.setEditMode(false);

                }   else {

                    //normally, this is called in automatically in Pads.cpp
                    //but on first occasion call it manually
                    #if MODE_SERIAL > 0
                        Serial.println(F("----------------------------------------"));
                        Serial.println(F("Pad edit mode"));
                        Serial.println(F("----------------------------------------"));
                    #endif
                    pads.setupPadEditMode(pads.getLastTouchedPad());

                }

            }   else pads.exitPadEditMode();

        }   else {

            lcDisplay.displayEditModeNotAllowed(noUserPreset);

        }

        //restore leds
        leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
        leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);
        //stop buttons temporarily
        buttons.pauseButton(BUTTON_OCTAVE_DOWN);
        buttons.pauseButton(BUTTON_OCTAVE_UP);

    }   else {

        bool editMode = pads.editModeActive();

        switch(editMode)    {

            case true:
            //do not shift octaves while pad is pressed
            if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                lcDisplay.displayEditModeNotAllowed(padNotReleased);

            }   else {

                switch(state)   {

                    case false:
                    pads.changeActiveOctave(direction);
                    lcDisplay.displayActiveOctave(normalizeOctave(pads.getActiveOctave()));
                    leds.displayActiveNoteLEDs(true, pads.getLastTouchedPad());
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                    break;

                    case true:
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                    break;

                }

            }
            break;

            case false:
            if (!buttons.getButtonState(BUTTON_TRANSPORT_STOP))   {

                //shift entire octave

                //shift all notes up or down
                if (!state)    {

                    changeOutput_t shiftResult = pads.shiftOctave(direction);
                    int8_t activeOctave = pads.getActiveOctave();
                    lcDisplay.displayNoteChange(shiftResult, octaveChange, activeOctave);
                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);

                }   else {

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);

                }

            }   else {

                //shift single note
                if (state)    {

                    if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                        lcDisplay.displayEditModeNotAllowed(padNotReleased);
                        return;

                    }

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);

                    //stop button is modifier
                    //disable it on release
                    buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
                    //clear message as well
                    //lcDisplay.clearMessage(true);
                    changeOutput_t shiftResult = pads.shiftNote(direction);
                    lcDisplay.displayNoteChange(shiftResult, noteUpOrDown, direction);

                }   else {

                    direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityDim) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityDim);

                }

            }
            break;

        }

    }

}

void Buttons::pauseButton(uint8_t buttonNumber) {

    buttonEnabled[buttonNumber] = false;

}

Buttons buttons;
