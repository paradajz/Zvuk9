#include "Pads.h"

void Pads::addXYSamples(uint16_t xValue, uint16_t yValue)    {

    xValueSamples[sampleCounterXY] = xValue;
    yValueSamples[sampleCounterXY] = yValue;

    sampleCounterXY++;

}

bool Pads::xySampled() {

    return (sampleCounterXY == NUMBER_OF_SAMPLES);

}

int16_t Pads::getMedianValueXYZ(coordinateType_t coordinate)  {

    int16_t medianValue = 0;

    switch(coordinate)  {

        case coordinateX:
        if ((xValueSamples[0] <= xValueSamples[1]) && (xValueSamples[0] <= xValueSamples[2]))
        {
            medianValue = (xValueSamples[1] <= xValueSamples[2]) ? xValueSamples[1] : xValueSamples[2];
        }
        else if ((xValueSamples[1] <= xValueSamples[0]) && (xValueSamples[1] <= xValueSamples[2]))
        {
            medianValue = (xValueSamples[0] <= xValueSamples[2]) ? xValueSamples[0] : xValueSamples[2];
        }
        else
        {
            medianValue = (xValueSamples[0] <= xValueSamples[1]) ? xValueSamples[0] : xValueSamples[1];
        }
        break;

        case coordinateY:
        if ((yValueSamples[0] <= yValueSamples[1]) && (yValueSamples[0] <= yValueSamples[2]))
        {
            medianValue = (yValueSamples[1] <= yValueSamples[2]) ? yValueSamples[1] : yValueSamples[2];
        }
        else if ((yValueSamples[1] <= yValueSamples[0]) && (yValueSamples[1] <= yValueSamples[2]))
        {
            medianValue = (yValueSamples[0] <= yValueSamples[2]) ? yValueSamples[0] : yValueSamples[2];
        }
        else
        {
            medianValue = (yValueSamples[0] <= yValueSamples[1]) ? yValueSamples[0] : yValueSamples[1];
        }
        break;

        case coordinateZ:
        if ((pressureValueSamples[0] <= pressureValueSamples[1]) && (pressureValueSamples[0] <= pressureValueSamples[2]))
        {
            medianValue = (pressureValueSamples[1] <= pressureValueSamples[2]) ? pressureValueSamples[1] : pressureValueSamples[2];
        }
        else if ((pressureValueSamples[1] <= pressureValueSamples[0]) && (pressureValueSamples[1] <= pressureValueSamples[2]))
        {
            medianValue = (pressureValueSamples[0] <= pressureValueSamples[2]) ? pressureValueSamples[0] : pressureValueSamples[2];
        }
        else
        {
            medianValue = (pressureValueSamples[0] <= pressureValueSamples[1]) ? pressureValueSamples[0] : pressureValueSamples[1];
        }
        break;

    }   return medianValue;

}

bool Pads::checkX(uint8_t pad)  {

    int16_t xValue = scaleXY(pad, getMedianValueXYZ(coordinateX), ccTypeX);

    bool xChanged = false;

    if ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(xValue - lastXMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP) xChanged = true;

    }   else if ((xValue != lastXMIDIvalue[pad]) && ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    xChanged = true;

    if (xChanged)   {

        lastXMIDIvalue[pad] = xValue;
        xSendTimer[pad] = newMillis();

        if (padCurveX[pad] != 0)  xValue = xyScale[padCurveX[pad]-1][xValue];

        padDebounceTimer[pad] = newMillis(); //why? investigate
        return true;

    } return false;

}

bool Pads::checkY(uint8_t pad)  {

    int16_t yValue = scaleXY(pad, getMedianValueXYZ(coordinateY), ccTypeY);

    bool yChanged = false;

    if ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(yValue - lastYMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP) yChanged = true;

    }   else if ((yValue != lastYMIDIvalue[pad]) && ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    yChanged = true;

    if (yChanged)   {

        lastYMIDIvalue[pad] = yValue;
        ySendTimer[pad] = newMillis();

        if (padCurveY[pad] != 0)  yValue = xyScale[padCurveY[pad]-1][yValue];

        padDebounceTimer[pad] = newMillis(); //why? investigate
        return true;

    }   return false;

}

bool Pads::checkAftertouch(uint8_t pad)  {

    int16_t pressure = lastPressureValue[pad]; //latest value
    bool returnValue = false;

    if (allPadsReleased()) { maxAftertouchValue = 0; return false; }
    if (!bitRead(padPressed, pad)) return false; //don't check aftertouch if pad isn't pressed

    //aftertouch
    if (getAfterTouchSendEnabled(pad)) {

        if (lastMIDInoteState[pad]) {

            //when pad is just pressed, wait a bit for pressure to stabilize
            if ((newMillis() - aftertouchActivationDelay[pad]) < AFTERTOUCH_INITIAL_VALUE_DELAY) return false;

        }

        uint8_t calibratedPressureAfterTouch = scalePressure(pad, pressure, pressureAftertouch);
        if (!calibratedPressureAfterTouch) return 0; //don't allow aftertouch 0

        uint32_t timeDifference = newMillis() - afterTouchSendTimer[pad];
        bool afterTouchSend = false;

        if (timeDifference > AFTERTOUCH_SEND_TIMEOUT)  {

            if (abs(calibratedPressureAfterTouch - lastAftertouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP)    {

                afterTouchSend = true;

            }

        }   else if ((calibratedPressureAfterTouch != lastAftertouchValue[pad]) && (timeDifference > AFTERTOUCH_SEND_TIMEOUT_IGNORE)) {

            afterTouchSend = true;

        }

        if (afterTouchSend) {

            lastAftertouchValue[pad] = calibratedPressureAfterTouch;

            if (!aftertouchActivated[pad])  {

                aftertouchActivated[pad] = true;

            }

            if (aftertouchActivated[pad])   {

                uint8_t padsPressed = 0;

                switch(aftertouchType)  {

                    case aftertouchPoly:
                    returnValue = true;
                    break;

                    case aftertouchChannel:
                    //we need to find max pressure
                    for (int i=0; i<MAX_PADS; i++)
                        if (isPadPressed(i)) padsPressed++;

                    if (padsPressed == 1) {

                        maxAftertouchValue = calibratedPressureAfterTouch;
                        returnValue = true;

                    } else {    uint8_t tempMaxValue = 0;

                        for (int i=0; i<MAX_PADS; i++)    {

                            if (!isPadPressed(i)) continue;
                            if (!aftertouchSendEnabled[i]) continue;
                            if (!aftertouchActivated[i]) continue;
                            if (lastAftertouchValue[i] > tempMaxValue)
                                tempMaxValue = lastAftertouchValue[i];


                        }

                        if (tempMaxValue != maxAftertouchValue)  {

                            maxAftertouchValue = calibratedPressureAfterTouch;
                            returnValue = true;

                            #if MODE_SERIAL > 0
                                Serial.print(F("Maximum channel aftertouch updated: "));
                                Serial.println(maxAftertouchValue);
                            #endif

                        } else {

                            returnValue = false;

                        }

                    }
                    break;

                }

            }   else {

                afterTouchSend = false;
                returnValue = false;

            }

            afterTouchSendTimer[pad] = newMillis();

        }

    }   return returnValue;

}

void Pads::update(bool midiEnabled)  {

    uint8_t pad = padID[activePad];

    static bool velocityAvailable = false;
    static bool aftertouchAvailable = false;
    static bool xAvailable = false;
    static bool yAvailable = false;
    bool restoreLCD = false;

    if (!switchToXYread)    {

        if (pressureUpdated())  {

            //all needed pressure samples are obtained
            velocityAvailable = checkVelocity(pad);
            aftertouchAvailable = checkAftertouch(pad);

            //special case
            //make sure to send aftertouch with pressure 0 on note off
            if (velocityAvailable && !lastMIDInoteState[pad])
                aftertouchAvailable = true;

        }

    } else {

        if (xyUpdated())    {

            xAvailable = checkX(pad);
            yAvailable = checkY(pad);

            switchToXYread = false;
            switchToNextPad = true;

        }

    }

    if (switchToNextPad)  {

        //if we got to this point, everything that can be read is read

        if (velocityAvailable)  {

            uint8_t index = getLastTouchedPad();

            //if pad is pressed, update last pressed pad
            //if it's released clear it from history
            updateLastPressedPad(pad, lastMIDInoteState[pad]);

            if (!lastMIDInoteState[pad])    {

                //a bit of black magic
                if ((index != getLastTouchedPad()) && !allPadsReleased())
                    restoreLCD = true;

            }

            if (!editModeActive() && lastMIDInoteState[pad]) //update only once, on press
                setFunctionLEDs(pad);

            if (editModeActive() && lastMIDInoteState[pad]) //update only once, on press
                setupPadEditMode(pad);

        }

        //check data to be sent
        #ifdef MODULE_LCD
        if (!editModeActive() && !menu.menuDisplayed()) {
        #else
        if (!editModeActive()) {
        #endif

            //don't send or show midi data while in pad edit mode or menu
            checkMIDIdata(pad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

            //only display data from last touched pad

            if (restoreLCD)    {

                uint8_t padIndex = getLastTouchedPad();

                //there are
                checkLCDdata(padIndex, true, aftertouchActivated[padIndex], true, true);
                setFunctionLEDs(padIndex);

            }   else {

                if (pad == getLastTouchedPad()) {

                    checkLCDdata(pad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

                }

            }

        }

        setNextPad();
        setMuxInput(activePad);

        velocityAvailable = false;
        aftertouchAvailable = false;
        xAvailable = false;
        yAvailable = false;

    }

    checkOctaveShift();

}

bool Pads::xyUpdated()  {

    //read x/y three times and get median value

    static int16_t xValue = -1, yValue = -1;
    static bool admuxSet = false;

    if (xValue == -1) {

        if (!admuxSet)  {

            //x
            #if XY_FLIP_AXIS > 0
                setupX();
            #else
                setupY();
            #endif
            admuxSet = true;

            return false;

        }

        #if XY_FLIP_AXIS > 0
            xValue = getX();
        #else
            xValue = getY();
        #endif

    }

    //check if value is now available
    if (xValue == -1) return false; //not yet

    //x is read by this point
    if (admuxSet && (yValue == -1)) {

        #if XY_FLIP_AXIS > 0
            setupY();
        #else
            setupX();
        #endif

        admuxSet = false;
        return false;

    }

    if (yValue == -1) {

        #if XY_FLIP_AXIS > 0
            yValue = getY();
        #else
            yValue = getX();
        #endif

    }

    if ((xValue != -1) && (yValue == -1)) return false;    //we don't have y yet

    //if we got to this point, we have x and y coordinates
    addXYSamples(xValue, yValue);

    if (!xySampled()) return false;
    else {

        xValue = -1;
        yValue = -1;
        sampleCounterXY = 0;
        admuxSet = false;
        return true;

    }

}

bool Pads::pressureStable(uint8_t pad, uint8_t pressDetected)  {

    if (pressDetected) {

        if (!firstPressureValueDelayTimerStarted[pad])  {

            firstPressureValueDelayTimerStarted[pad] = true;
            padDebounceTimerStarted[pad] = false;
            firstPressureValueDelayTimer[pad] = newMillis();
            return false;

        }   return (newMillis() - firstPressureValueDelayTimer[pad] > PAD_PRESS_DEBOUNCE_TIME);


        } else {

        if (!padDebounceTimerStarted[pad])  {

            padDebounceTimerStarted[pad] = true;
            firstPressureValueDelayTimerStarted[pad] = false;
            padDebounceTimer[pad] = newMillis();
            return false;

        }   return (newMillis() - padDebounceTimer[pad] > PAD_RELEASE_DEBOUNCE_TIME);

    }

}

void Pads::addPressureSamples(uint16_t value) {

    pressureValueSamples[sampleCounterPressure] = value;
    sampleCounterPressure++;

}

bool Pads::pressureSampled()   {

    return (sampleCounterPressure == NUMBER_OF_SAMPLES);

}

bool Pads::pressureUpdated() {

    int16_t pressure;

    pressure = getPressure();

    if (pressure == -1) return false;

    //we have pressure
    addPressureSamples(pressure);

    if (!pressureSampled()) return false;
    else {

        //reset pressure sample counter
        sampleCounterPressure = 0;
        return true;

    }

}

bool Pads::checkVelocity(uint8_t pad)  {

    //we've taken 3 pressure samples so far, get median value
    int16_t medianValue = getMedianValueXYZ(coordinateZ);
    //calibrate pressure based on median value (0-1023 -> 0-127)
    uint8_t calibratedPressure = scalePressure(pad, medianValue, pressureVelocity);

    bool pressDetected = (calibratedPressure > 0);
    bool returnValue = false;

    if (pressureStable(pad, pressDetected))    {

        //pad reading is stable

        switch (pressDetected)    {

            case true:
            if (!bitRead(padPressed, pad)) {  //pad isn't already pressed

                //sensor is really pressed
                bitWrite(padPressed, pad, true);  //set pad pressed
                lastVelocityValue[pad] = calibratedPressure;
                lastMIDInoteState[pad] = true;
                aftertouchActivationDelay[pad] = newMillis();
                returnValue = true;

            }
            break;

            case false:
            if (bitRead(padPressed, pad))  {  //pad is already pressed

                lastVelocityValue[pad] = calibratedPressure;
                lastMIDInoteState[pad] = false;
                returnValue = true;
                lastXMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                lastYMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                lastAftertouchValue[pad] = DEFAULT_XY_AT_VALUE;
                bitWrite(padPressed, pad, false);  //set pad not pressed
                switchToXYread = false;

            }
            break;

        }

        //send aftertouch only while sensor is pressed
        if (bitRead(padPressed, pad))  {

            lastPressureValue[pad] = medianValue;
            switchToXYread = true;

        }

    }

    if (!switchToXYread) switchToNextPad = true;
    return returnValue;

}

void Pads::checkMIDIdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)   {

    //send X/Y immediately
    if (xAvailable && xSendEnabled[pad])
        sendX(pad);

    if (yAvailable && ySendEnabled[pad])
        sendY(pad);

    //send aftertouch immediately
    if (aftertouchAvailable && aftertouchSendEnabled[pad])
        sendAftertouch(pad);

    if (velocityAvailable && noteSendEnabled[pad])  {

        switch(lastMIDInoteState[pad])  {

            case true:
            //if note on event happened, store notes in buffer first
            storeNotes(pad);
            break;

            case false:
            //note off event
            //send note off immediately
            sendNotes(pad, 0, false);
            break;

        }

    }

    checkNoteBuffer();

}

void Pads::checkNoteBuffer()    {

    //notes are stored in buffer and they're sent after PAD_NOTE_SEND_DELAY
    //to avoid glide effect while sending x/y + notes
    while (note_buffer_head != note_buffer_tail)   {

        uint8_t i = note_buffer_tail + 1;
        if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;

        //check buffer until it's empty
        uint32_t noteTime = pad_note_timer_buffer[i];
        //this is fifo (circular) buffer
        //check first element in buffer
        //if first element (note) can't pass this condition, none of the other elements can, so return
        if ((newMillis() - noteTime) < PAD_NOTE_SEND_DELAY) return;
        note_buffer_tail = i;

        if (noteSendEnabled[pad_buffer[i]]) {

            sendNotes(pad_buffer[i], velocity_buffer[i], true);

        }

    }

}

void Pads::checkLCDdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)   {

    if (isPadPressed(pad))  {

        //show X
        #ifdef MODULE_LCD
            if (xAvailable) {

                if (xSendEnabled[pad])  {

                    display.displayXYposition(lastXMIDIvalue[pad], ccTypeX);
                    display.displayXYcc(ccXPad[pad], ccTypeX);

                }   else {

                    display.clearXYposition(ccTypeX);
                    display.clearXYcc(ccTypeX);

                }

            }
        #endif

        #ifdef MODULE_LCD
            if (yAvailable) {

                if (ySendEnabled[pad])  {

                    display.displayXYposition(lastYMIDIvalue[pad], ccTypeY);
                    display.displayXYcc(ccYPad[pad], ccTypeY);

                }   else {

                    display.clearXYposition(ccTypeY);
                    display.clearXYcc(ccTypeY);

                }

            }
        #endif

        //show aftertouch
        #ifdef MODULE_LCD
            if (aftertouchAvailable)    {

                if (aftertouchSendEnabled[pad] && aftertouchActivated[pad])
                    display.displayAftertouch(lastAftertouchValue[pad]);
                else display.clearAftertouch();

            }
        #endif

        if (velocityAvailable)  {

            #ifdef MODULE_LCD
                display.displayPad(pad+1);
            #endif
            handleNoteLCD(pad, lastVelocityValue[pad], lastMIDInoteState[pad]);

        }

    }   else if (allPadsReleased()) {

            #ifdef MODULE_LCD
                display.displayActivePadNotes(0, 0, 0);
                display.clearVelocity();
                display.clearAftertouch();
                display.clearXYcc(ccTypeX);
                display.clearXYcc(ccTypeY);
                display.clearXYposition(ccTypeX);
                display.clearXYposition(ccTypeY);
                display.clearPad();
            #endif

    }

}

void Pads::updateLastPressedPad(uint8_t pad, bool state)   {

    switch(state)   {

        case true:
        //pad is pressed, add it to touch history buffer
        if (pad != getLastTouchedPad())    {

            #if MODE_SERIAL > 0
            Serial.print(F("Last touched pad: ")); Serial.println(getLastTouchedPad());
            Serial.print(F("Current pad: ")); Serial.println(pad);
            #endif

            updatePressHistory(pad);

        }
        break;

        case false:
        //pad released, clear it from buffer
        clearTouchHistoryPad(pad);
        break;

    }

}

void Pads::updatePressHistory(uint8_t pad) {

    //store currently pressed pad in buffer
    uint8_t pressedPads = 0;

    for (int i=0; i<MAX_PADS; i++)
        if (isPadPressed(i)) pressedPads++;

    if (pressedPads == 1)   {

        padPressHistory_buffer[0] = pad;
        padPressHistory_counter = 0;

    }   else {

        padPressHistory_counter++;
        if (padPressHistory_counter >= MAX_PADS)
            padPressHistory_counter = 0; //overwrite

        padPressHistory_buffer[padPressHistory_counter] = pad;

    }

    //#if MODE_SERIAL > 0
        //Serial.print(F("Inserting pad "));
        //Serial.print(pad);
        //Serial.println(F(" to pad press history. New buffer:"));
        //for (int i=0; i<MAX_PADS; i++)
            //Serial.println(padPressHistory_buffer[i]);
    //#endif

}

void Pads::clearTouchHistoryPad(uint8_t pad)    {

    uint8_t padPressedCounter = 0;

    for (int i=0; i<MAX_PADS; i++)
        if (isPadPressed(i)) padPressedCounter++;

    if (padPressedCounter < 1)  {

        for (int i=0; i<MAX_PADS; i++)
            padPressHistory_buffer[i] = 0;

        padPressHistory_buffer[0] = pad;
        padPressHistory_counter = 0;

        //#if MODE_SERIAL > 0
            //Serial.println(F("No more pressed pads. Current buffer:"));
            //for (int i=0; i<MAX_PADS; i++)
                //Serial.println(padPressHistory_buffer[i]);
            //Serial.print(F("Current padPressHistoryCounter: "));
            //Serial.println(padPressHistory_counter);
        //#endif

        return;

    }

    uint8_t index = pad;
    uint8_t newValue = 0;

    for (int i=0; i<MAX_PADS; i++)    {

        if (padPressHistory_buffer[i] == pad)   { index = i; padPressHistory_buffer[i] = newValue; break; }

    }

    //copy history array
    int8_t tempHistoryArray[MAX_PADS];

    for (int i=0; i<MAX_PADS; i++)
        tempHistoryArray[i] = padPressHistory_buffer[i];

    //shift all values so that newValue is at the end of array
    for (int i=index; i<(MAX_PADS-1); i++) {

        padPressHistory_buffer[i] = tempHistoryArray[i+1];

    }

    padPressHistory_counter--;
    if (padPressHistory_counter < 0) padPressHistory_counter = 0;

    //#if MODE_SERIAL > 0
        //Serial.print(F("Clearing pad "));
        //Serial.print(pad);
        //Serial.println(F(" from history"));
        //Serial.println(F("New history buffer:"));
        //for (int i=0; i<MAX_PADS; i++)
            //Serial.println(padPressHistory_buffer[i]);
    //#endif

}

void Pads::storeNotes(uint8_t pad)  {

    //#if MODE_SERIAL > 0
        //Serial.print(F("Storing notes in buffer for pad "));
        //Serial.println(pad);
    //#endif

    //store midi note on in circular buffer
    uint8_t i = note_buffer_head + 1;
    if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;
    pad_buffer[i] = pad;
    velocity_buffer[i] = lastVelocityValue[pad];
    pad_note_timer_buffer[i] = newMillis();
    note_buffer_head = i;

}