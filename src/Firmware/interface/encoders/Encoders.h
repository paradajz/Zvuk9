#pragma once

#include "../../board/Board.h"
#include "Config.h"

class Encoders
{
    public:
    Encoders();
    void init();
    void update(bool process = true);
    void flush();

    private:
    uint8_t lastStepTime[MAX_NUMBER_OF_ENCODERS];
    int8_t encoderSpeed[MAX_NUMBER_OF_ENCODERS];
};

extern Encoders encoders;
