#ifndef LEDS_H_
#define LEDS_H_

#include "Arduino.h"
#include "hardware/pins/Pins.h"
#include "Types.h"
#include "LEDsettings.h"

class LEDs  {

    public:
    LEDs();
    void init();

    void allLEDsOn();
    void allLEDsOff();

    void setLEDstate(uint8_t ledNumber, ledIntensity_t state);
    ledIntensity_t getLEDstate(uint8_t ledNumber);
    void setFadeSpeed(uint8_t speed);

    uint8_t getLEDnumberFromTonic(tonic_t _tonic);
    void tonicLEDsOff();
    void setNoteLEDstate(tonic_t _tonic, ledIntensity_t state);
    ledIntensity_t getTonicLEDstate(tonic_t _tonic);

    private:

};

#endif

extern LEDs leds;
