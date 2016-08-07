#include "../../Modules.h"

#ifdef MODULE_LEDS

#ifndef LEDS_H_
#define LEDS_H_

#include "../../hardware/pins/Pins.h"
#include "../../Types.h"
#include "LEDsettings.h"

class LEDs  {

    public:
    LEDs();
    void init();

    void allLEDsOn();
    void allLEDsOff();

    void displayActiveNoteLEDs(bool padEditMode = false, uint8_t padNumber = 0);

    void setLEDstate(uint8_t ledNumber, ledState_t state);
    ledState_t getLEDstate(uint8_t ledNumber);
    void setFadeSpeed(uint8_t speed);

    uint8_t getLEDnumberFromTonic(note_t note);
    void tonicLEDsOff();
    void setNoteLEDstate(note_t note, ledState_t state);
    ledState_t getTonicLEDstate(note_t note);

    private:

};

#endif

extern LEDs leds;
#endif