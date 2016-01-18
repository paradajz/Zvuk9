#ifndef TIMEROBJECT_H_
#define TIMEROBJECT_H_

#include "Arduino.h"
#include "hardware/pins/HardwareIDs.h"
#include "Types.h"

void newDelay(uint32_t);
uint32_t newMillis();

class TimerObject {

    public:
    TimerObject();
    void init();
    ledIntensity getLEDstate(uint8_t ledNumber);
    void setLEDstate(uint8_t ledNumber, ledIntensity intensity);
    encoderPosition getEncoderState(uint8_t encoderID);
    void setPWMsteps(uint8_t steps);

    private:
    void setupEncoder(uint8_t id, uint8_t, uint8_t);

};

extern TimerObject timers;

#endif