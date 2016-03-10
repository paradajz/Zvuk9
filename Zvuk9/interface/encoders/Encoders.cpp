#include "Encoders.h"
#include "EncoderSettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"

#define ENCODER_SPEED_1         1
#define ENCODER_SPEED_2         5

#define SPEED_TIMEOUT           100

#define ENCODER_DEBOUNCE_TIME   500

Encoders::Encoders()    {

    //default constructor
    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
        lastStepTime[i] = 0;

    sendEncoderCallback = NULL;

}

void Encoders::init()   {

    //nothing

}

void Encoders::update() {

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        int8_t encoderSteps = timers.getEncoderState(i);
        if (encoderSteps == 0) continue;

        uint32_t timeDifference = newMillis() - lastStepTime[i];
        uint8_t steps = ENCODER_SPEED_1;

        if (timeDifference > SPEED_TIMEOUT) steps = ENCODER_SPEED_1;
        else steps = ENCODER_SPEED_2;

        lastStepTime[i] = newMillis();

        for (int j=0; j<abs(encoderSteps); j++)
            sendEncoderCallback(i, (encoderSteps > 0), steps);

    }

}

void Encoders::setHandleEncoderCallback(void (*fptr)(uint8_t encoderNumber, bool direction, uint8_t steps))   {

    sendEncoderCallback = fptr;

}

Encoders encoders;
