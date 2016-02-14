#include "Pads.h"

#define READ_AFTERTOUCH_0   0
#define READ_AFTERTOUCH_1   1
#define READ_X              2
#define READ_Y              3


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
        setAnalogInput(READ_AFTERTOUCH_0);
        analogReadStart();
        firstPin = false;

    }   else

    //conversion finished, save first value and request second
    if (!readInitiated && !firstPressurePinFinished && !secondPressurePinFinished && !firstPin)   {

        firstPressurePinFinished = true;
        tempPressureValueZ1 = getAnalogValue();
        setAnalogInput(READ_AFTERTOUCH_1);
        analogReadStart();
        firstPin = true;

    }   else

    //second conversion finished, save value
    if (!readInitiated && firstPressurePinFinished && !secondPressurePinFinished)    {

        secondPressurePinFinished = true;
        tempPressureValueZ2 = getAnalogValue();

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
    static bool admuxSet = false;

    if (!admuxSet)  {

        setupX();
        setAnalogInput(READ_X);
        admuxSet = true;
        return -1;

    }

    if (!readInitiated && !xSwitched)   {

        analogReadStart();
        xSwitched = true;
        return -1;

    }   else if (readInitiated && xSwitched)   return -1;
    else {

        xSwitched = false;
        admuxSet = false;

        return getAnalogValue();

    }

}

int16_t Pads::getY()  {

    bool readInitiated = analogReadInProgress();
    static bool ySwitched = false;
    static bool admuxSet = false;

    if (!admuxSet)  {

        setupY();
        setAnalogInput(READ_Y);
        admuxSet = true;
        return -1;

    }

    if (!readInitiated && !ySwitched)   {

        analogReadStart();
        ySwitched = true;
        return -1;

    }   else if (readInitiated && ySwitched)   return -1;

    else {

        ySwitched = false;
        admuxSet = false;

        return getAnalogValue();

    }

}