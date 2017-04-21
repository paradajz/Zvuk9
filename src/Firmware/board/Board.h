#pragma once

#include "lcd/lcd.h"
#include "map/Map.h"
#include "pins/Pins.h"
#include "variables/Variables.h"
#include "constants/Constants.h"
#include "Import.h"
#include "DataTypes.h"

//function prototypes
inline void setAnalogPin(uint8_t muxNumber) __attribute__((always_inline));
inline void nextMuxInput() __attribute__((always_inline));
inline void ledRowsOff() __attribute__((always_inline));
inline void ledRowOn(uint8_t rowNumber, uint8_t intensity) __attribute__((always_inline));
inline void checkLEDs() __attribute__((always_inline));
inline void activateInputColumn(uint8_t column) __attribute__((always_inline));
inline void storeDigitalIn(uint8_t column) __attribute__((always_inline));
inline void activateOutputColumn() __attribute__((always_inline));

class Board
{
    public:
    Board();
    void init();
    int8_t getEncoderState(uint8_t encoderID);
    bool buttonDataAvailable();
    uint8_t getButtonState(uint8_t buttonID);
    uint8_t getEncoderPair(uint8_t buttonID);
    void selectPad(uint8_t pad);
    int16_t getPadPressure();
    int16_t getPadX();
    int16_t getPadY();
    void reboot(rebootType_t type);

    private:
    void initPins();
    void initTimers();
    void initPads();
};

extern Board board;
