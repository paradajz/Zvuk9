#pragma once

#include "../../board/Board.h"
#include "Config.h"

class Encoders
{
    public:
    Encoders();
    void init();
    void update(bool process = true);
    bool isEnabled(uint8_t encoderID);
    void flush();

    private:
    void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps);
    uint32_t lastStepTime[MAX_NUMBER_OF_ENCODERS];
    uint8_t encoderEnabled[MAX_NUMBER_OF_ENCODERS];
    bool encoderInverted[MAX_NUMBER_OF_ENCODERS];
};

extern Encoders encoders;
