#include "Buttons.h"

#include "Config.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#ifdef USE_USB_MIDI
#include "../../midi/src/MIDI.h"
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

    //enable all buttons
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS/8+1; i++)
        buttonEnabled[i] = 0xFF;
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
        menu.show(serviceMenu);
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
            if (!getButtonEnableState(buttonID) && !getButtonState(buttonID))
            {
                enable(buttonID);
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

    if (getMIDIchannelEnc() && (display.getActiveTextType() == lcdtext_still))
        setMIDIchannelEnc(false);
}

bool Buttons::buttonDebounced(uint8_t buttonID, uint8_t state)
{
    return ((state == 0xFF) || (state == BUTTON_DEBOUNCE_COMPARE));
}

void Buttons::enable(int8_t buttonID)
{
    if (buttonID == -1)
    {
        processingEnabled = true;
    }
    else
    {
        uint8_t arrayIndex = buttonID/8;
        uint8_t buttonIndex = buttonID - 8*arrayIndex;

        bitWrite(buttonEnabled[arrayIndex], buttonIndex, true);
    }
}

void Buttons::disable(int8_t buttonID)
{
    if (buttonID == -1)
    {
        processingEnabled = false;
    }
    else
    {
        uint8_t arrayIndex = buttonID/8;
        uint8_t buttonIndex = buttonID - 8*arrayIndex;

        bitWrite(buttonEnabled[arrayIndex], buttonIndex, false);
    }
}

bool Buttons::getButtonEnableState(uint8_t buttonID)
{
    uint8_t arrayIndex = buttonID/8;
    uint8_t buttonIndex = buttonID - 8*arrayIndex;

    return bitRead(buttonEnabled[arrayIndex], buttonIndex);
}

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)
{
    return (note_t)buttonToNoteArray[buttonNumber];
}

void Buttons::setTransportControlType(transportControlType_t type)
{
    transportControlType = type;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_TRANSPORT_CC_ID, type);
}

transportControlType_t Buttons::getTransportControlType()
{
    return transportControlType;
}

uint8_t Buttons::getLastPressedButton()
{
    return lastPressedButton;
}

void Buttons::setMIDIchannelEnc(bool state)
{
    midiChannelEncState = state;
}

bool Buttons::getMIDIchannelEnc()
{
    return midiChannelEncState;
}

Buttons buttons;
