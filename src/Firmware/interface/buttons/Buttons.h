#pragma once

#include "DataTypes.h"
#include "../pads/Pads.h"
#include "../leds/LEDs.h"

class Buttons
{
    public:
    Buttons();
    void init();
    void update();
    void enable(int8_t buttonID = -1);
    void disable(int8_t buttonID = -1);
    bool getButtonEnableState(uint8_t buttonID);

    void setTransportControlType(transportControlType_t type);
    transportControlType_t getTransportControlType();

    //getters
    note_t getTonicFromButton(uint8_t buttonNumber);
    uint8_t getLastPressedButton();

    bool getButtonState(uint8_t buttonID);

    void setButtonState(uint8_t buttonID, bool state);
    void setMIDIchannelEnc(bool state);
    bool getMIDIchannelEnc();

    private:
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(note_t note, bool state);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);

    //button-to-led mapping
    void mapButtonsToNotes();

    void processButton(uint8_t button, uint8_t state);

    uint32_t                lastCheckTime;
    uint8_t                 buttonEnabled[MAX_NUMBER_OF_BUTTONS/8+1];
    bool                    processingEnabled;
    uint32_t                userMenuTimeout;
    transportControlType_t  transportControlType;
    uint8_t                 buttonToNoteArray[MAX_NUMBER_OF_BUTTONS];
    uint8_t                 lastPressedButton;
    uint8_t                 buttonPressed[MAX_NUMBER_OF_BUTTONS/8+1];
    bool                    midiChannelEncState;
};

extern Buttons buttons;
