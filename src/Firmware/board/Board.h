#pragma once

#include "lcd/lcd.h"
#include "map/Map.h"
#include "pins/Pins.h"
#include "variables/Variables.h"
#include "constants/Constants.h"
#include "Import.h"

//function prototypes
inline void setAnalogPin(uint8_t muxNumber) __attribute__((always_inline));
inline void setMuxInput(uint8_t muxInput) __attribute__((always_inline));
inline void ledRowsOff() __attribute__((always_inline));
inline void ledRowOn(uint8_t rowNumber, uint8_t intensity) __attribute__((always_inline));
inline void checkLEDs() __attribute__((always_inline));
#ifdef BOARD_R2
inline void activateInputColumn(uint8_t column) __attribute__((always_inline));
inline void storeDigitalIn(uint8_t column, uint8_t bufferIndex) __attribute__((always_inline));
#endif
inline void activateOutputColumn() __attribute__((always_inline));
inline int8_t readEncoder(uint8_t encoderID, uint8_t pairState) __attribute__((always_inline));

class Board
{
    public:
    Board();
    void init();
    int8_t getEncoderState(uint8_t encoderID);
    bool buttonDataAvailable();
    bool getButtonState(uint8_t buttonID);

    void selectPad(uint8_t pad);
    int16_t getPadPressure();
    int16_t getPadX();
    int16_t getPadY();
    #ifdef BOARD_R2
    uint8_t getEncoderPair(uint8_t buttonID);
    bool encoderDataAvailable();
    #endif

    private:
    void initTimers();
    #ifdef BOARD_R1
    void initButtons();
    void initEncoders();
    #endif
    void initLEDs();
    void initPads();
    #ifdef BOARD_R2
    bool copyInputMatrixBuffer();
    void checkInputMatrixBufferCopy();
    #endif
};

extern Board board;
