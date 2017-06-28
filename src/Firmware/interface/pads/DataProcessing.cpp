#include "Pads.h"
#include "../lcd/menu/Menu.h"
#include "../../database/blocks/PadCalibration.h"

void Pads::update()
{
    bool restoreLCD = false;

    if (!board.padDataAvailable())
        return;

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        bool velocityAvailable = false;
        bool aftertouchAvailable = false;

        bool xAvailable = false;
        bool yAvailable = false;

        //all needed pressure samples are obtained
        velocityAvailable = checkVelocity(i);
        aftertouchAvailable = checkAftertouch(i, velocityAvailable);

        //only check x/y if pad is pressed
        if (isPadPressed(i))
        {
            xAvailable = checkX(i);
            yAvailable = checkY(i);

            if (calibrationEnabled && (activeCalibration == coordinateZ) && (pressureCalibrationTime != PRESSURE_ZONE_CALIBRATION_TIMEOUT) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
            {
                //update time after one second
                if ((rTimeMs() - pressureCalibrationLastChange) > 1000)
                {
                    pressureCalibrationTime++;
                    if (pressureCalibrationTime < PRESSURE_ZONE_CALIBRATION_TIMEOUT)
                    {
                        display.displayPressureCalibrationTime(PRESSURE_ZONE_CALIBRATION_TIMEOUT-pressureCalibrationTime, getPressureZone(i)+1, false);
                    }
                    else
                    {
                        display.displayPressureCalibrationTime(0, getPressureZone(i)+1, true);
                        calibratePressure(i, getPressureZone(i), board.getPadPressure(i));
                    }

                    pressureCalibrationLastChange = rTimeMs();
                }

            }
        }

        if (isPadPressed(i))
        {
            //send only midi event matching with pressed on/off button (if pressed)
            if (buttons.getButtonState(BUTTON_ON_OFF_X))
            {
                velocityAvailable = false;
                yAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_X);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_Y))
            {
                velocityAvailable = false;
                xAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_Y);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_NOTES))
            {
                xAvailable = false;
                yAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_NOTES);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_AFTERTOUCH))
            {
                velocityAvailable = false;
                xAvailable = false;
                yAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_AFTERTOUCH);
            }
        }
        else if (!isPadPressed(i) && velocityAvailable)
        {
            //make sure not to send note off if not necessary
            if (buttons.getButtonState(BUTTON_ON_OFF_X))
            {
                velocityAvailable = false;
                yAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_X);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_Y))
            {
                velocityAvailable = false;
                xAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_Y);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_NOTES))
            {
                xAvailable = false;
                yAvailable = false;
                aftertouchAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_NOTES);
            }
            else if (buttons.getButtonState(BUTTON_ON_OFF_AFTERTOUCH))
            {
                velocityAvailable = false;
                xAvailable = false;
                yAvailable = false;
                //disable x button temporarily on release
                buttons.disable(BUTTON_ON_OFF_AFTERTOUCH);
            }
        }

        if (velocityAvailable)
        {
            uint8_t index = getLastTouchedPad();

            //if pad is pressed, update last pressed pad
            //if it's released clear it from history
            updateLastPressedPad(i, bitRead(lastMIDInoteState, i));

            if (!bitRead(lastMIDInoteState, i))
            {
                //lcd restore detection
                //display data from last touched pad if current pad is released
                if ((index != getLastTouchedPad()) && getNumberOfPressedPads())
                    restoreLCD = true;
            }

            if (!getEditModeState())
            {
                if (bitRead(lastMIDInoteState, i) && splitEnabled)
                {
                    //update function leds only once, on press
                    //don't update if split is disabled (no need)
                    setFunctionLEDs(i);
                }
            }
            else
            {
                //setup pad edit mode on press for current pad
                if (bitRead(lastMIDInoteState, i))
                    setEditModeState(true, i);
            }
        }

        //check data to be sent
        //don't send or show midi data while in pad edit mode
        if (!getEditModeState())
        {
            //don't send midi data while in menu
            if (!menu.menuDisplayed())
                checkMIDIdata(i, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

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
                if (i == getLastTouchedPad())
                {
                    if (menu.menuDisplayed())
                    {
                        if (calibrationEnabled)
                            checkLCDdata(i, (velocityAvailable && (activeCalibration == coordinateZ)), false, (xAvailable && (activeCalibration == coordinateX)), (yAvailable && (activeCalibration == coordinateY)));
                    }
                    else
                    {
                        checkLCDdata(i, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);
                    }
                }
            }
        }
    }

    checkRemainingOctaveShift();
    checkRemainingNoteShift();
}

bool Pads::checkVelocity(uint8_t pad)
{
    uint16_t value = board.getPadPressure(pad);

    //detect if pressure is increasing or decreasing, but only if pad is pressed
    if (isPadPressed(pad))
        bitWrite(pressureReduction, pad, value < (uint16_t)lastPressureValue[pad]);
    else
        bitWrite(pressureReduction, pad, 0);

    uint8_t calibratedPressure = scalePressure(pad, value, getPressureZone(pad), pressureVelocity);
    calibratedPressure = curves.getCurveValue(pressureCurve, calibratedPressure, 0, 127);

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
                lastVelocityValue[pad] = calibratedPressure;
                bitWrite(padPressed, pad, true);  //set pad pressed
                bitWrite(lastMIDInoteState, pad, true);
                returnValue = true;
            }

            //always update lastPressure value
            lastPressureValue[pad] = value;
            break;

            case false:
            if (bitRead(padPressed, pad))
            {
                //pad is already pressed
                lastVelocityValue[pad] = calibratedPressure;
                bitWrite(lastMIDInoteState, pad, false);
                returnValue = true;
                lastXMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                lastYMIDIvalue[pad] = DEFAULT_XY_AT_VALUE;
                bitWrite(padPressed, pad, false);  //set pad not pressed
                if (isCalibrationEnabled() && (activeCalibration == coordinateZ))
                {
                    //reset calibration
                    if (!leds.getLEDstate(LED_TRANSPORT_RECORD))
                        display.displayPressureCalibrationStatus(false);
                    else
                        display.displayPressureCalibrationStatus(true);

                    pressureCalibrationTime = 0;
                    pressureCalibrationLastChange = 0;
                }
            }
            break;
        }
    }

    return returnValue;
}

bool Pads::checkAftertouch(uint8_t pad, bool velocityAvailable)
{
    //pad is pressed
    if (bitRead(lastMIDInoteState, pad))
    {
        uint8_t calibratedPressureAfterTouch = scalePressure(pad, lastPressureValue[pad], getPressureZone(pad), pressureAftertouch);

        if (!calibratedPressureAfterTouch)
            return false; //don't allow aftertouch 0

        if (lastAftertouchUpdateTime[pad] < 255)
            lastAftertouchUpdateTime[pad]++;

        bool updateAftertouch = false;

        //if it's been more than AFTERTOUCH_SEND_TIMEOUT since last time aftertouch was sent, aftertouch value
        //must exceed AFTERTOUCH_SEND_TIMEOUT_STEP
        //else the value must differ from last one and time difference must be more than AFTERTOUCH_SEND_TIMEOUT_IGNORE
        //so that we don't send fluctuating values
        if (lastAftertouchUpdateTime[pad] > AFTERTOUCH_SEND_TIMEOUT)
        {
            if (abs(calibratedPressureAfterTouch - lastAftertouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP)
                updateAftertouch = true;
        }
        else if (calibratedPressureAfterTouch != lastAftertouchValue[pad])
        {
            updateAftertouch = true;
        }

        //so far, it seems new aftertouch value passed all conditions
        if (updateAftertouch)
        {
            lastAftertouchValue[pad] = calibratedPressureAfterTouch;
            lastAftertouchUpdateTime[pad] = 0;

            if (!bitRead(aftertouchActivated, pad))
            bitWrite(aftertouchActivated, pad, true);

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
                    if (isPadPressed(i) && bitRead(aftertouchActivated, i))
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

                        if (!bitRead(aftertouchActivated, i))
                        continue;

                        if (!bitRead(aftertouchSendEnabled, i))
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
        if (velocityAvailable && bitRead(aftertouchActivated, pad))
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
                    if (bitRead(aftertouchActivated, i) && isPadPressed(i) && bitRead(aftertouchSendEnabled, i))
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

bool Pads::checkX(uint8_t pad)
{
    if (bitRead(pressureReduction, pad))
        return false;

    uint16_t value = board.getPadX(pad);

    if (calibrationEnabled && (activeCalibration == coordinateX) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
    {
        if (value < padXLimitLower[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating lowest value for X, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(coordinateX, limitTypeMin, pad, value);
        }
        else if (value > padXLimitUpper[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for X, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(coordinateX, limitTypeMax, pad, value);
        }
    }

    int16_t xValue = scaleXY(pad, value, coordinateX, true);
    xValue = curves.getCurveValue((curve_t)padCurveX[pad], xValue, ccXminPad[pad], ccXmaxPad[pad]);

    bool xChanged = false;

    if (xSendTimer[pad] < 255)
        xSendTimer[pad]++;

    if (xSendTimer[pad] > XY_SEND_TIMEOUT)
    {
        if (abs(xValue - lastXMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP)
            xChanged = true;
    }
    else if (xValue != lastXMIDIvalue[pad])
    {
        xChanged = true;
    }

    if (xChanged)
    {
        lastXMIDIvalue[pad] = xValue;
        xSendTimer[pad] = 0;
        return true;
    }

    return false;
}

bool Pads::checkY(uint8_t pad)
{
    if (bitRead(pressureReduction, pad))
        return false;

    uint16_t value = board.getPadY(pad);

    if (calibrationEnabled && (activeCalibration == coordinateY) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
    {
        if (value < padYLimitLower[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating lowest value for Y, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(coordinateY, limitTypeMin, pad, value);
        }
        else if (value > padYLimitUpper[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for Y, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(coordinateY, limitTypeMax, pad, value);
        }
    }

    int16_t yValue = scaleXY(pad, value, coordinateY, true);
    yValue = curves.getCurveValue((curve_t)padCurveY[pad], yValue, ccYminPad[pad], ccYmaxPad[pad]);

    bool yChanged = false;

    if (ySendTimer[pad] < 255)
        ySendTimer[pad]++;

    if (ySendTimer[pad] > XY_SEND_TIMEOUT)
    {
        if (abs(yValue - lastYMIDIvalue[pad]) > XY_SEND_TIMEOUT_STEP)
            yChanged = true;
    }
    else if (yValue != lastYMIDIvalue[pad])
    {
        yChanged = true;
    }

    if (yChanged)
    {
        lastYMIDIvalue[pad] = yValue;
        ySendTimer[pad] = 0;
        return true;
    }

    return false;
}

bool Pads::pressureStable(uint8_t pad, bool pressDetected)
{
    if (pressDetected)
    {
        if (!padDebounceTimer[pad])
        {
            padDebounceTimer[pad]++;
            return false;
        }
        else
        {
            if (padDebounceTimer[pad] > PAD_DEBOUNCE_TIME)
            {
                return true;
            }
            else
            {
                padDebounceTimer[pad]++;
                return false;
            }
        }
    }
    else
    {
        padDebounceTimer[pad] = 0;
        return true;
    }
}

void Pads::checkMIDIdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)
{
    //send X/Y immediately
    if (xAvailable && bitRead(xSendEnabled, pad))
        sendX(pad);

    if (yAvailable && bitRead(ySendEnabled, pad))
        sendY(pad);

    //send aftertouch immediately
    if (aftertouchAvailable && bitRead(aftertouchSendEnabled, pad))
        sendAftertouch(pad);

    if (velocityAvailable && bitRead(noteSendEnabled, pad))
    {
        switch(bitRead(lastMIDInoteState, pad))
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
    //make sure to check if pad is still pressed!
    if (bitRead(noteSendEnabled, pad_buffer[index]) && isPadPressed(pad_buffer[index]))
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
            if (bitRead(xSendEnabled, pad))
            {
                display.displayXYposition(lastXMIDIvalue[pad], coordinateX);

                if (!calibrationEnabled)
                    display.displayXYcc(ccXPad[pad], coordinateX);
            }
            else
            {
                display.clearXYposition(coordinateX);

                if (!calibrationEnabled)
                    display.clearXYcc(coordinateX);
            }
        }

        if (yAvailable)
        {
            if (bitRead(ySendEnabled, pad))
            {
                display.displayXYposition(lastYMIDIvalue[pad], coordinateY);

                if (!calibrationEnabled)
                    display.displayXYcc(ccYPad[pad], coordinateY);
            }
            else
            {
                display.clearXYposition(coordinateY);

                if (!calibrationEnabled)
                    display.clearXYcc(coordinateY);
            }
        }

        if (aftertouchAvailable)
        {
            if (bitRead(aftertouchSendEnabled, pad) && bitRead(aftertouchActivated, pad))
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
        else if (velocityAvailable && !bitRead(aftertouchActivated, pad))
        {
            display.clearAftertouch();
        }

        if (velocityAvailable)
        {
            display.displayVelocity(lastVelocityValue[getLastTouchedPad()]);

            if (!isCalibrationEnabled())
            {
                display.displayActivePadNotes();
                display.displayMIDIchannel();

                if (isPredefinedScale(activeScale))
                {
                    if (noteShiftLevel != 0)
                        display.displayNoteShiftLevel(noteShiftLevel);
                }
            }
        }

        if (lastShownPad != pad)
        {
            lastShownPad = pad;
            display.displayPad();
        }
    }
    else if (!getNumberOfPressedPads() && !lcdCleared)
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

    if (padPressHistory_counter < 0)
        padPressHistory_counter = 0;
}

void Pads::storeNotes(uint8_t pad)
{
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
