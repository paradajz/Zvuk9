#include "Buttons.h"

#ifdef BUTTONS_H_

#include "../../hardware/i2c/i2c_master.h"
#include "../../hardware/reset/Reset.h"

#include <util/delay.h>

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         10
#define STOP_DISABLE_TIMEOUT        500
#define RESET_ENABLE_TIMEOUT        4000

//#define SOFT_REBOOT                 1

//MCP23017 address bytes
const uint8_t expanderAddress[] = { 0x21, 0x20 };   //chip address
const uint8_t gpioAddress[]     = { 0x12, 0x13 };   //input/output
const uint8_t iodirAddress[]    = { 0x00, 0x01 };   //port A/port B
const uint8_t gppuAddress[]     = { 0x0C, 0x0D };   //interrupt/pull-up

//shift new values from button in this variable
//if it's 0xFF or buttonDebounceCompare, its reading is stable
const uint8_t buttonDebounceCompare = 0b11110000;

bool processingEnabled = true;

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
    processingEnabled = false;

    //read buttons for 0.1 seconds
    do {

        //read all buttons without activating event handlers
        update();

    }   while ((rTimeMillis() - currentTime) < 100);

    if (buttons.getButtonState(BUTTON_TRANSPORT_PLAY) && buttons.getButtonState(BUTTON_TRANSPORT_STOP)) {

        //we should activate service menu now
        #if MODE_SERIAL > 0
            printf("Activating service menu\n");
        #endif

        #ifdef MODULE_LCD
            menu.displayMenu(serviceMenu);
        #endif

        buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
        buttonEnabled[BUTTON_TRANSPORT_PLAY] = false;

    }   else processingEnabled = true;

}

void Buttons::update()    {

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
        if (!stopDisableTimeout || !pads.allPadsReleased()) stopDisableTimeout = rTimeMillis();
        else if ((rTimeMillis() - stopDisableTimeout) > STOP_DISABLE_TIMEOUT) {

            buttonEnabled[BUTTON_TRANSPORT_STOP] = false;
            stopDisableTimeout = 0;
            #ifdef MODULE_LCD
            display.displayModifierEnabled();
            #endif
            #if MODE_SERIAL > 0
                printf("Modifier active\n");
            #endif
            modifierActive = true;

            #ifdef MODULE_LEDS
            if ((!pads.editModeActive()) && (pads.isPredefinedScale(pads.getActivePreset()))) {

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

#endif