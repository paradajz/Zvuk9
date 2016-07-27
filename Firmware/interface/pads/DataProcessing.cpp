#include "Pads.h"

void Pads::addXYSamples(int16_t xValue, int16_t yValue)    {

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
    if (padCurveX[pad] != 0)  xValue = xyScale[padCurveX[pad]-1][xValue];

    bool xChanged = false;

    if ((rTimeMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(xValue - lastXMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP) xChanged = true;

    }   else if ((xValue != lastXMIDIvalue[pad]) && ((rTimeMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    xChanged = true;

    if (xChanged)   {

        lastXMIDIvalue[pad] = xValue;
        xSendTimer[pad] = rTimeMillis();

        padDebounceTimer[pad] = rTimeMillis(); //why? investigate
        return true;

    } return false;

}

bool Pads::checkY(uint8_t pad)  {

    int16_t yValue = scaleXY(pad, getMedianValueXYZ(coordinateY), ccTypeY);
    if (padCurveY[pad] != 0)  yValue = xyScale[padCurveY[pad]-1][yValue];

    bool yChanged = false;

    if ((rTimeMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(yValue - lastYMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP) yChanged = true;

    }   else if ((yValue != lastYMIDIvalue[pad]) && ((rTimeMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    yChanged = true;

    if (yChanged)   {

        lastYMIDIvalue[pad] = yValue;
        ySendTimer[pad] = rTimeMillis();

        padDebounceTimer[pad] = rTimeMillis(); //why? investigate
        return true;

    }   return false;

}

bool Pads::checkAftertouch(uint8_t pad, bool velocityAvailable)  {

    //pad is pressed
    if (lastMIDInoteState[pad]) {

        //initial pad press
        if (velocityAvailable)  {

            //pad has just been pressed, start the timer and return false
            aftertouchActivationDelay[pad] = rTimeMillis();
            return false;

        }   else

        //when pad is just pressed, wait a bit for pressure to stabilize
        if ((rTimeMillis() - aftertouchActivationDelay[pad]) < AFTERTOUCH_INITIAL_VALUE_DELAY) return false;

        uint8_t calibratedPressureAfterTouch = scalePressure(pad, lastPressureValue[pad], pressureAftertouch);
        if (!calibratedPressureAfterTouch) return false; //don't allow aftertouch 0

        uint32_t timeDifference = rTimeMillis() - lastAftertouchUpdateTime[pad];
        bool updateAftertouch = false;

        //if it's been more than AFTERTOUCH_SEND_TIMEOUT since last time aftertouch was sent, aftertouch value
        //must exceed AFTERTOUCH_SEND_TIMEOUT_STEP
        //else the value must differ from last one and time difference must be more than AFTERTOUCH_SEND_TIMEOUT_IGNORE
        //so that we don't send fluctuating values
        if (timeDifference > AFTERTOUCH_SEND_TIMEOUT)  {

            if (abs(calibratedPressureAfterTouch - lastAftertouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP)    {

                updateAftertouch = true;

            }

        }   else if ((calibratedPressureAfterTouch != lastAftertouchValue[pad]) && (timeDifference > AFTERTOUCH_SEND_TIMEOUT_IGNORE)) {

            updateAftertouch = true;

        }

        //so far, it seems new aftertouch value passed all conditions
        if (updateAftertouch) {

            lastAftertouchValue[pad] = calibratedPressureAfterTouch;
            lastAftertouchUpdateTime[pad] = rTimeMillis();
            if (!aftertouchActivated[pad]) aftertouchActivated[pad] = true;

            uint8_t padsPressed = 0;

            switch(aftertouchType)  {

                case aftertouchPoly:
                //no further checks needed
                return true;
                break;

                case aftertouchChannel:
                for (int i=0; i<MAX_PADS; i++)
                    if (isPadPressed(i) && aftertouchActivated[i]) padsPressed++;

                if (padsPressed == 1) {

                    maxAftertouchValue = calibratedPressureAfterTouch;
                    return true;

                } else if (padsPressed > 1) {    //find max pressure

                    uint8_t tempMaxValue = 0;

                    for (int i=0; i<MAX_PADS; i++)    {

                        if (!isPadPressed(i)) continue;
                        if (!aftertouchActivated[i]) continue;
                        if (!aftertouchSendEnabled[i]) continue;
                        if (lastAftertouchValue[i] > tempMaxValue)
                            tempMaxValue = lastAftertouchValue[i];

                    }

                    if (tempMaxValue != maxAftertouchValue)  {

                        maxAftertouchValue = calibratedPressureAfterTouch;

                        #if MODE_SERIAL > 0
                            printf("Maximum channel aftertouch updated: %d", maxAftertouchValue);
                        #endif

                        return true;

                    } else return false;

                }
                break;

            }

        }

    }   else {

        //pad is released
        //make sure to send aftertouch with pressure 0 on note off under certain conditions
        if (velocityAvailable && aftertouchActivated[pad])   {

            uint8_t pressedPadCounter = 0;

            lastAftertouchValue[pad] = 0;
            switch(aftertouchType)  {

                case aftertouchPoly:
                return true; //no further checks are needed

                case aftertouchChannel:
                for (int i=0; i<MAX_PADS; i++)  {

                    //count how many pads are pressed with activated aftertouch
                    if (aftertouchActivated[i] && isPadPressed(i) && aftertouchSendEnabled[i])
                        pressedPadCounter++;

                }

                if (!pressedPadCounter)  {

                    maxAftertouchValue = 0;
                    return true;

                }
                break;

            }

        }   else return false; //pad is released and 0 aftertouch has been sent

    }   return false;

}

void Pads::update()  {

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
            aftertouchAvailable = checkAftertouch(pad, velocityAvailable);

        }

    } else {

        if (xyUpdated(pad))    {

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
        if (!editModeActive()) {

            //don't send or show midi data while in pad edit mode or menu
            #ifdef MODULE_LCD
            if (!menu.menuDisplayed())
            #endif
                checkMIDIdata(pad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

            //only display data from last touched pad

            if (restoreLCD)    {

                uint8_t padIndex = getLastTouchedPad();

                //there are
                #ifdef MODULE_LCD
                if (!menu.menuDisplayed())
                #endif
                    checkLCDdata(padIndex, true, true, true, true);
                setFunctionLEDs(padIndex);

            }   else {

                    if (pad == getLastTouchedPad()) {
                    #ifdef MODULE_LCD
                        if (menu.menuDisplayed())   {

                            if (calibrationEnabled)
                                checkLCDdata(pad, false, false, (xAvailable && (activeCalibration == coordinateX)), (yAvailable && (activeCalibration == coordinateY)));

                        }
                    else checkLCDdata(pad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);
                    #else
                    checkLCDdata(pad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);
                    #endif

                }

            }

        }

        setNextPad();
        setMuxInput(activePad);

        velocityAvailable = false;
        aftertouchAvailable = false;
        xAvailable = false;
        yAvailable = false;

        //#if MODE_SERIAL > 0
            //if (activePad == 0)
                //printf("\n");
        //#endif

    }

    checkRemainingOctaveShift();
    checkRemainingNoteShift();

}

bool Pads::xyUpdated(uint8_t pad)  {

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

        if (calibrationEnabled) {

            //calibration is enabled
            int16_t medianValue = getMedianValueXYZ(activeCalibration);

            if (medianValue < minCalibrationValue)
            minCalibrationValue = medianValue+X_MIN_CALIBRATION_OFFSET;

            if (medianValue > maxCalibrationValue)
            maxCalibrationValue = medianValue+X_MAX_CALIBRATION_OFFSET;

            if (minCalibrationValue >= 0 && maxCalibrationValue < 1024) {

                pads.calibrate(activeCalibration, lower, pad, minCalibrationValue);
                pads.calibrate(activeCalibration, upper, pad, maxCalibrationValue);

                //#if MODE_SERIAL > 0
                    //printf("Calibrating pad %d\n", pad);
                    //printf("Minimum value: %d\n", minCalibrationValue);
                    //printf("Maximum value: %d\n", maxCalibrationValue);
                    //printf("Calibrated value: %d\n", scale_raw(medianValue, minCalibrationValue, maxCalibrationValue));
                //#endif

            }

        }

        return true;

    }

}

bool Pads::pressureStable(uint8_t pad, bool pressDetected)  {

    if (pressDetected) {

        if (!firstPressureValueDelayTimerStarted[pad])  {

            firstPressureValueDelayTimerStarted[pad] = true;
            padDebounceTimerStarted[pad] = false;
            firstPressureValueDelayTimer[pad] = rTimeMillis();
            return false;

        }   return (rTimeMillis() - firstPressureValueDelayTimer[pad] > PAD_PRESS_DEBOUNCE_TIME);


        } else {

        if (!padDebounceTimerStarted[pad])  {

            padDebounceTimerStarted[pad] = true;
            firstPressureValueDelayTimerStarted[pad] = false;
            padDebounceTimer[pad] = rTimeMillis();
            return false;

        }   return (rTimeMillis() - padDebounceTimer[pad] > PAD_RELEASE_DEBOUNCE_TIME);

    }

}

void Pads::addPressureSamples(int16_t value) {

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

        //#if MODE_SERIAL > 0
            //printf("pad %d median value: %d\tMIDI value: %d\n", pad, medianValue, calibratedPressure);
        //#endif

        //pad reading is stable

        switch (pressDetected)    {

            case true:
            if (!bitRead(padPressed, pad)) {  //pad isn't already pressed

                //sensor is really pressed
                bitWrite(padPressed, pad, true);  //set pad pressed
                lastVelocityValue[pad] = calibratedPressure;
                lastMIDInoteState[pad] = true;
                returnValue = true;

            }
            //always update lastPressure value
            lastPressureValue[pad] = medianValue;
            switchToXYread = true;
            break;

            case false:
            if (bitRead(padPressed, pad))  {  //pad is already pressed

                lastVelocityValue[pad] = calibratedPressure;
                lastMIDInoteState[pad] = false;
                returnValue = true;
                lastXMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                lastYMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                bitWrite(padPressed, pad, false);  //set pad not pressed
                switchToXYread = false;

            }
            break;

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

bool Pads::checkNoteBuffer()    {

    //notes are stored in buffer and they're sent after PAD_NOTE_SEND_DELAY
    //to avoid glide effect while sending x/y + notes

    if (note_buffer_head == note_buffer_tail)   {

        //buffer is empty
        return false;

    }

    //there is something in buffer
    uint8_t index = note_buffer_tail + 1;
    if (index >= PAD_NOTE_BUFFER_SIZE) index = 0;
    uint32_t noteTime = pad_note_timer_buffer[index];
    //this is fifo (circular) buffer
    //check first element in buffer
    //if first element (note) can't pass this condition, none of the other elements can, so return
    if ((rTimeMillis() - noteTime) < PAD_NOTE_SEND_DELAY) return false;
    //send
    if (noteSendEnabled[pad_buffer[index]]) {

        sendNotes(pad_buffer[index], velocity_buffer[index], true);

    }

    note_buffer_tail = index;
    return true;

}

void Pads::checkLCDdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)   {

    static bool lcdCleared = false;
    static int8_t lastShownPad = -1;

    if (isPadPressed(pad))  {   lcdCleared = false;

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

        if (yAvailable) {

            if (ySendEnabled[pad])  {

                display.displayXYposition(lastYMIDIvalue[pad], ccTypeY);
                display.displayXYcc(ccYPad[pad], ccTypeY);

                }   else {

                display.clearXYposition(ccTypeY);
                display.clearXYcc(ccTypeY);

            }

        }

        if (aftertouchAvailable)    {

            if (aftertouchSendEnabled[pad] && aftertouchActivated[pad]) {

                switch(aftertouchType)  {

                    case aftertouchChannel:
                    display.displayAftertouch(maxAftertouchValue);
                    break;

                    case aftertouchPoly:
                    display.displayAftertouch(lastAftertouchValue[pad]);
                    break;

                }

            }

            else display.clearAftertouch();

        }   else if (velocityAvailable && !aftertouchActivated[pad])
        display.clearAftertouch();
        #endif

        if (velocityAvailable)  {

            handleNoteLCD(pad, lastVelocityValue[pad], lastMIDInoteState[pad]);

        }

        if (velocityAvailable)  {

            #ifdef MODULE_LCD
                display.displayMIDIchannel(midiChannel[pad]);
            #endif

        }

        if (lastShownPad != pad)    {

            lastShownPad = pad;
            #ifdef MODULE_LCD
            display.displayPad(pad+1);
            #endif

        }

    }   else if (allPadsReleased() && !lcdCleared) {

            #ifdef MODULE_LCD
                display.clearPadData();
            #endif

            lcdCleared = true;
            lastShownPad = -1;

    }   else {

        if (velocityAvailable)
            display.clearAftertouch();

    }

}

void Pads::updateLastPressedPad(uint8_t pad, bool state)   {

    switch(state)   {

        case true:
        //pad is pressed, add it to touch history buffer
        if (pad != getLastTouchedPad())
            updatePressHistory(pad);
        break;

        case false:
        //pad released, clear it from buffer
        clearTouchHistoryPad(pad);
        break;

    }   resetCalibration();

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
        //vserial.print("Inserting pad ");
        //vserial.print(pad);
        //vserial.println(" to pad press history. New buffer:");
        //for (int i=0; i<MAX_PADS; i++)
            //vserial.println(padPressHistory_buffer[i]);
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
            //vserial.println("No more pressed pads. Current buffer:");
            //for (int i=0; i<MAX_PADS; i++)
                //vserial.println(padPressHistory_buffer[i]);
            //vserial.print("Current padPressHistoryCounter: ");
            //vserial.println(padPressHistory_counter);
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
        //vserial.print("Clearing pad ");
        //vserial.print(pad);
        //vserial.println(" from history");
        //vserial.println("New history buffer:");
        //for (int i=0; i<MAX_PADS; i++)
            //vserial.println(padPressHistory_buffer[i]);
    //#endif

}

void Pads::storeNotes(uint8_t pad)  {

    //#if MODE_SERIAL > 0
        //vserial.print("Storing notes in buffer for pad ");
        //vserial.println(pad);
    //#endif

    uint8_t i = note_buffer_head + 1;
    if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;
    //if buffer is full, wait until there is some space
    if (note_buffer_tail == i)  {

        #if MODE_SERIAL > 0
            printf("Oops, buffer full. Waiting...\n");
        #endif

        while (!checkNoteBuffer());

    }

    pad_buffer[i] = pad;
    velocity_buffer[i] = lastVelocityValue[pad];
    pad_note_timer_buffer[i] = rTimeMillis();
    note_buffer_head = i;

}