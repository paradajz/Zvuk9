#ifndef TIMEROBJECT_H_
#define TIMEROBJECT_H_

#include "Arduino.h"
#include "../pins/Pins.h"
#include "../../Types.h"

void newDelay(uint32_t);
uint32_t newMillis();

class TimerObject {

    public:
    TimerObject();
    void init();
    ledIntensity_t getLEDstate(uint8_t ledNumber);
    void setLEDstate(uint8_t ledNumber, ledIntensity_t intensity);
    int8_t getEncoderState(uint8_t encoderID);
    void setPWMsteps(uint8_t steps);
    void debugInfo();

    private:
    void setupEncoder(uint8_t id, uint8_t, uint8_t);

};

extern TimerObject timers;

#endif
