#include "Pads.h"
#include "../lcd/menu/Menu.h"
#include "../../database/blocks/PadCalibration.h"

const uint8_t pressureReductionConstant = 0b11111100;

uint16_t Pads::getAverageValue(padCoordinate_t coordinate)
{
    switch(coordinate)
    {
        case coordinateX:
        return xValueSample >> SAMPLE_SHIFT_AMOUNT;
        break;

        case coordinateY:
        return yValueSample >> SAMPLE_SHIFT_AMOUNT;
        break;

        case coordinateZ:
        return pressureValueSample >> SAMPLE_SHIFT_AMOUNT;
        break;

        default:
        return 0;

    }
}

bool Pads::checkX(uint8_t pad)
{
    if (pressureReduction[pad])
        return false;

    int16_t medianValue = getAverageValue(coordinateX);

    if (calibrationEnabled && (activeCalibration == coordinateX))
    {
        if ((medianValue < minCalibrationValue) && (medianValue != minCalibrationValue) && (medianValue >= DEFAULT_PAD_X_LIMIT_LOWER))
        {
            minCalibrationValue = medianValue;

            #ifdef DEBUG
            printf_P(PSTR("Calibrating lowest value for X, pad %d: %d\n"), pad, minCalibrationValue+X_MIN_CALIBRATION_OFFSET);
            #endif
            calibrate(coordinateX, lower, pad, minCalibrationValue+X_MIN_CALIBRATION_OFFSET);
        }
        else if ((medianValue > maxCalibrationValue) && (medianValue != maxCalibrationValue) && maxCalibrationValue && (medianValue <= DEFAULT_PAD_X_LIMIT_UPPER))
        {
            maxCalibrationValue = medianValue;

            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for X, pad %d: %d\n"), pad, maxCalibrationValue+X_MAX_CALIBRATION_OFFSET);
            #endif
            calibrate(coordinateX, upper, pad, maxCalibrationValue+X_MAX_CALIBRATION_OFFSET);
        }
    }

    int16_t xValue = scaleXY(pad, medianValue, coordinateX);

    xValue = curves.getCurveValue(coordinateX, (curve_t)padCurveX[pad], xValue, ccXminPad[pad], ccXmaxPad[pad]);

    bool xChanged = false;

    if ((rTimeMs() - xSendTimer[pad]) > XY_SEND_TIMEOUT)
    {
        if (abs(xValue - lastXMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP)
            xChanged = true;
    }
    else if ((xValue != lastXMIDIvalue[pad]) && ((rTimeMs() - xSendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    {
        xChanged = true;
    }

    if (xChanged)
    {
        lastXMIDIvalue[pad] = xValue;
        xSendTimer[pad] = rTimeMs();

        padDebounceTimer[pad] = rTimeMs(); //why? investigate
        return true;
    }

    return false;
}

bool Pads::checkY(uint8_t pad)
{
    if (pressureReduction[pad])
        return false;

    int16_t medianValue = getAverageValue(coordinateY);

    if (calibrationEnabled && (activeCalibration == coordinateY))
    {
        if ((medianValue < minCalibrationValue) && (medianValue != minCalibrationValue) && (medianValue >= DEFAULT_PAD_Y_LIMIT_LOWER))
        {
            minCalibrationValue = medianValue;

            #ifdef DEBUG
            printf_P(PSTR("Calibrating lowest value for Y, pad %d: %d\n"), pad, minCalibrationValue+X_MIN_CALIBRATION_OFFSET);
            #endif
            calibrate(coordinateY, lower, pad, minCalibrationValue+X_MIN_CALIBRATION_OFFSET);
        }
        else if ((medianValue > maxCalibrationValue) && (medianValue != maxCalibrationValue) && maxCalibrationValue && (medianValue <= DEFAULT_PAD_Y_LIMIT_UPPER))
        {
            maxCalibrationValue = medianValue;

            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for Y, pad %d: %d\n"), pad, maxCalibrationValue+X_MAX_CALIBRATION_OFFSET);
            #endif
            calibrate(coordinateY, upper, pad, maxCalibrationValue+X_MAX_CALIBRATION_OFFSET);
        }
    }

    int16_t yValue = scaleXY(pad, medianValue, coordinateY);
    yValue = curves.getCurveValue(coordinateY, (curve_t)padCurveY[pad], yValue, ccYminPad[pad], ccYmaxPad[pad]);

    bool yChanged = false;

    if ((rTimeMs() - ySendTimer[pad]) > XY_SEND_TIMEOUT)
    {
        if (abs(yValue - lastYMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP)
            yChanged = true;
    }
    else
    if ((yValue != lastYMIDIvalue[pad]) && ((rTimeMs() - ySendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
    {
        yChanged = true;
    }

    if (yChanged)
    {
        lastYMIDIvalue[pad] = yValue;
        ySendTimer[pad] = rTimeMs();

        padDebounceTimer[pad] = rTimeMs(); //why? investigate
        return true;
    }

    return false;
}

bool Pads::checkAftertouch(uint8_t pad, bool velocityAvailable)
{
    //pad is pressed
    if (lastMIDInoteState[pad])
    {
        //initial pad press
        if (velocityAvailable)
        {
            //pad has just been pressed, start the timer and return false
            aftertouchActivationDelay[pad] = rTimeMs();
            return false;
        }
        else
        {
            //when pad is just pressed, wait a bit for pressure to stabilize
            if ((rTimeMs() - aftertouchActivationDelay[pad]) < AFTERTOUCH_INITIAL_VALUE_DELAY)
                return false;
        }

        uint8_t calibratedPressureAfterTouch = scalePressure(pad, lastPressureValue[pad], pressureAftertouch);

        if (!calibratedPressureAfterTouch)
            return false; //don't allow aftertouch 0

        uint32_t timeDifference = rTimeMs() - lastAftertouchUpdateTime[pad];
        bool updateAftertouch = false;

        //if it's been more than AFTERTOUCH_SEND_TIMEOUT since last time aftertouch was sent, aftertouch value
        //must exceed AFTERTOUCH_SEND_TIMEOUT_STEP
        //else the value must differ from last one and time difference must be more than AFTERTOUCH_SEND_TIMEOUT_IGNORE
        //so that we don't send fluctuating values
        if (timeDifference > AFTERTOUCH_SEND_TIMEOUT)
        {
            if (abs(calibratedPressureAfterTouch - lastAftertouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP)
                updateAftertouch = true;
        }
        else if ((calibratedPressureAfterTouch != lastAftertouchValue[pad]) && (timeDifference > AFTERTOUCH_SEND_TIMEOUT_IGNORE))
        {
            updateAftertouch = true;
        }

        //so far, it seems new aftertouch value passed all conditions
        if (updateAftertouch)
        {
            lastAftertouchValue[pad] = calibratedPressureAfterTouch;
            lastAftertouchUpdateTime[pad] = rTimeMs();

            if (!aftertouchActivated[pad])
                aftertouchActivated[pad] = true;

            uint8_t padsPressed = 0;

            switch(aftertouchType)
            {
                case aftertouchPoly:
                //no further checks needed
                return true;
                break;

                case aftertouchChannel:
                for (int i=0; i<NUMBER_OF_PADS; i++)
                {
                    if (isPadPressed(i) && aftertouchActivated[i])
                        padsPressed++;
                }

                if (padsPressed == 1)
                {
                    maxAftertouchValue = calibratedPressureAfterTouch;
                    return true;
                } else if (padsPressed > 1)
                {
                    //find max pressure
                    uint8_t tempMaxValue = 0;

                    for (int i=0; i<NUMBER_OF_PADS; i++)
                    {
                        if (!isPadPressed(i))
                            continue;

                        if (!aftertouchActivated[i])
                            continue;
                        if (!aftertouchSendEnabled[i])
                            continue;

                        if (lastAftertouchValue[i] > tempMaxValue)
                            tempMaxValue = lastAftertouchValue[i];
                    }

                    if (tempMaxValue != maxAftertouchValue)
                    {
                        maxAftertouchValue = calibratedPressureAfterTouch;

                        #ifdef DEBUG
                        printf_P(PSTR("Maximum channel aftertouch updated: %d"), maxAftertouchValue);
                        #endif

                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                break;
            }
        }
    }
    else
    {
        //pad is released
        //make sure to send aftertouch with pressure 0 on note off under certain conditions
        if (velocityAvailable && aftertouchActivated[pad])
        {
            uint8_t pressedPadCounter = 0;

            lastAftertouchValue[pad] = 0;
            switch(aftertouchType)
            {
                case aftertouchPoly:
                return true; //no further checks are needed

                case aftertouchChannel:
                for (int i=0; i<NUMBER_OF_PADS; i++)
                {
                    //count how many pads are pressed with activated aftertouch
                    if (aftertouchActivated[i] && isPadPressed(i) && aftertouchSendEnabled[i])
                        pressedPadCounter++;
                }

                if (!pressedPadCounter)
                {
                    maxAftertouchValue = 0;
                    return true;
                }
                break;
            }
        }
        else
        {
            //pad is released and 0 aftertouch has been sent
            return false;
        }
    }

    return false;
}

/*
* This function is responsible for updating pad states. Function only updates one reading at the time
* to avoid blocking other parts of code. Three samples are needed to update pressure, three to update
* X, and three more to update Y. X/Y are read only if pressure is detected.
*/
//this is a horrible function
//go away
void Pads::update()
{
    static bool velocityAvailable = false;
    static bool aftertouchAvailable = false;
    static bool xAvailable = false;
    static bool yAvailable = false;
    bool restoreLCD = false;

    if (!switchToXYread)
    {
        if (pressureUpdated(activePad))
        {
            //all needed pressure samples are obtained
            velocityAvailable = checkVelocity(activePad);
            aftertouchAvailable = checkAftertouch(activePad, velocityAvailable);

        }
    }
    else
    {
        //don't update x/y if pressure isn't read
        if (xyUpdated(activePad))
        {
            xAvailable = checkX(activePad);
            yAvailable = checkY(activePad);

            switchToXYread = false;
            switchToNextPad = true;
        }
    }

    if (switchToNextPad)
    {
        //if we got to this point, everything that can be read is read
        if (velocityAvailable)
        {
            uint8_t index = getLastTouchedPad();

            //if pad is pressed, update last pressed pad
            //if it's released clear it from history
            updateLastPressedPad(activePad, lastMIDInoteState[activePad]);

            if (!lastMIDInoteState[activePad])
            {
                //lcd restore detection
                //display data from last touched pad if current pad is released
                if ((index != getLastTouchedPad()) && !allPadsReleased())
                    restoreLCD = true;
            }

            if (!getEditModeState())
            {
                if (lastMIDInoteState[activePad] && splitEnabled)
                {
                    //update function leds only once, on press
                    //don't update if split is disabled (no need)
                    setFunctionLEDs(activePad);
                }
                if (lastMIDInoteState[activePad])
                {
                    uint8_t lastPressedButton = buttons.getLastPressedButton();
                    if (buttons.getButtonState(lastPressedButton))
                    {
                        switch(lastPressedButton)
                        {
                            case BUTTON_ON_OFF_X:
                            buttons.disable(lastPressedButton);
                            noteSendEnabled[getLastTouchedPad()] = false;
                            ySendEnabled[getLastTouchedPad()] = false;
                            aftertouchSendEnabled[getLastTouchedPad()] = false;
                            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);
                            break;

                            case BUTTON_ON_OFF_Y:
                            buttons.disable(lastPressedButton);
                            noteSendEnabled[getLastTouchedPad()] = false;
                            xSendEnabled[getLastTouchedPad()] = false;
                            aftertouchSendEnabled[getLastTouchedPad()] = false;
                            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
                            break;

                            case BUTTON_ON_OFF_NOTES:
                            buttons.disable(lastPressedButton);
                            xSendEnabled[getLastTouchedPad()] = false;
                            ySendEnabled[getLastTouchedPad()] = false;
                            aftertouchSendEnabled[getLastTouchedPad()] = false;
                            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);
                            break;

                            case BUTTON_ON_OFF_AFTERTOUCH:
                            buttons.disable(lastPressedButton);
                            noteSendEnabled[getLastTouchedPad()] = false;
                            ySendEnabled[getLastTouchedPad()] = false;
                            xSendEnabled[getLastTouchedPad()] = false;
                            leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
                            leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);
                            break;

                            default:
                            //do nothing
                            break;
                        }
                    }
                }
            }
            else
            {
                 //setup pad edit mode on press for current pad
                if (lastMIDInoteState[activePad])
                    setEditModeState(true, activePad);
            }
        }

        //check data to be sent
        //don't send or show midi data while in pad edit mode
        if (!getEditModeState())
        {
            //don't send midi data while in menu
            if (!menu.menuDisplayed())
                checkMIDIdata(activePad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

            if (restoreLCD)
            {
                uint8_t padIndex = getLastTouchedPad();

                if (!menu.menuDisplayed())
                    checkLCDdata(padIndex, true, true, true, true);

                if (splitEnabled)
                    setFunctionLEDs(padIndex);
            }
            else
            {
                //if two pads are pressed, update data only from last pressed pad
                //i hate this function
                if (activePad == getLastTouchedPad())
                {
                    if (menu.menuDisplayed())
                    {
                        if (calibrationEnabled)
                            checkLCDdata(activePad, (velocityAvailable && (activeCalibration == coordinateZ)), false, (xAvailable && (activeCalibration == coordinateX)), (yAvailable && (activeCalibration == coordinateY)));
                    }
                    else
                    {
                        checkLCDdata(activePad, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);
                    }
                }
            }
        }

        setNextPad();

        velocityAvailable = false;
        aftertouchAvailable = false;
        xAvailable = false;
        yAvailable = false;
    }

    checkRemainingOctaveShift();
    checkRemainingNoteShift();
}

bool Pads::xyUpdated(uint8_t pad)
{
    int16_t xValue = board.getPadX();
    int16_t yValue = board.getPadY();

    xValueSample += xValue;
    yValueSample += yValue;
    xySampleCounter++;

    return (xySampleCounter == NUMBER_OF_SAMPLES);
}

bool Pads::pressureStable(uint8_t pad, bool pressDetected)
{
    if (pressDetected)
    {
        if (!firstPressureValueDelayTimerStarted[pad])
        {
            firstPressureValueDelayTimerStarted[pad] = true;
            padDebounceTimerStarted[pad] = false;
            firstPressureValueDelayTimer[pad] = rTimeMs();
            return false;
        }

        return (rTimeMs() - firstPressureValueDelayTimer[pad] > PAD_PRESS_DEBOUNCE_TIME);
    }
    else
    {
        if (!padDebounceTimerStarted[pad])
        {
            padDebounceTimerStarted[pad] = true;
            firstPressureValueDelayTimerStarted[pad] = false;
            padDebounceTimer[pad] = rTimeMs();
            return false;
        }

        return (rTimeMs() - padDebounceTimer[pad] > PAD_RELEASE_DEBOUNCE_TIME);
    }
}

void Pads::addPressureSample(int16_t value)
{
    pressureValueSample += value;
    pressureSampleCounter++;
}

bool Pads::pressureSampled()
{
    return (pressureSampleCounter == NUMBER_OF_SAMPLES);
}

bool Pads::pressureUpdated(uint8_t pad)
{
    int16_t pressure = board.getPadPressure();
    addPressureSample(pressure);

    if (!pressureSampled())
    {
        return false;
    }
    else
    {
        //we have pressure
        //reset pressure sample counter
        pressureSampleCounter = 0;

        //detect if pressure is increasing or decreasing, but only if pad is pressed
        if (isPadPressed(pad))
        {
            //pressureReduction[pad] = abs(getMedianValueXYZ(coordinateZ) - lastPressureValue[pad]) > 5;
            pressureReduction[pad] = getAverageValue(coordinateZ) < (uint16_t)lastPressureValue[pad];
        }
        else
        {
            pressureReduction[pad] = 0;
        }

        return true;
    }
}

bool Pads::checkVelocity(uint8_t pad)
{
    //we've taken 3 pressure samples so far, get median value
    int16_t medianValue = getAverageValue(coordinateZ);

    //if (calibrationEnabled && (activeCalibration == coordinateZ) && (medianValue > DEFAULT_PAD_PRESSURE_LIMIT_LOWER)) {
//
        ////calibration is enabled
        //int16_t medianValue = getMedianValueXYZ(activeCalibration);
//
        ////we are only updating max calibration value for pressure
//
        //if (medianValue > maxCalibrationValue)
        //maxCalibrationValue = medianValue;
//
        //if (maxCalibrationValue < 1024 && (maxCalibrationValue > DEFAULT_PAD_PRESSURE_LIMIT_LOWER)) {
//
            //pads.calibrate(activeCalibration, upper, pad, maxCalibrationValue);
            //#ifdef DEBUG
            //printf_P("New max calibration value, pressure, pad %d: %d\n", pad, maxCalibrationValue);
            //#endif
//
        //}
//
    //}

    //calibrate pressure based on median value (0-1023 -> 0-127)
    uint8_t calibratedPressure = scalePressure(pad, medianValue, pressureVelocity);
    calibratedPressure = curves.getCurveValue(coordinateZ, pressureCurve, calibratedPressure, 0, 127);

    bool pressDetected = (calibratedPressure > 0);
    bool returnValue = false;

    if (pressureStable(pad, pressDetected))
    {
        //pad reading is stable
        switch (pressDetected)
        {
            case true:
            if (!bitRead(padPressed, pad))
            {
                //pad isn't already pressed
                //sensor is really pressed
                bitWrite(padPressed, pad, true);  //set pad pressed
                lastVelocityValue[pad] = calibratedPressure;
                lastMIDInoteState[pad] = true;
                //always do this when pad is pressed
                resetCalibration();
                returnValue = true;
            }

            //always update lastPressure value
            lastPressureValue[pad] = medianValue;
            switchToXYread = true;
            break;

            case false:
            if (bitRead(padPressed, pad))
            {
                //pad is already pressed
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

    if (!switchToXYread)
        switchToNextPad = true;

    return returnValue;
}

void Pads::checkMIDIdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)
{
    //send X/Y immediately
    if (xAvailable && xSendEnabled[pad])
        sendX(pad);

    if (yAvailable && ySendEnabled[pad])
        sendY(pad);

    //send aftertouch immediately
    if (aftertouchAvailable && aftertouchSendEnabled[pad])
        sendAftertouch(pad);

    if (velocityAvailable && noteSendEnabled[pad])
    {
        switch(lastMIDInoteState[pad])
        {
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

bool Pads::checkNoteBuffer()
{
    //notes are stored in buffer and they're sent after PAD_NOTE_SEND_DELAY
    //to avoid glide effect while sending x/y + notes
    if (note_buffer_head == note_buffer_tail)
    {
        //buffer is empty
        return false;
    }

    //there is something in buffer
    uint8_t index = note_buffer_tail + 1;
    if (index >= PAD_NOTE_BUFFER_SIZE)
        index = 0;

    uint32_t noteTime = pad_note_timer_buffer[index];

    //this is fifo (circular) buffer
    //check first element in buffer
    //if first element (note) can't pass this condition, none of the other elements can, so return
    if ((rTimeMs() - noteTime) < PAD_NOTE_SEND_DELAY)
        return false;

    //send
    if (noteSendEnabled[pad_buffer[index]])
        sendNotes(pad_buffer[index], velocity_buffer[index], true);

    note_buffer_tail = index;
    return true;
}

void Pads::checkLCDdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)
{
    static bool lcdCleared = false;
    static int8_t lastShownPad = -1;

    if (isPadPressed(pad))
    {
        lcdCleared = false;

        if (xAvailable)
        {
            if (xSendEnabled[pad])
            {
                display.displayXYposition(lastXMIDIvalue[pad], coordinateX);
                display.displayXYcc(ccXPad[pad], coordinateX);
            }
            else
            {
                display.clearXYposition(coordinateX);
                display.clearXYcc(coordinateX);
            }
        }

        if (yAvailable)
        {
            if (ySendEnabled[pad])
            {
                display.displayXYposition(lastYMIDIvalue[pad], coordinateY);
                display.displayXYcc(ccYPad[pad], coordinateY);
            }
            else
            {
                display.clearXYposition(coordinateY);
                display.clearXYcc(coordinateY);
            }
        }

        if (aftertouchAvailable)
        {
            if (aftertouchSendEnabled[pad] && aftertouchActivated[pad])
            {
                switch(aftertouchType)
                {
                    case aftertouchChannel:
                    display.displayAftertouch(maxAftertouchValue);
                    break;

                    case aftertouchPoly:
                    display.displayAftertouch(lastAftertouchValue[pad]);
                    break;
                }
            }
            else
            {
                display.clearAftertouch();
            }
        }
        else if (velocityAvailable && !aftertouchActivated[pad])
        {
            display.clearAftertouch();
        }

        if (velocityAvailable)
            handleNoteLCD(pad, lastVelocityValue[pad], lastMIDInoteState[pad]);

        if (velocityAvailable)
            display.displayMIDIchannel(midiChannel[pad]);

        if (lastShownPad != pad)
        {
            lastShownPad = pad;
            display.displayPad(pad+1);
        }
    }
    else if (allPadsReleased() && !lcdCleared)
    {
        display.clearPadData();
        lcdCleared = true;
        lastShownPad = -1;
    }
    else
    {
        if (velocityAvailable)
            display.clearAftertouch();
    }
}

void Pads::updateLastPressedPad(uint8_t pad, bool state)
{
    switch(state)
    {
        case true:
        //pad is pressed, add it to touch history buffer
        if (pad != getLastTouchedPad())
        {
            updatePressHistory(pad);
            //setup curve?
        }
        break;

        case false:
        //pad released, clear it from buffer
        clearTouchHistoryPad(pad);
        break;
    }
}

void Pads::updatePressHistory(uint8_t pad)
{
    //store currently pressed pad in buffer
    uint8_t pressedPads = 0;

    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i)) pressedPads++;

    if (pressedPads == 1)
    {
        padPressHistory_buffer[0] = pad;
        padPressHistory_counter = 0;
    }
    else
    {
        padPressHistory_counter++;

        if (padPressHistory_counter >= NUMBER_OF_PADS)
            padPressHistory_counter = 0; //overwrite

        padPressHistory_buffer[padPressHistory_counter] = pad;
    }

    //#ifdef DEBUG
    //vserial.print("Inserting pad ");
    //vserial.print(pad);
    //vserial.println(" to pad press history. New buffer:");
    //for (int i=0; i<MAX_PADS; i++)
        //vserial.println(padPressHistory_buffer[i]);
    //#endif
}

void Pads::clearTouchHistoryPad(uint8_t pad)
{
    uint8_t padPressedCounter = 0;

    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i)) padPressedCounter++;

    if (padPressedCounter < 1)
    {
        for (int i=0; i<NUMBER_OF_PADS; i++)
            padPressHistory_buffer[i] = 0;

        padPressHistory_buffer[0] = pad;
        padPressHistory_counter = 0;

        //#ifdef DEBUG
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

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        if (padPressHistory_buffer[i] == pad)
        {
            index = i;
            padPressHistory_buffer[i] = newValue;
            break;
        }
    }

    //copy history array
    int8_t tempHistoryArray[NUMBER_OF_PADS];

    for (int i=0; i<NUMBER_OF_PADS; i++)
        tempHistoryArray[i] = padPressHistory_buffer[i];

    //shift all values so that newValue is at the end of array
    for (int i=index; i<(NUMBER_OF_PADS-1); i++)
        padPressHistory_buffer[i] = tempHistoryArray[i+1];

    padPressHistory_counter--;
    if (padPressHistory_counter < 0) padPressHistory_counter = 0;

    //#ifdef DEBUG
    //vserial.print("Clearing pad ");
    //vserial.print(pad);
    //vserial.println(" from history");
    //vserial.println("New history buffer:");
    //for (int i=0; i<MAX_PADS; i++)
        //vserial.println(padPressHistory_buffer[i]);
    //#endif
}

void Pads::storeNotes(uint8_t pad)
{
    //#ifdef DEBUG
    //vserial.print("Storing notes in buffer for pad ");
    //vserial.println(pad);
    //#endif

    uint8_t i = note_buffer_head + 1;

    if (i >= PAD_NOTE_BUFFER_SIZE)
        i = 0;

    //if buffer is full, wait until there is some space
    if (note_buffer_tail == i)
    {
        #ifdef DEBUG
        printf_P(PSTR("Oops, buffer full. Waiting...\n"));
        #endif

        while (!checkNoteBuffer());
    }

    pad_buffer[i] = pad;
    velocity_buffer[i] = lastVelocityValue[pad];
    pad_note_timer_buffer[i] = rTimeMs();
    note_buffer_head = i;
}

void Pads::setNextPad()
{
    switchToNextPad = false;
    xValueSample = 0;
    yValueSample = 0;
    pressureValueSample = 0;
    activePad++;
    pressureSampleCounter = 0;
    xySampleCounter = 0;

    if (activePad == CONNECTED_PADS)
        activePad = 0;

    board.selectPad(activePad);
}
