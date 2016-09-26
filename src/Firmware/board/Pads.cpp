#include "Board.h"

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] = {

    MUX_COMMON_PIN_0_INDEX,
    MUX_COMMON_PIN_1_INDEX,
    MUX_COMMON_PIN_2_INDEX,
    MUX_COMMON_PIN_3_INDEX

};

uint8_t adcPinReadOrder[] = {

    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[0], //pressure, second reading
    muxCommonPinsAnalogRead[2], //x coordinate
    muxCommonPinsAnalogRead[0]  //y coordinate

};

typedef enum {

    readPressure0,
    readPressure1,
    readX,
    readY

} padReadOrder;

void Board::initPads()  {

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

}

void Board::setMuxInput(uint8_t muxInput)   {

    bitRead(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_INDEX);
    bitRead(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_INDEX);
    bitRead(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_INDEX);
    bitRead(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

}

void setupPressure()  {

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

void setupX()  {

    //x is read from y+
    //set 0/5V across x+/x-
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder[readX]);

}

void setupY()  {

    //y is read from x+
    //set 0/5V across y+/y-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_INDEX);
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_INDEX);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_INDEX);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_INDEX);

    _NOP(); _NOP(); _NOP();

    setADCchannel(adcPinReadOrder[readY]);

}

int16_t Board::getPadPressure()  {

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

int16_t Board::getPadX()  {

    static int16_t xValue = -1;
    static bool admuxSet = false;

    if (xValue == -1) {

        if (!admuxSet)  {

            setupX();
            setupY();
            admuxSet = true;
            return -1;

        }

        bool readInitiated = analogReadInProgress();
        static bool xSwitched = false;

        if (!readInitiated && !xSwitched)   {

            startADCconversion();
            xSwitched = true;
            return -1;

            } else if (readInitiated && xSwitched)  {

            return -1;

            } else {

            xSwitched = false;
            xValue = -1;
            return getADCvalue();

        }

    }   return -1;

}

int16_t Board::getPadY()  {

    static int16_t yValue = -1;
    static bool admuxSet = false;

    if (yValue == -1) {

        if (!admuxSet)  {

            setupY();
            admuxSet = true;
            return -1;

        }

        bool readInitiated = analogReadInProgress();
        static bool ySwitched = false;

        if (!readInitiated && !ySwitched)   {

            startADCconversion();
            ySwitched = true;
            return -1;

            } else if (readInitiated && ySwitched)  {

            return -1;

            } else {

            ySwitched = false;
            yValue = -1;
            return getADCvalue();

        }

    }   return -1;

}