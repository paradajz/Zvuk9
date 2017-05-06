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
#include "../encoders/Encoders.h"

extern void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

Buttons::Buttons()
{
    //default constructor
    lastCheckTime               = 0;
    processingEnabled           = true;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        buttonEnabled[i] = true;
    }
}

void Buttons::init()
{
    mapButtonsToNotes();
    initHandlers_buttons();
    uint32_t currentTime = rTimeMs();
    processingEnabled = false;
    transportControlType = (transportControlType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID);

    //read buttons for 0.1 seconds
    do
    {
        //read all buttons without activating event handlers
        update();
    }
    while ((rTimeMs() - currentTime) < 100);

    if (getButtonState(BUTTON_PROGRAM_ENC) && getButtonState(BUTTON_PRESET_ENC))
    {
        menu.displayMenu(serviceMenu);
        disable();
    }
    else
    {
        processingEnabled = true;
    }

    processingEnabled = true;
}

bool Buttons::getButtonState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return bitRead(buttonPressed[arrayIndex], buttonIndex);
}

void Buttons::setButtonState(uint8_t buttonID, bool state)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    bitWrite(buttonPressed[arrayIndex], buttonIndex, state);
}

void Buttons::processButton(uint8_t buttonID, uint8_t state)
{
    bool debounced = buttonDebounced(buttonID, state);

    if (debounced)
    {
        state = state == 0xFF;

        //if button state is same as last one, do nothing
        //act on change only
        if (state == getButtonState(buttonID))
            return;

        //update previous button state with current one
        setButtonState(buttonID, state);
        lastPressedButton = buttonID;

        if (processingEnabled)
        {
            (*buttonHandler[buttonID])(buttonID, state);

            //resume button processing
            if (!buttonEnabled[buttonID] && !getButtonState(buttonID))
            {
                buttonEnabled[buttonID] = true;
            }
        }
    }
}

void Buttons::update()
{
    uint8_t buttonState;

    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
    {
        if (buttonHandler[i] == NULL)
            continue;

        buttonState = board.getButtonState(i);
        processButton(i, buttonState);
    }
}

bool Buttons::buttonDebounced(uint8_t buttonID, uint8_t state)
{
    return ((state == 0xFF) || (state == BUTTON_DEBOUNCE_COMPARE));
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

uint8_t Buttons::getLastPressedButton()
{
    return lastPressedButton;
}

void Buttons::setModifierState(bool state)
{
    modifierActive = state;
}

bool Buttons::getModifierState()
{
    return modifierActive;
}

Buttons buttons;
