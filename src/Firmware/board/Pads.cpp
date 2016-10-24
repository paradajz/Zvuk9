#include "Board.h"

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] =
{
    MUX_COMMON_PIN_0_INDEX,
    MUX_COMMON_PIN_1_INDEX,
    MUX_COMMON_PIN_2_INDEX,
    MUX_COMMON_PIN_3_INDEX
};

uint8_t adcPinReadOrder_board[] =
{
    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[0], //pressure, second reading
    muxCommonPinsAnalogRead[0], //x coordinate
    muxCommonPinsAnalogRead[2]  //y coordinate
};

typedef enum
{
    readPressure0,
    readPressure1,
    readX,
    readY
} padReadOrder;

void Board::initPads()
{
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setOutput(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    setOutput(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    setOutput(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    setOutput(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    setHigh(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setUpADC();
}

void Board::setMuxInput(uint8_t muxInput)
{
    bitRead(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    bitRead(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    bitRead(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    bitRead(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();
}

void setupPressure()
{
    //pressure is read from x+/y+
    //set 0/5V across x-/y-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();
}

void setupX()
{
    //x is read from y+
    //set 0/5V across x+/x-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder_board[readX]);
}

void setupY()
{
    //y is read from x+
    //set 0/5V across y+/y-
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    //y=127
    //setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    //setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    //setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    //setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    //y=127
    //setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    //setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    //setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    //setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    //y value jumping
    //setHigh(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    //setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    //setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    //setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder_board[readY]);
}

int16_t Board::getPadPressure()
{
    int16_t value1, value2;

    setupPressure();
    setADCchannel(adcPinReadOrder_board[readPressure0]);
    value1 = getADCvalue();
    setADCchannel(adcPinReadOrder_board[readPressure1]);
    value2 = getADCvalue();

    return (1023 - (value2 - value1));
}

int16_t Board::getPadX()
{
    setupX();
    return getADCvalue();
}

int16_t Board::getPadY()
{
    setupY();
    return getADCvalue();
}