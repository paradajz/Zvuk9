#include "Buttons.h"

#ifdef BUTTONS_H_

#include "../../hardware/i2c/i2c_master.h"
#include "../../hardware/reset/Reset.h"

#include <util/delay.h>

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         5
#define USER_MENU_TIMEOUT           1500

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
    processingEnabled           = true;

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
    write_I2C_reg(expanderAddress[0], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[0], 0x05, 0x00);              //IOCON=0x00 if BANK=1
    write_I2C_reg(expanderAddress[1], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[1], 0x05, 0x00);              //IOCON=0x00 if BANK=1

    write_I2C_reg(expanderAddress[0], iodirAddress[0], 0xFF);   //expander 1, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[0], iodirAddress[1], 0xFF);   //expander 1, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[1], iodirAddress[0], 0xFF);   //expander 2, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[1], iodirAddress[1], 0xFF);   //expander 2, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[0], gppuAddress[0], 0xFF);    //expander 1, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[0], gppuAddress[1], 0xFF);    //expander 1, turn on pull-ups, PORTB

    write_I2C_reg(expanderAddress[1], gppuAddress[0], 0xFF);    //expander 2, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[1], gppuAddress[1], 0xFF);    //expander 2, turn on pull-ups, PORTB

    uint32_t currentTime = rTimeMillis();

    #ifdef MODULE_LCD
    processingEnabled = false;

    //read buttons for 0.1 seconds
    do {

        //read all buttons without activating event handlers
        update();

    }   while ((rTimeMillis() - currentTime) < 100);

    if (getButtonState(BUTTON_TRANSPORT_PLAY) && getButtonState(BUTTON_TRANSPORT_STOP)) {

        menu.displayMenu(serviceMenu);
        disable();

    }   else processingEnabled = true;
    #endif

}

void Buttons::update()    {

    if (!((rTimeMillis() - lastCheckTime) > EXPANDER_CHECK_TIME)) return;

    if (readStates())   {

        for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

            //invert button state because of pull-ups
            uint8_t state = !((mcpData >> i) & 0x01);
            bool debounced = buttonDebounced(i, state);

            if (debounced) {

                //if button state is same as last one, do nothing
                //act on change only
                if (state == getButtonState(i)) continue;

                //update previous button state with current one
                setButtonState(i, state);
                if (processingEnabled) processButton(i, state);

            }

        }

        mcpData = 0;

    }

    //check split button for entering into user menu
    if (getButtonState(BUTTON_ON_OFF_SPLIT) && buttonEnabled[BUTTON_ON_OFF_SPLIT])   {

        //measure the time the button is pressed
        if (!userMenuTimeout) userMenuTimeout = rTimeMillis();
        else if (((rTimeMillis() - userMenuTimeout) > USER_MENU_TIMEOUT) && !menu.menuDisplayed()) {

            buttonEnabled[BUTTON_ON_OFF_SPLIT] = false;
            userMenuTimeout = 0;
            #ifdef MODULE_LCD
            menu.displayMenu(userMenu);
            #endif
            #if MODE_SERIAL > 0
                printf("Entering user menu\n");
            #endif
            //disable buttons while in menu
            buttons.disable();

        }

    }  else userMenuTimeout = 0;

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

        switch (buttonNumber)   {

            case BUTTON_ON_OFF_AFTERTOUCH:
            case BUTTON_ON_OFF_NOTES:
            case BUTTON_ON_OFF_X:
            case BUTTON_ON_OFF_Y:
            case BUTTON_ON_OFF_SPLIT:
            handleOnOffEvent(buttonNumber, state);
            break;

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
            handleTonicEvent(note, state);
            break;

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

void Buttons::enable(int8_t buttonID)  {

    if (buttonID == -1)
        processingEnabled = true;

    else buttonEnabled[buttonID] = true;

}

void Buttons::disable(int8_t buttonID) {

    if (buttonID == -1)
        processingEnabled = false;

    else buttonEnabled[buttonID] = false;

}

#endif