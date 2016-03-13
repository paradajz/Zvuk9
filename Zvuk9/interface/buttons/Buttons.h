/*

Button control using two MPC23017 expanders.

*/

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include "../../Debug.h"
#include "../../hardware/pins/Pins.h"
#include "../../midi/MIDI_parameters.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"

#define MAX_NUMBER_OF_BUTTONS   32

class Buttons   {

    public:
    Buttons();
    void init();
    void update(bool processingEnabled = true);
    void pauseButton(uint8_t buttonNumber);

    //getters
    bool getButtonPressed(uint8_t buttonNumber);

    private:
    //callbacks
    void handleOnOffEvent(uint8_t buttonNumber);
    void handleTransportControlEvent(uint8_t buttonNumber, bool state);
    void handleTonicEvent(tonic_t _tonic);
    void handleOctaveEvent(bool direction, bool state);

    //read/debounce handling
    bool readStates();
    void processButton(uint8_t buttonNumber, bool state);
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);
    bool getPreviousButtonState(uint8_t buttonNumber);
    void setPreviousButtonState(uint8_t buttonNumber, uint8_t state);

    //send MIDI transport control
    void sendTransportControl(uint8_t buttonNumber, bool state);

    //getters
    tonic_t getTonicFromButton(uint8_t buttonNumber);

    uint32_t mcpData;                                       //store data from all expander inputs
    uint8_t previousButtonState[MAX_NUMBER_OF_BUTTONS];     //debounce status
    uint32_t lastButtonDataPress;                           //previous button value
    uint32_t lastCheckTime;                                 //last time buttons were checked
    bool buttonEnabled[MAX_NUMBER_OF_BUTTONS];


};

extern Buttons buttons;

#endif /* BUTTONS_H_ */
