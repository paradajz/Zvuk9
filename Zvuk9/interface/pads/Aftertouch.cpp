#include "Pads.h"

bool Pads::getAfterTouchGestureActivated(uint8_t padNumber, uint8_t pressure)  {

    if (afterTouchGestureCounter[padNumber] == GESTURE_NUMBER_OF_CHANGES) return true;

    //reset gesture if GESTURE_ACTIVATION_TIME is exceeded
    if (
    (newMillis() - afterTouchGestureTimer[padNumber] > GESTURE_ACTIVATION_TIME) &&
    (afterTouchGestureCounter[padNumber] != 0)
    )    {

        resetAfterTouchCounters(padNumber);
        initialPressure[padNumber] = pressure;

    }

    if (abs(pressure - initialPressure[padNumber]) > AFTERTOUCH_SEND_AFTER_DIFFERENCE)  {

        bool tempGestureDirection = (initialPressure[padNumber] > pressure);

        if (afterTouchGestureCounter[padNumber] == 0)   {

            //record new reference value
            initialPressure[padNumber] = pressure;
            //start gesture timer
            afterTouchGestureTimer[padNumber] = newMillis();
            //increment gesture counter
            afterTouchGestureCounter[padNumber]++;

            }   else if (tempGestureDirection != lastAfterTouchGestureDirection[padNumber]) {

            //only increase gesture counter if there's been a direction change
            afterTouchGestureCounter[padNumber]++;
            //record new reference value
            initialPressure[padNumber] = pressure;

        }

        //update last direction with current
        lastAfterTouchGestureDirection[padNumber] = tempGestureDirection;

    }

    return false;

}

void Pads::resetAfterTouchCounters(uint8_t padNumber) {

    afterTouchGestureCounter[padNumber]   = 0;
    afterTouchGestureTimer[padNumber]     = 0;
    initialPressure[padNumber]            = 0;
    initialXvalue[padNumber]              = -999;
    initialYvalue[padNumber]              = -999;

}

void Pads::sendAftertouch(uint8_t pad)  {

    #if MODE_SERIAL
        Serial.print(F("Pad "));
        Serial.print(pad);
        Serial.print(F(" aftertouch value: "));
        Serial.println(midiAfterTouch);
    #else
        midi.sendAfterTouch(midiChannel, midiAfterTouch);
    #endif

    messageBuilder.displayAftertouch(midiAfterTouch);

    afterTouchAvailable = false;

}

void Pads::checkAftertouch()  {

    uint8_t pad = activePad;
    int16_t pressure = lastPressureValue[pad]; //latest value

    if (!bitRead(padPressed, pad)) return; //don't check aftertouch if pad isn't pressed

    afterTouchActivated[pad] = getAfterTouchSendEnabled(pad) && getAfterTouchGestureActivated(pad, scalePressure(pad, pressure, pressureVelocity));

    //aftertouch
    if (afterTouchActivated[pad]) {

        uint8_t calibratedPressureAfterTouch = scalePressure(pad, pressure, pressureAfterTouch);

        uint32_t timeDifference = newMillis() - afterTouchSendTimer[pad];
        bool afterTouchSend = false;

        if (timeDifference > AFTERTOUCH_SEND_TIMEOUT)  {

            if (abs(calibratedPressureAfterTouch - lastAfterTouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP)    {

                afterTouchSend = true;

            }

            }   else if ((calibratedPressureAfterTouch != lastAfterTouchValue[pad]) && (timeDifference > AFTERTOUCH_SEND_TIMEOUT_IGNORE)) {

            afterTouchSend = true;

        }

        if (afterTouchSend) {

            afterTouchAvailable = true;
            padMovementDetected = true;
            midiAfterTouch = calibratedPressureAfterTouch;
            lastAfterTouchValue[pad] = midiAfterTouch;
            afterTouchSendTimer[pad] = newMillis();

        }

    }
    
}
