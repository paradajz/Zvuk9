#ifndef BOARD_H_
#define BOARD_H_

#include "../core/Core.h"
#include "pins/board_v1/Map.h"
#include "pins/board_v1/Pins.h"
#include "lcd/lcd.h"
#include "Variables.h"
#include "Constants.h"

uint32_t rTimeMillis();

typedef enum {

    ledStateOff,
    ledStateDim,
    ledStateFull

} ledState_t;

class Board {

    public:
    Board();
    void init();
    ledState_t getLEDstate(uint8_t ledNumber);
    void setLEDstate(uint8_t ledNumber, ledState_t state, bool blink = false);
    int8_t getEncoderState(uint8_t encoderID);
    void setPWMsteps(uint8_t steps);
    bool buttonDataAvailable();
    bool getButtonState(uint8_t buttonID);
    int16_t getPadPressure();
    int16_t getPadX();
    int16_t getPadY();
    void setMuxInput(uint8_t muxInput);

    private:
    void write_I2C_reg(uint8_t address, uint8_t reg, uint8_t value);
    uint8_t read_I2C_reg(uint8_t address, uint8_t reg);

    void initTimers();
    void initButtons();
    void initEncoders();
    void initLEDs();
    void initPads();

};

extern Board board;

#endif