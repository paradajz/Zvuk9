#include "../../Modules.h"

#ifdef MODULE_ENCODERS

#ifndef ENCODERS_H_
#define ENCODERS_H_

#include "../../hardware/pins/Pins.h"

#define PROGRAM_ENCODER         0
#define PRESET_ENCODER          1

#define X_CC_ENCODER            8
#define X_MAX_ENCODER           3
#define X_MIN_ENCODER           6
#define X_CURVE_ENCODER         2

#define Y_CC_ENCODER            7
#define Y_MAX_ENCODER           9
#define Y_MIN_ENCODER           5
#define Y_CURVE_ENCODER         4

class Encoders  {

    public:
    Encoders();
    void init();
    void update();

    private:
    void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps);
    uint32_t lastStepTime[NUMBER_OF_ENCODERS];

};

extern Encoders encoders;

#endif
#endif