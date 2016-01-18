#ifndef LEDS_H_
#define LEDS_H_

#include "Arduino.h"
#include "hardware/pins/HardwareIDs.h"
#include "Types.h"
#include "LEDsettings.h"

class LEDs  {

    public:
    LEDs();
    void init();

    void allLEDsOn();
    void allLEDsOff();

    void setLEDstate(uint8_t ledNumber, ledIntensity state);
    ledIntensity getLEDstate(uint8_t ledNumber);
    void setFadeSpeed(uint8_t speed);

    uint8_t getLEDnumberFromTonic(tonic _tonic);
    void tonicLEDsOff();
    void setTonicLEDstate(tonic _tonic, ledIntensity state);
    ledIntensity getTonicLEDstate(tonic _tonic);

    private:

};

#endif

extern LEDs leds;
