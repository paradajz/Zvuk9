#pragma once

#include "Import.h"

class BoardCommon
{
    public:
    virtual void init() = 0;
    virtual int8_t getEncoderState(uint8_t encoderID) = 0;
    virtual bool getButtonState(uint8_t buttonID) = 0;
    virtual void selectPad(uint8_t pad) = 0;
    virtual int16_t getPadPressure() = 0;
    virtual int16_t getPadX() = 0;
    virtual int16_t getPadY() = 0;
};
