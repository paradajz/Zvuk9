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

void Pads::checkXY()  {

    //store median value in these variables NUMBER_OF_MEDIAN_RUNS times, then get avg value
    static int16_t xAvg = 0;
    static int16_t yAvg = 0;

    xAvailable = false;
    yAvailable = false;

    uint8_t pad = padID[activePad];

    int16_t xValue = scaleXY(pad, getMedianValueXYZ(coordinateX), ccTypeX);
    int16_t yValue = scaleXY(pad, getMedianValueXYZ(coordinateY), ccTypeY);

    xAvg += xValue;
    yAvg += yValue;

    medianRunCounterXY++;
    if (medianRunCounterXY != NUMBER_OF_MEDIAN_RUNS) return;

    xAvg = xAvg / NUMBER_OF_MEDIAN_RUNS;
    yAvg = yAvg / NUMBER_OF_MEDIAN_RUNS;

    medianRunCounterXY = 0;

    xValue = xAvg;
    yValue = yAvg;

    xAvg = 0;
    yAvg = 0;

    bool xChanged = false;
    bool yChanged = false;

    if ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(xValue - lastXValue[pad]) > XY_SEND_TIMEOUT_STEP) xChanged = true;

    }   else if ((xValue != lastXValue[pad]) && ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    xChanged = true;


    if ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(yValue - lastYValue[pad]) > XY_SEND_TIMEOUT_STEP) yChanged = true;

    }   else if ((yValue != lastYValue[pad]) && ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    yChanged = true;

    if (xChanged)   {

        lastXValue[pad] = xValue;
        xSendTimer[pad] = newMillis();

        if (padCurveX[pad] != 0)  xValue = xyScale[padCurveX[pad]-1][xValue];

        xAvailable = true;

    }

    if (yChanged)   {

        lastYValue[pad] = yValue;
        ySendTimer[pad] = newMillis();

        if (padCurveY[pad] != 0)  yValue = xyScale[padCurveY[pad]-1][yValue];

        yAvailable = true;

    }

    if (xChanged || yChanged)   {

        padDebounceTimer[pad] = newMillis();
        xyAvailable = true;
        padMovementDetected = true;
        midiX = xValue;
        midiY = yValue;

    }

    switchToXYread = false;
    switchToNextPad = true;

}

void Pads::update(bool midiEnabled)  {

    static bool firstRun = true;    //true when switching to another pad

    if (firstRun) { setMuxInput(activePad); firstRun = false; } //only activate mux input once per pad

    if (!switchToXYread)    {

        bool pressureProcessed = processPressure();
        if (pressureProcessed)  {

            //all needed pressure samples are obtained
            checkVelocity();
            checkAftertouch();

        }

        } else {

        bool xyProcessed = processXY();
        if (xyProcessed)
        checkXY();

    }

    if (switchToNextPad)  {

        //if we got to this point, everything that can be read is read
        //check data to be sent

        if (padMovementDetected)   {

            updateLastTouchedPad();
            padMovementDetected = false;

        }

        if (!editModeActive() && !menu.menuDisplayed())
        //don't send midi data while in pad edit mode or menu
        checkMIDIdata();

        firstRun = true;
        setNextPad();

    }

    checkOctaveShift();

}

bool Pads::processXY()  {

    //read x/y three times, get median value, then read x/y again until NUMBER_OF_MEDIAN_RUNS
    //get avg x/y value

    static int16_t xValue = -1, yValue = -1;
    static bool admuxSet = false;

    if (xValue == -1) {

        if (!admuxSet)  {

            //x
            #if XY_FLIP_AXIS > 0
            setupY();
            #else
            setupX();
            #endif
            admuxSet = true;

            return false;

        }

        #if XY_FLIP_AXIS > 0
        xValue = getY();
        #else
        xValue = getX();
        #endif

    }

    //check if value is now available
    if (xValue == -1) return false; //not yet

    //x is read by this point
    if (admuxSet && (xValue != -1) && (yValue == -1)) {

        #if XY_FLIP_AXIS > 0
        setupX();
        #else
        setupY();
        #endif

        admuxSet = false;
        return false;

    }

    if (yValue == -1) {

        #if XY_FLIP_AXIS > 0
        yValue = getX();
        #else
        yValue = getY();
        #endif

    }
    if (!((xValue != -1) && (yValue != -1))) return false;    //we don't have y yet

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

bool Pads::processPressure() {

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

void Pads::checkVelocity()  {

    uint8_t pad = padID[activePad];

    //we've taken 3 pressure samples so far, get median value
    int16_t medianValue = getMedianValueXYZ(coordinateZ);
    //calibrate pressure based on median value (0-1023 -> 0-127)
    uint8_t calibratedPressure = scalePressure(pad, medianValue, pressureVelocity);

    bool pressDetected = (calibratedPressure > 0);

    if (pressureStable(pad, pressDetected))    {

        //pad reading is stable

        switch (pressDetected)    {

            case true:
            if (!bitRead(padPressed, pad)) {  //pad isn't already pressed

                //sensor is really pressed
                bitWrite(padPressed, pad, true);  //set pad pressed
                initialPressure[pad] = calibratedPressure;
                midiVelocity = calibratedPressure;
                midiNoteOn = true;
                velocityAvailable = true;
                padMovementDetected = true;

            }
            break;

            case false:
            if (bitRead(padPressed, pad))  {  //pad is already pressed

                midiVelocity = calibratedPressure;
                midiNoteOn = false;
                velocityAvailable = true;
                padMovementDetected = true;
                lastXValue[pad] = DEFAULT_XY_VALUE;
                lastYValue[pad] = DEFAULT_XY_VALUE;
                bitWrite(padPressed, pad, false);  //set pad not pressed
                //reset all aftertouch gestures after pad is released
                resetAfterTouchCounters(pad);
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

}

void Pads::checkMIDIdata()   {

    uint8_t pad = padID[activePad];
    uint8_t previousPad = getPadPressHistoryIndex(previousID);

    //send X/Y immediately
    if (xyAvailable)
        sendXY(pad);

    //send aftertouch immediately
    if (afterTouchAvailable)
        sendAftertouch(pad);

    if (velocityAvailable)  {

        switch(midiNoteOn)  {

            case true:
            //if note on event happened, store notes in buffer first
            storeNotes(pad);
            break;

            case false:
            //note off event
            //send note off
            sendNotes(pad, 0, false);
            //pad has been released
            //decrease pad history counter, but only if there are some pressed pads
            if (!allPadsReleased()) {

                padPressHistory_counter--;
                if (padPressHistory_counter < 0) padPressHistory_counter = NUMBER_OF_PADS-1;

            }
            //restore lcd/led states from previous pad if it's pressed
            if (isPadPressed(previousPad))  {

                //restore data from last touched pad (display+midi cc x/cc y)
                bool ccXsendEnabled = getCCXsendEnabled(previousPad);
                bool ccYsendEnabled = getCCYsendEnabled(previousPad);
                uint8_t ccXvaluePreviousPad = getCCvalue(ccTypeX, previousPad);
                uint8_t ccYvaluePreviousPad = getCCvalue(ccTypeY, previousPad);
                uint8_t ccXvalueActivePad = getCCvalue(ccTypeX, pad);
                uint8_t ccYvalueActivePad = getCCvalue(ccTypeY, pad);

                if ((ccXvalueActivePad == ccXvaluePreviousPad) && ccXsendEnabled)
                    midi.sendControlChange(midiChannel, ccXvaluePreviousPad, lastXMIDIvalue[previousPad]);
                else if ((ccYvalueActivePad == ccYvaluePreviousPad) && ccYsendEnabled)
                    midi.sendControlChange(midiChannel, ccYvaluePreviousPad, lastYMIDIvalue[previousPad]);

                if (getNoteSendEnabled(previousPad))
                    handleNote(previousPad, lastVelocityValue[previousPad], true);

                handleXY(previousPad, lastXValue[previousPad], lastYValue[previousPad], ccXsendEnabled, ccYsendEnabled);
                setFunctionLEDs(previousPad);

            }
            break;

        }   velocityAvailable = false;

    }

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

        sendNotes(pad_buffer[i], velocity_buffer[i], true);

    }

}

void Pads::updateLastTouchedPad()   {

    if (padID[activePad] != getPadPressHistoryIndex(lastActiveID))    {

        #if MODE_SERIAL > 0
            Serial.print(F("Last touched pad: ")); Serial.println(getPadPressHistoryIndex(lastActiveID));
            Serial.print(F("Current pad: ")); Serial.println(padID[activePad]);
        #endif

        setFunctionLEDs(padID[activePad]);
        updatePressHistory(padID[activePad]);

        if (editModeActive())
        setupPadEditMode(padID[activePad]);

    }

}

void Pads::updatePressHistory(uint8_t pad) {

    //store currently pressed pad in circular buffer
    padPressHistory_counter++;
    if (padPressHistory_counter >= NUMBER_OF_PADS) padPressHistory_counter = 0; //overwrite

    padPressHistory_buffer[padPressHistory_counter] = pad;

    #if MODE_SERIAL > 0
        Serial.println(F("Current touch history:"));
        for (int i=0; i<NUMBER_OF_PADS; i++)
            Serial.println(padPressHistory_buffer[i]);
    #endif

}

void Pads::storeNotes(uint8_t pad)  {

    //store midi note on in circular buffer
    uint8_t i = note_buffer_head + 1;
    if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;
    pad_buffer[i] = pad;
    velocity_buffer[i] = midiVelocity;
    pad_note_timer_buffer[i] = newMillis();
    note_buffer_head = i;

    lastVelocityValue[pad] = midiVelocity;

}