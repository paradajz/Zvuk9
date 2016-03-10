/*

    Hardware control functions for pads.

*/

#include "Pads.h"
#include <avr/cpufunc.h>

void Pads::initPadPins() {

    setOutputMacro(MUX_COMMON_PIN_0_DDR, MUX_COMMON_PIN_0_INDEX);
    setOutputMacro(MUX_COMMON_PIN_1_DDR, MUX_COMMON_PIN_1_INDEX);
    setOutputMacro(MUX_COMMON_PIN_2_DDR, MUX_COMMON_PIN_2_INDEX);
    setOutputMacro(MUX_COMMON_PIN_3_DDR, MUX_COMMON_PIN_3_INDEX);

    setOutputMacro(MUX_SELECT_PIN_0_DDR, MUX_SELECT_PIN_0_INDEX);
    setOutputMacro(MUX_SELECT_PIN_1_DDR, MUX_SELECT_PIN_1_INDEX);
    setOutputMacro(MUX_SELECT_PIN_2_DDR, MUX_SELECT_PIN_2_INDEX);
    setOutputMacro(MUX_SELECT_PIN_3_DDR, MUX_SELECT_PIN_3_INDEX);

    setLowMacro(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    setLowMacro(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    setLowMacro(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    setLowMacro(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    setHighMacro(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHighMacro(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setHighMacro(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHighMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

}

void Pads::setMuxInput(uint8_t muxInput)   {

    bitRead(muxInput, 0) ? setHighMacro(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX) : setLowMacro(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    bitRead(muxInput, 1) ? setHighMacro(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX) : setLowMacro(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    bitRead(muxInput, 2) ? setHighMacro(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX) : setLowMacro(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    bitRead(muxInput, 3) ? setHighMacro(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX) : setLowMacro(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

}

void Pads::setupPressure()  {

    //pressure is read from x+/y+
    //set 0/5V across x-/y-
    setInputMacro(MUX_COMMON_PIN_0_DDR, MUX_COMMON_PIN_0_INDEX);
    setOutputMacro(MUX_COMMON_PIN_1_DDR, MUX_COMMON_PIN_1_INDEX);
    setInputMacro(MUX_COMMON_PIN_2_DDR, MUX_COMMON_PIN_2_INDEX);
    setOutputMacro(MUX_COMMON_PIN_3_DDR, MUX_COMMON_PIN_3_INDEX);

    setHighMacro(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLowMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP(); _NOP();

    _NOP(); _NOP(); _NOP(); _NOP();

}

void Pads::setupX()  {

    //x is read from y+
    //set 0/5V across x+/x-
    setOutputMacro(MUX_COMMON_PIN_0_DDR, MUX_COMMON_PIN_0_INDEX);
    setOutputMacro(MUX_COMMON_PIN_1_DDR, MUX_COMMON_PIN_1_INDEX);
    setInputMacro(MUX_COMMON_PIN_2_DDR, MUX_COMMON_PIN_2_INDEX);
    setInputMacro(MUX_COMMON_PIN_3_DDR, MUX_COMMON_PIN_3_INDEX);

    setLowMacro(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHighMacro(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);

    _NOP(); _NOP(); _NOP(); _NOP();

}

void Pads::setupY()  {

    //y is read from x+
    //set 0/5V across y+/y-
    setInputMacro(MUX_COMMON_PIN_0_DDR, MUX_COMMON_PIN_0_INDEX);
    setInputMacro(MUX_COMMON_PIN_1_DDR, MUX_COMMON_PIN_1_INDEX);
    setOutputMacro(MUX_COMMON_PIN_2_DDR, MUX_COMMON_PIN_2_INDEX);
    setOutputMacro(MUX_COMMON_PIN_3_DDR, MUX_COMMON_PIN_3_INDEX);

    setLowMacro(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHighMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP(); _NOP();

}