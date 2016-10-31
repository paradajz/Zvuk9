#include "Buttons.h"

#include <util/delay.h>
#include "../lcd/menu/Menu.h"
#include "../../eeprom/Configuration.h"

//time after which expanders are checked in ms
#define EXPANDER_CHECK_TIME         5
#define USER_MENU_TIMEOUT           3000

//shift new values from button in this variable
//if it's 0xFF or buttonDebounceCompare, its reading is stable
const uint8_t buttonDebounceCompare = 0b11110000;

Buttons::Buttons()
{
    //default constructor
    lastCheckTime               = 0;
    lastButtonDataPress         = 0;
    mcpData                     = 0;
    processingEnabled           = true;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        previousButtonState[i] = buttonDebounceCompare;
        buttonEnabled[i] = true;
    }
}

void Buttons::init()
{
    mapButtonsToLEDs();
    uint32_t currentTime = rTimeMillis();
    processingEnabled = false;
    transportCCenabled_ = configuration.readParameter(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC);

    //read buttons for 0.1 seconds
    do
    {
        //read all buttons without activating event handlers
        update();
    }
    while ((rTimeMillis() - currentTime) < 100);

    if (getButtonState(BUTTON_TRANSPORT_PLAY) && getButtonState(BUTTON_TRANSPORT_STOP))
    {
        menu.displayMenu(serviceMenu);
        disable();
    }
    else
    {
        processingEnabled = true;
    }
}

void Buttons::update()
{
    if (!((rTimeMillis() - lastCheckTime) > EXPANDER_CHECK_TIME))
        return;

    if (!board.buttonDataAvailable())
        return;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        bool state = board.getButtonState(i);
        bool debounced = buttonDebounced(i, state);

        if (debounced)
        {
            //if button state is same as last one, do nothing
            //act on change only
            if (state == getButtonState(i))
                continue;
            //update previous button state with current one
            setButtonState(i, state);
            if (processingEnabled)
                processButton(i, state);
        }
    }

    //check split button for entering into user menu
    if (getButtonState(BUTTON_ON_OFF_SPLIT) && buttonEnabled[BUTTON_ON_OFF_SPLIT])
    {
        //measure the time the button is pressed
        if (!userMenuTimeout)
        {
            userMenuTimeout = rTimeMillis();
        }
        else if (((rTimeMillis() - userMenuTimeout) > USER_MENU_TIMEOUT) && !menu.menuDisplayed())
        {
            userMenuTimeout = 0;
            menu.displayMenu(userMenu);
            #ifdef DEBUG
            printf("Entering user menu\n");
            #endif
            //disable buttons while in menu
            buttons.disable();
            //turn off blinky led
            leds.setLEDstate(LED_ON_OFF_SPLIT, leds.getLEDstate(LED_ON_OFF_SPLIT), false);

        }
    }
    else
    {
        userMenuTimeout = 0;
    }

    lastCheckTime = rTimeMillis();
}

bool Buttons::getButtonState(uint8_t buttonNumber)
{
    return bitRead(lastButtonDataPress, buttonNumber);
}

bool Buttons::buttonDebounced(uint8_t buttonNumber, uint8_t state)
{
    //shift new button reading into previousButtonState
    previousButtonState[buttonNumber] = (previousButtonState[buttonNumber] << 1) | state | buttonDebounceCompare;

    //if button is debounced, return true
    return ((previousButtonState[buttonNumber] == buttonDebounceCompare) || (previousButtonState[buttonNumber] == 0xFF));
}

void Buttons::processButton(uint8_t buttonNumber, bool state)
{
    if (buttonEnabled[buttonNumber])
    {
        switch (buttonNumber)
        {
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
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        if (!buttonEnabled[i] && !getButtonState(i))
        {
            buttonEnabled[i] = true;
            //reset blinking
            uint8_t ledNumber = getLEDnumberFromButton(i);
            leds.setLEDstate(ledNumber, leds.getLEDstate(ledNumber), false);
        }
    }
}

void Buttons::setButtonState(uint8_t buttonNumber, uint8_t state)
{
    bitWrite(lastButtonDataPress, buttonNumber, state);
}

void Buttons::enable(int8_t buttonID)
{
    if (buttonID == -1)
        processingEnabled = true;
    else
        buttonEnabled[buttonID] = true;
}

void Buttons::disable(int8_t buttonID)
{
    if (buttonID == -1)
        processingEnabled = false;
    else
        buttonEnabled[buttonID] = false;
}
