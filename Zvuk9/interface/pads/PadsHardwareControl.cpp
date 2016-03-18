#include "Pads.h"
#include <avr/cpufunc.h>

uint8_t adcPinReadOrder[] = {

    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[0], //pressure, second reading
    muxCommonPinsAnalogRead[2], //x coordinate
    muxCommonPinsAnalogRead[0]  //y coordinate

};

//read order
typedef enum {

    readPressure0,
    readPressure1,
    readX,
    readY

} padReadOrder;

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

    setLowMacro(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHighMacro(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLowMacro(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setLowMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

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
    setLowMacro(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setLowMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder[readX]);

}

void Pads::setupY()  {

    //y is read from x+
    //set 0/5V across y+/y-
    setInputMacro(MUX_COMMON_PIN_0_DDR, MUX_COMMON_PIN_0_INDEX);
    setInputMacro(MUX_COMMON_PIN_1_DDR, MUX_COMMON_PIN_1_INDEX);
    setOutputMacro(MUX_COMMON_PIN_2_DDR, MUX_COMMON_PIN_2_INDEX);
    setOutputMacro(MUX_COMMON_PIN_3_DDR, MUX_COMMON_PIN_3_INDEX);

    setLowMacro(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setLowMacro(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLowMacro(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHighMacro(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder[readY]);

}

int16_t Pads::getPressure()  {

    static int16_t  tempPressureValueZ1 = 0,
                    tempPressureValueZ2 = 0;

    static bool firstPin = true;

    static bool firstPressurePinFinished = false;
    static bool secondPressurePinFinished = false;

    bool readInitiated = analogReadInProgress();
    int16_t returnValue = -1;

    //conversion isn't started yet, setup pressure and request first value
    if (!readInitiated && (!firstPressurePinFinished && !secondPressurePinFinished) && firstPin)   {

        setupPressure();
        setADCchannel(adcPinReadOrder[readPressure0]);
        startADCconversion();
        firstPin = false;

    }   else

    //conversion finished, save first value and request second
    if (!readInitiated && !firstPressurePinFinished && !secondPressurePinFinished && !firstPin)   {

        firstPressurePinFinished = true;
        tempPressureValueZ1 = getADCvalue();
        setADCchannel(adcPinReadOrder[readPressure1]);
        startADCconversion();
        firstPin = true;

    }   else

    //second conversion finished, save value
    if (!readInitiated && firstPressurePinFinished && !secondPressurePinFinished)    {

        secondPressurePinFinished = true;
        tempPressureValueZ2 = getADCvalue();

        firstPressurePinFinished = false;
        secondPressurePinFinished = false;
        returnValue = (1023 - (tempPressureValueZ2 - tempPressureValueZ1));

        tempPressureValueZ1 = 0;
        tempPressureValueZ2 = 0;

    }

    return returnValue;

}

int16_t Pads::getX()  {

    bool readInitiated = analogReadInProgress();
    static bool xSwitched = false;

    if (!readInitiated && !xSwitched)   {

        startADCconversion();
        xSwitched = true;
        return -1;

    }   else if (readInitiated && xSwitched)   return -1;
    else {

        xSwitched = false;

        return getADCvalue();

    }

}

int16_t Pads::getY()  {

    bool readInitiated = analogReadInProgress();
    static bool ySwitched = false;

    if (!readInitiated && !ySwitched)   {

        startADCconversion();
        ySwitched = true;
        return -1;

    }   else if (readInitiated && ySwitched)   return -1;

    else {

        ySwitched = false;

        return getADCvalue();

    }

}