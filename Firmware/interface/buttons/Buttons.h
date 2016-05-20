/*

Button control using two MPC23017 expanders.

*/

#include "../../Modules.h"

#ifdef MODULE_BUTTONS
#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "../../Debug.h"
#include "../../midi/MIDI_parameters.h"
#include "../../Types.h"
#include "../pads/Pads.h"

#ifdef MODULE_LCD
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#endif

#ifdef MODULE_LEDS
#include "../leds/LEDs.h"
#endif

#define MAX_NUMBER_OF_BUTTONS   32

class Buttons   {

    public:
    Buttons();
    void init();
    void update();
    void enable();
    void disable();

    //getters
    bool getButtonState(uint8_t buttonNumber);
    bool modifierEnabled();

    private:
    //callbacks
    void handleOnOffEvent(uint8_t buttonNumber);
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(note_t note);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    bool readStates();
    void processButton(uint8_t buttonNumber, bool state);
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);
    void setButtonState(uint8_t buttonNumber, uint8_t state);

    //send MIDI transport control
    void sendTransportControl(uint8_t buttonNumber, bool state);

    //getters
    note_t getTonicFromButton(uint8_t buttonNumber);

    uint32_t mcpData;                                       //store data from all expander inputs
    uint8_t previousButtonState[MAX_NUMBER_OF_BUTTONS];     //debounce status
    uint32_t lastButtonDataPress;                           //previous button value
    uint32_t lastCheckTime;                                 //last time buttons were checked
    bool buttonEnabled[MAX_NUMBER_OF_BUTTONS];
    uint32_t stopDisableTimeout;
    uint32_t resetActivationTimeout;
    bool modifierActive;
    bool processingEnabled;

};

extern Buttons buttons;

#endif
#endif