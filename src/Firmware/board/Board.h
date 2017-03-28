#pragma once

#include "lcd/lcd.h"
#include "map/Map.h"
#include "pins/Pins.h"
#include "variables/Variables.h"
#include "constants/Constants.h"
#include "Import.h"

//function prototypes
inline void setAnalogPin(uint8_t muxNumber) __attribute__((always_inline));
inline void nextMuxInput() __attribute__((always_inline));
inline void ledRowsOff() __attribute__((always_inline));
inline void ledRowOn(uint8_t rowNumber, uint8_t intensity) __attribute__((always_inline));
inline void checkLEDs() __attribute__((always_inline));
#ifdef BOARD_R2
inline void activateInputColumn(uint8_t column) __attribute__((always_inline));
inline void storeDigitalIn(uint8_t column) __attribute__((always_inline));
#endif
inline void activateOutputColumn() __attribute__((always_inline));

class Board
{
    public:
    Board();
    void init();
    int8_t getEncoderState(uint8_t encoderID);
    bool buttonDataAvailable();
    uint8_t getButtonState(uint8_t buttonID);
    bool padDataAvailable();
    void samplePads();
    uint16_t getPadPressure(uint8_t pad);
    uint16_t getPadX(uint8_t pad);
    uint16_t getPadY(uint8_t pad);
    #ifdef BOARD_R2
    uint8_t getEncoderPair(uint8_t buttonID);
    #endif

    private:
    void initPins();
    void initTimers();
    void initPads();
    #ifdef BOARD_R1
    void initButtons();
    #endif
    void initEncoders();
    int8_t readEncoder(uint8_t encoderID, uint8_t pairState);
};

extern Board board;
