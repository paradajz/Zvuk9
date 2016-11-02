#pragma once
#include "../pads/Pads.h"

#include "../leds/LEDs.h"
#include "../../core/Core.h"

typedef enum
{
    transportStop,
    transportPlay,
    transportRecordOn,
    transportRecordOff
} transportControl_t;

typedef enum
{
    transportMMC,
    transportCC,
    transportMMC_CC
} transportControlType_t;

class Buttons
{
    public:
    Buttons();
    void init();
    void update();
    void enable(int8_t buttonID = -1);
    void disable(int8_t buttonID = -1);

    void setTransportControlType(transportControlType_t type);
    transportControlType_t getTransportControlType();

    //getters
    bool getButtonState(uint8_t buttonNumber);

    private:
    //callbacks
    void handleOnOffEvent(uint8_t buttonNumber, bool state);
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(note_t note, bool state);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    void processButton(uint8_t buttonNumber, bool state);
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);
    void setButtonState(uint8_t buttonNumber, uint8_t state);

    //getters
    note_t getTonicFromButton(uint8_t buttonNumber);
    uint8_t getLEDnumberFromButton(uint8_t buttonNumber);

    //button-to-led mapping
    void mapButtonsToLEDs();

    uint32_t mcpData;                                       //store data from all expander inputs
    uint8_t previousButtonState[MAX_NUMBER_OF_BUTTONS];     //debounce status
    uint32_t lastButtonDataPress;                           //previous button value
    uint32_t lastCheckTime;                                 //last time buttons were checked
    bool buttonEnabled[MAX_NUMBER_OF_BUTTONS];
    bool processingEnabled;
    uint32_t userMenuTimeout;
    transportControlType_t transportControlType;
};

extern Buttons buttons;
