#pragma once

class BoardCommon
{
    public:
    virtual void init() = 0;
    virtual int8_t getEncoderState(uint8_t encoderID) = 0;
    virtual bool getButtonState(uint8_t buttonID) = 0;
    virtual int16_t getPadPressure(uint8_t pad) = 0;
    virtual int16_t getPadX(uint8_t pad) = 0;
    virtual int16_t getPadY(uint8_t pad) = 0;
};
