#pragma once

#include "lcd/lcd.h"
#include "map/Map.h"
#include "Pins.h"
#include "Variables.h"
#include "constants/Constants.h"
#include "../Common.h"

//function prototypes
inline void setAnalogPin(uint8_t muxNumber) __attribute__((always_inline));
inline void setMuxInput(uint8_t muxInput) __attribute__((always_inline));
inline void ledRowsOff() __attribute__((always_inline));
inline void ledRowOn(uint8_t rowNumber, uint8_t intensity) __attribute__((always_inline));
inline void checkLEDs() __attribute__((always_inline));
inline void activateInputColumn(uint8_t column) __attribute__((always_inline));
inline void activateOutputColumn() __attribute__((always_inline));
inline void storeDigitalIn(uint8_t column, uint8_t bufferIndex) __attribute__((always_inline));
inline int8_t readEncoder(uint8_t encoderID, uint8_t pairState) __attribute__((always_inline));

class Board : BoardCommon
{
    public:
    Board();
    void init();
    int8_t getEncoderState(uint8_t encoderID);
    bool buttonDataAvailable();
    bool getButtonState(uint8_t buttonID);

    int16_t getPadPressure(uint8_t pad);
    int16_t getPadX(uint8_t pad);
    int16_t getPadY(uint8_t pad);

    private:
    void initTimers();
    void initButtons();
    void initEncoders();
    void initLEDs();
    void initPads();
};

extern Board board;
