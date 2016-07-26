#ifndef TIMEROBJECT_H_
#define TIMEROBJECT_H_

#include "../../Modules.h"
#include "../pins/Pins.h"
#include "../../Types.h"
#include "../../BitManipulation.h"

void wait(uint32_t);
uint32_t rTimeMillis();

class TimerObject {

    public:
    TimerObject();
    void init();
    ledState_t getLEDstate(uint8_t ledNumber);
    void setLEDstate(uint8_t ledNumber, ledState_t state);
    int8_t getEncoderState(uint8_t encoderID);
    void setPWMsteps(uint8_t steps);

};

extern TimerObject timers;

#endif
