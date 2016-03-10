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
    void update();

    //getters
    bool getButtonPressed(uint8_t buttonNumber);

    //setters
    void pauseCallback(uint8_t buttonNumber);

    //callbacks
    void setHandleOnOffPress(void (*fptr)(uint8_t buttonNumber, buttonState_t state));
    void setHandleTransportControlCallback(void (*fptr)(uint8_t buttonNumber, buttonState_t state));
    void setHandleOctaveUpDownPress(void (*fptr)(uint8_t direction, bool state));
    void setHandleTonic(void (*fptr)(tonic_t _tonic));

    private:
    //callbacks
    void (*sendOnOffPressCallback)(uint8_t buttonNumber, buttonState_t state);
    void (*sendTransportControlCallback)(uint8_t buttonNumber, buttonState_t state);
    void (*sendOctaveUpDownPressCallback)(uint8_t direction, bool state);
    void (*sendTonicCallback)(tonic_t _tonic);
    bool callbackEnabled(uint8_t buttonNumber);
    void enableCallback(uint8_t buttonNumber);

    //read/debounce handling
    bool readStates();
    void processButton(uint8_t buttonNumber, uint8_t state);
    bool buttonDebounced(uint8_t buttonNumber, uint8_t state);
    bool getPreviousButtonState(uint8_t buttonNumber);
    void setPreviousButtonState(uint8_t buttonNumber, uint8_t state);

    //send MIDI transport control
    void sendTransportControl(uint8_t buttonNumber, buttonState_t state);

    //getters
    tonic_t getTonicFromButton(uint8_t buttonNumber);

    uint32_t mcpData;                                       //store data from all expander inputs
    uint8_t previousButtonState[MAX_NUMBER_OF_BUTTONS];     //debounce status
    uint32_t lastButtonDataPress;                           //previous button value
    uint32_t lastCheckTime;                                 //last time buttons were checked
    uint32_t callbackEnableState;                           //enable/disable callback for certain button


};

extern Buttons buttons;

#endif /* BUTTONS_H_ */
