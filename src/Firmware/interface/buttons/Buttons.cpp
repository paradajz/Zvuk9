#include "Buttons.h"

#include "Config.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#ifdef NDEBUG
#include "../../midi/MIDI.h"
#endif
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#include "handlers/Handlers.h"

//shift new values from button in this variable
//if it's 0xFF or buttonDebounceCompare, its reading is stable
const uint8_t buttonDebounceCompare = 0b11110000;

extern void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

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
    mapButtonsToNotes();
    initHandlers_buttons();
    uint32_t currentTime = rTimeMs();
    processingEnabled = false;
    transportControlType = (transportControlType_t)db.read(CONF_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID);

    //read buttons for 0.1 seconds
    do
    {
        //read all buttons without activating event handlers
        update();
    }
    while ((rTimeMs() - currentTime) < 100);

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
    if (!((rTimeMs() - lastCheckTime) > EXPANDER_CHECK_TIME))
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
            {
                if (buttonHandler[i] != NULL)
                    (*buttonHandler[i])(i, state);

                //resume button processing
                if (!buttonEnabled[i] && !getButtonState(i))
                {
                    buttonEnabled[i] = true;
                }
            }
        }
    }

    //check split button for entering into user menu
    if (getButtonState(BUTTON_ON_OFF_SPLIT) && buttonEnabled[BUTTON_ON_OFF_SPLIT])
    {
        if (!pads.editModeActive())
        {
            //measure the time the button is pressed
            if (!userMenuTimeout)
            {
                userMenuTimeout = rTimeMs();
            }
            else if (((rTimeMs() - userMenuTimeout) > USER_MENU_TIMEOUT) && !menu.menuDisplayed())
            {
                userMenuTimeout = 0;
                menu.displayMenu(userMenu);
                #ifdef DEBUG
                printf_P(PSTR("Entering user menu\n"));
                #endif
                //disable buttons while in menu
                buttons.disable();
                //turn off blinky led
                leds.setLEDstate(LED_ON_OFF_SPLIT, leds.getLEDstate(LED_ON_OFF_SPLIT));

            }
        }
    }
    else
    {
        userMenuTimeout = 0;
    }

    lastCheckTime = rTimeMs();
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

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)
{
    return buttonToNoteArray[buttonNumber];
}

void Buttons::setTransportControlType(transportControlType_t type)
{
    transportControlType = type;
}

transportControlType_t Buttons::getTransportControlType()
{
    return transportControlType;
}

bool Buttons::getButtonEnableState(uint8_t buttonID)
{
    return buttonEnabled[buttonID];
}

Buttons buttons;
