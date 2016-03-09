#include "Buttons.h"
#include "hardware/i2c/i2c_master.h"

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         10

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
    callbackEnableState         = 0xFFFFFFFF;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
        previousButtonState[i] = buttonDebounceCompare;

    sendOnOffPressCallback          = NULL;
    sendOctaveUpDownPressCallback   = NULL;
    sendTonicCallback               = NULL;
    sendTransportControlCallback    = NULL;

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

}

void Buttons::update()    {

    if (!((newMillis() - lastCheckTime) > EXPANDER_CHECK_TIME)) return;

     if (readStates())   {

         for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

             //invert button state because of pull-ups
             uint8_t state = !((mcpData >> i) & 0x01);

             if (buttonDebounced(i, state))
                processButton(i, state);

         }   mcpData = 0;

     }

     lastCheckTime = newMillis();

}

bool Buttons::getButtonPressed(uint8_t buttonNumber) {

    return getPreviousButtonState(buttonNumber);

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

void Buttons::processButton(uint8_t buttonNumber, uint8_t state)    {

    if (state == getPreviousButtonState(buttonNumber)) return;

    //update previous button state with current one
    setPreviousButtonState(buttonNumber, state);

    //only process button if its reading is different from last one

    if (state)    {

        switch(buttonNumber)    {

            case BUTTON_ON_OFF_AFTERTOUCH:
            case BUTTON_ON_OFF_NOTES:
            case BUTTON_ON_OFF_X:
            case BUTTON_ON_OFF_Y:
            case BUTTON_ON_OFF_SPLIT:
            if (callbackEnabled(buttonNumber))
                sendOnOffPressCallback(buttonNumber, (buttonState_t)state);
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
            tonic_t _tonic = getTonicFromButton(buttonNumber);
            if (callbackEnabled(buttonNumber))
                sendTonicCallback(_tonic);
            break;

        }

    }

    switch (buttonNumber)   {

        case BUTTON_OCTAVE_DOWN:
        if (callbackEnabled(buttonNumber))
            sendOctaveUpDownPressCallback(false, state);
        break;

        case BUTTON_OCTAVE_UP:
        if (callbackEnabled(buttonNumber))
            sendOctaveUpDownPressCallback(true, state);
        break;

        case BUTTON_TRANSPORT_STOP:
        case BUTTON_TRANSPORT_PLAY:
        case BUTTON_TRANSPORT_RECORD:
        if (callbackEnabled(buttonNumber))
            sendTransportControlCallback(buttonNumber, (buttonState_t)state);
        break;

    }

    //resume all callbacks
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++) {

        if (!callbackEnabled(i) && !getButtonPressed(i)) {

            enableCallback(i);

        }

    }

}

bool Buttons::getPreviousButtonState(uint8_t buttonNumber) {

    return bitRead(lastButtonDataPress, buttonNumber);

}

void Buttons::setPreviousButtonState(uint8_t buttonNumber, uint8_t state) {

    bitWrite(lastButtonDataPress, buttonNumber, state);

}

tonic_t Buttons::getTonicFromButton(uint8_t buttonNumber)   {

    switch(buttonNumber)    {

        case BUTTON_NOTE_C:
        return tonicC;

        case BUTTON_NOTE_C_SHARP:
        return tonicCSharp;

        case BUTTON_NOTE_D:
        return tonicD;

        case BUTTON_NOTE_D_SHARP:
        return tonicDSharp;

        case BUTTON_NOTE_E:
        return tonicE;

        case BUTTON_NOTE_F:
        return tonicF;

        case BUTTON_NOTE_F_SHARP:
        return tonicFSharp;

        case BUTTON_NOTE_G:
        return tonicG;

        case BUTTON_NOTE_G_SHARP:
        return tonicGSharp;

        case BUTTON_NOTE_A:
        return tonicA;

        case BUTTON_NOTE_A_SHARP:
        return tonicASharp;

        case BUTTON_NOTE_B:
        return tonicB;

    }   return tonicInvalid;   //impossible case

}

//callbacks

void Buttons::setHandleOnOffPress(void (*fptr)(uint8_t buttonNumber, buttonState_t state))   {

    sendOnOffPressCallback = fptr;

}

void Buttons::setHandleOctaveUpDownPress(void (*fptr)(uint8_t direction, bool state))   {

    sendOctaveUpDownPressCallback = fptr;

}

void Buttons::setHandleTonic(void (*fptr)(tonic_t _tonic))   {

    sendTonicCallback = fptr;

}

void Buttons::setHandleTransportControlCallback(void (*fptr)(uint8_t buttonNumber, buttonState_t state))  {

    sendTransportControlCallback = fptr;

}

bool Buttons::callbackEnabled(uint8_t buttonNumber) {

    return bitRead(callbackEnableState, buttonNumber);

}

void Buttons::enableCallback(uint8_t buttonNumber)  {

    bitWrite(callbackEnableState, buttonNumber, 1);

}

void Buttons::pauseCallback(uint8_t buttonNumber)  {

    bitWrite(callbackEnableState, buttonNumber, 0);

}

Buttons buttons;