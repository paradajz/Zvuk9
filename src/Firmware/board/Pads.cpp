#include "Board.h"

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] =
{
    MUX_COMMON_PIN_0_PIN,
    MUX_COMMON_PIN_1_PIN,
    MUX_COMMON_PIN_2_PIN,
    MUX_COMMON_PIN_3_PIN
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

inline void setMuxInput(uint8_t muxInput)
{
    bitRead(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    bitRead(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    bitRead(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    bitRead(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    //add NOPs to compensate for propagation delay
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
}

void setupPressure()
{
    //pressure is read from x+/y+
    //set 0/5V across x-/y-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();
}

void setupX()
{
    //x is read from y+
    //set 0/5V across x+/x-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder_board[readX]);
}

void setupY()
{
    //y is read from x+
    //set 0/5V across y+/y-
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder_board[readY]);
}

void Board::selectPad(uint8_t pad)
{
    static int8_t lastPad = -1;

    //do this only once
    if (lastPad != pad)
    {
        setMuxInput(padIDArray[pad]);
        lastPad = pad;
    }
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