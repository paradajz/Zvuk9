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
    void enable(int8_t buttonID = -1);
    void disable(int8_t buttonID = -1);

    //getters
    bool getButtonState(uint8_t buttonNumber);

    private:
    //callbacks
    void handleOnOffEvent(uint8_t buttonNumber, bool state);
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(note_t note, bool state);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    bool readStates();
    void processButton(uint8_t buttonNumber, bool state);
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);
    void setButtonState(uint8_t buttonNumber, uint8_t state);

    //getters
    note_t getTonicFromButton(uint8_t buttonNumber);

    //menu
    void checkMenu(menuType_t type);

    uint32_t mcpData;                                       //store data from all expander inputs
    uint8_t previousButtonState[MAX_NUMBER_OF_BUTTONS];     //debounce status
    uint32_t lastButtonDataPress;                           //previous button value
    uint32_t lastCheckTime;                                 //last time buttons were checked
    bool buttonEnabled[MAX_NUMBER_OF_BUTTONS];
    bool processingEnabled;
    uint32_t userMenuTimeout;

};

extern Buttons buttons;

#endif
#endif