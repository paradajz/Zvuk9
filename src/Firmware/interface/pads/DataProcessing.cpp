#include "Pads.h"
#include "../lcd/menu/Menu.h"
#include "../../database/blocks/PadCalibration.h"

///
/// \ingroup interfacePads
/// @{

///
/// \brief Holds press states for all pads.
/// Implementation of Board variable.
///
volatile uint16_t   padPressed;

///
/// \brief Performs various continuous checks related to pad data.
///
void Pads::update()
{
    bool restoreLCD = false;

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        bool velocityAvailable = false;
        bool aftertouchAvailable = false;

        bool xAvailable = false;
        bool yAvailable = false;

        //all needed pressure samples are obtained
        velocityAvailable = checkVelocity(i);

        //only check x/y and aftertouch if pad is pressed
        if (isPadPressed(i))
        {
            xAvailable = checkX(i);
            yAvailable = checkY(i);

            aftertouchAvailable = checkAftertouch(i, velocityAvailable);

            if (calibrationEnabled && (activeCalibration == coordinateZ) && (pressureCalibrationTime != PRESSURE_ZONE_CALIBRATION_TIMEOUT) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
            {
                //update time after one second
                if ((rTimeMs() - pressureCalibrationLastChange) > 1000)
                {
                    pressureCalibrationTime++;

                    if (pressureCalibrationTime < PRESSURE_ZONE_CALIBRATION_TIMEOUT)
                    {
                        if (pressureCalibrationTime == 1)
                        {
                            if (xAvailable && yAvailable)
                                display.displayPressureCalibrationTime(PRESSURE_ZONE_CALIBRATION_TIMEOUT-pressureCalibrationTime, getPressureZone(i), false);
                        }
                        else
                        {
                            display.displayPressureCalibrationTime(PRESSURE_ZONE_CALIBRATION_TIMEOUT-pressureCalibrationTime, getPressureZone(i), false);
                        }
                    }
                    else
                    {
                        display.displayPressureCalibrationTime(0, getPressureZone(i), true);
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
                    updateFunctionLEDs(i);
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
            if (!menu.isMenuDisplayed())
                checkMIDIdata(i, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);

            if (restoreLCD)
            {
                uint8_t padIndex = getLastTouchedPad();

                if (!menu.isMenuDisplayed())
                    checkLCDdata(padIndex, true, true, true, true);

                if (splitEnabled)
                    updateFunctionLEDs(padIndex);

                #ifdef DEBUG
                printf_P(PSTR("Restoring data on LCD from last pad.\n"));
                #endif
            }
            else
            {
                //if two pads are pressed, update data only from last pressed pad
                //i hate this function
                if (i == getLastTouchedPad())
                {
                    if (menu.isMenuDisplayed())
                    {
                        //is menu is active and calibration mode is enabled, display only data for coordinate which is being calibrated
                        if (calibrationEnabled)
                            checkLCDdata(i, (activeCalibration == coordinateZ), false, (activeCalibration == coordinateX), activeCalibration == coordinateY);
                    }
                    else
                    {
                        checkLCDdata(i, velocityAvailable, aftertouchAvailable, xAvailable, yAvailable);
                    }
                }
            }
        }
    }
}

///
/// \brief Checks if velocity data is available on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns True if data is available, false otherwise.
///
bool Pads::checkVelocity(int8_t pad)
{
    assert(PAD_CHECK(pad));

    int16_t value = board.getPadPressure(pad);

    if (PRESSURE_RAW_VALUE_CHECK(value) == 0)
        return false;

    //detect if pressure is increasing or decreasing, but only if pad is pressed
    if (isPadPressed(pad))
        bitWrite(pressureReduction, pad, (uint16_t)value < (uint16_t)lastPressureValue[pad]);
    else
        bitWrite(pressureReduction, pad, 0);

    uint8_t calibratedPressure = getScaledPressure(pad, value, pressureVelocity);
    calibratedPressure = curves.getCurveValue(velocityCurve, calibratedPressure, 0, 127);

    bool pressDetected = (calibratedPressure > 0);
    bool returnValue = false;

    switch (pressDetected)
    {
        case true:
        if (!isPadPressed(pad))
        {
            //pad isn't already pressed
            //sensor is really pressed
            setPadPressState(pad, true);
            //store raw value so that pressure zone can be determined more precisely once x and y are read
            lastVelocityValue[pad] = getScaledPressure(pad, value, pressureVelocity);
            bitWrite(lastMIDInoteState, pad, true);
            lastPadPressTime[pad] = rTimeMs();
            returnValue = true;
        }

        //always update lastPressure value
        lastPressureValue[pad] = value;
        break;

        case false:
        if (isPadPressed(pad))
        {
            //pad is already pressed
            setPadPressState(pad, false);
            lastVelocityValue[pad] = getScaledPressure(pad, value, pressureVelocity);
            bitWrite(lastMIDInoteState, pad, false);
            returnValue = true;
            lastXCCvalue[pad] = DEFAULT_XY_AT_VALUE;
            lastYCCvalue[pad] = DEFAULT_XY_AT_VALUE;
            lastXPitchBendValue[pad] = DEFAULT_PITCH_BEND_VALUE;
            lastYPitchBendValue[pad] = DEFAULT_PITCH_BEND_VALUE;
            initialXposition[pad] = DEFAULT_INITIAL_XY_VALUE;
            initialYposition[pad] = DEFAULT_INITIAL_XY_VALUE;

            if (isCalibrationEnabled() && (activeCalibration == coordinateZ))
            {
                //reset calibration
                if (!leds.getLEDstate(LED_TRANSPORT_RECORD))
                    display.displayCalibrationStatus(pads.getCalibrationMode(), false);
                else
                    display.displayCalibrationStatus(pads.getCalibrationMode(), true);

                pressureCalibrationTime = 0;
                pressureCalibrationLastChange = 0;
            }
        }
        break;
    }

    return returnValue;
}

///
/// \brief Checks if aftertouch data is available on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns True if data is available, false otherwise.
///
bool Pads::checkAftertouch(int8_t pad, bool velocityAvailable)
{
    assert(PAD_CHECK(pad));

    //pad is pressed
    if (bitRead(lastMIDInoteState, pad))
    {
        uint8_t calibratedPressureAfterTouch = getScaledPressure(pad, lastPressureValue[pad], pressureAftertouch);

        if (lastAftertouchUpdateTime[pad] < 255)
            lastAftertouchUpdateTime[pad]++;

        bool updateAftertouch = false;

        //if it's been more than AFTERTOUCH_SEND_TIMEOUT since last time aftertouch was sent, aftertouch value
        //must exceed AFTERTOUCH_SEND_TIMEOUT_STEP
        //else the value must differ from last one and time difference must be more than AFTERTOUCH_SEND_TIMEOUT_IGNORE
        //so that we don't send fluctuating values
        if (lastAftertouchUpdateTime[pad] > AFTERTOUCH_SEND_TIMEOUT)
        {
            if ((abs(calibratedPressureAfterTouch - lastAftertouchValue[pad]) > AFTERTOUCH_SEND_TIMEOUT_STEP) || ((calibratedPressureAfterTouch != lastAftertouchValue[pad]) && !calibratedPressureAfterTouch))
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

            if (!bitRead(aftertouchActivated, pad) && calibratedPressureAfterTouch)
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
                }
                else if (padsPressed > 1)
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

///
/// \brief Checks if X coordinate data is available on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns True if data is available, false otherwise.
///
bool Pads::checkX(int8_t pad)
{
    assert(PAD_CHECK(pad));

    if ((rTimeMs() - lastPadPressTime[pad]) < XY_READ_DELAY)
        return false;

    if (bitRead(pressureReduction, pad))
        return false;

    int16_t value = board.getPadX(pad);

    if (!XY_RAW_VALUE_CHECK(value))
        return false;

    if (calibrationEnabled && (activeCalibration == coordinateX) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
    {
        if ((uint16_t)value < padXLimitLower[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating min value for X, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(pad, coordinateX, limitTypeMin, value);
        }
        else if ((uint16_t)value > padXLimitUpper[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for X, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(pad, coordinateX, limitTypeMax, value);
        }
    }

    if (initialXposition[pad] == DEFAULT_INITIAL_XY_VALUE)
        initialXposition[pad] = getScaledXY(pad, value, coordinateX, rawScale);

    bool xChanged = false;

    if (xSendTimer[pad] < 255)
        xSendTimer[pad]++;

    if (getPitchBendState(pad, coordinateX))
    {
        value = getScaledXY(pad, value, coordinateX, midiScale_14b);

        if (xSendTimer[pad] > XY_SEND_TIMEOUT)
        {
            if (abs(value - lastXPitchBendValue[pad]) > XY_SEND_TIMEOUT_STEP)
                xChanged = true;
        }
        else if (value != (int16_t)lastXPitchBendValue[pad])
        {
            xChanged = true;
        }
    }
    else
    {
        value = getScaledXY(pad, value, coordinateX, midiScale_7b);
        value = curves.getCurveValue((curve_t)padCurveX[pad], value, ccXminPad[pad], ccXmaxPad[pad]);

        if (xSendTimer[pad] > XY_SEND_TIMEOUT)
        {
            if (abs(value - lastXCCvalue[pad]) > XY_SEND_TIMEOUT_STEP)
                xChanged = true;
        }
        else if (value != lastXCCvalue[pad])
        {
            xChanged = true;
        }
    }

    if (xChanged)
    {
        if (getPitchBendState(pad, coordinateX))
            lastXPitchBendValue[pad] = value;
        else
            lastXCCvalue[pad] = value;

        xSendTimer[pad] = 0;
        return true;
    }

    return false;
}

///
/// \brief Checks if Y coordinate data is available on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns True if data is available, false otherwise.
///
bool Pads::checkY(int8_t pad)
{
    assert(PAD_CHECK(pad));

    if ((rTimeMs() - lastPadPressTime[pad]) < XY_READ_DELAY)
        return false;

    if (bitRead(pressureReduction, pad))
        return false;

    int16_t value = board.getPadY(pad);

    if (!XY_RAW_VALUE_CHECK(value))
        return false;

    if (calibrationEnabled && (activeCalibration == coordinateY) && (bool)leds.getLEDstate(LED_TRANSPORT_RECORD))
    {
        if ((uint16_t)value < padYLimitLower[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating min value for Y, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(pad, coordinateY, limitTypeMin, value);
        }
        else if ((uint16_t)value > padYLimitUpper[pad])
        {
            #ifdef DEBUG
            printf_P(PSTR("Calibrating max value for Y, pad %d: %d\n"), pad, value);
            #endif
            calibrateXY(pad, coordinateY, limitTypeMax, value);
        }
    }

    if (initialYposition[pad] == DEFAULT_INITIAL_XY_VALUE)
        initialYposition[pad] = getScaledXY(pad, value, coordinateY, rawScale);

    bool yChanged = false;

    if (ySendTimer[pad] < 255)
        ySendTimer[pad]++;

    if (getPitchBendState(pad, coordinateY))
    {
        value = getScaledXY(pad, value, coordinateY, midiScale_14b);

        if (ySendTimer[pad] > XY_SEND_TIMEOUT)
        {
            if (abs(value - lastYPitchBendValue[pad]) > XY_SEND_TIMEOUT_STEP)
                yChanged = true;
        }
        else if (value != (int16_t)lastYPitchBendValue[pad])
        {
            yChanged = true;
        }
    }
    else
    {
        value = getScaledXY(pad, value, coordinateY, midiScale_7b);
        value = curves.getCurveValue((curve_t)padCurveY[pad], value, ccYminPad[pad], ccYmaxPad[pad]);

        if (ySendTimer[pad] > XY_SEND_TIMEOUT)
        {
            if (abs(value - lastYCCvalue[pad]) > XY_SEND_TIMEOUT_STEP)
                yChanged = true;
        }
        else if (value != (int16_t)lastYCCvalue[pad])
        {
            yChanged = true;
        }
    }

    if (yChanged)
    {
        if (getPitchBendState(pad, coordinateY))
            lastYPitchBendValue[pad] = value;
        else
            lastYCCvalue[pad] = value;

        ySendTimer[pad] = 0;
        return true;
    }

    return false;
}

///
/// \brief Checks if pad MIDI data is available and calls corresponding handlers if it is.
/// @param [in] pad                     Pad for which MIDI data is being checked.
/// @param [in] velocityAvailable       If set to true, MIDI velocity data will be sent for requested pad.
/// @param [in] aftertouchAvailable     If set to true, MIDI aftertouch data will be sent for requested pad.
/// @param [in] xAvailable              If set to true, CC MIDI data for X coordinate will be sent for requested pad.
/// @param [in] yAvailable              If set to true, CC MIDI data for Y coordinate will be sent for requested pad.
///
void Pads::checkMIDIdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)
{
    assert(PAD_CHECK(pad));

    //send X/Y immediately
    if (xAvailable && bitRead(xSendEnabled, pad))
        sendX(pad);

    if (yAvailable && bitRead(ySendEnabled, pad))
        sendY(pad);

    //send aftertouch immediately
    if (aftertouchAvailable && bitRead(aftertouchSendEnabled, pad))
    {
        sendAftertouch(pad);

        if (isAftertouchActivated(pad) && !lastAftertouchValue[pad])
            bitWrite(aftertouchActivated, pad, 0);
    }

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

///
/// \brief Checks if there are any notes to send in pad note buffer.
/// \returns True if data is available in buffer, false otherwise.s
///
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

    //this is fifo (circular) buffer
    //check first element in buffer
    //if first element (note) can't pass this condition, none of the other elements can, so return
    if ((rTimeMs() - lastPadPressTime[index]) < PAD_NOTE_SEND_DELAY)
        return false;

    //send
    //make sure to check if pad is still pressed!
    if (bitRead(noteSendEnabled, pad_buffer[index]) && isPadPressed(pad_buffer[index]))
    {
        sendNotes(pad_buffer[index], lastVelocityValue[pad_buffer[index]], true);
        #ifdef DEBUG
        printf_P(PSTR("Zone %d\n"), getPressureZone(pad_buffer[index]));
        #endif
    }

    note_buffer_tail = index;

    return true;
}

///
/// \brief Checks if data for requested pad should be updated on LCD.
/// @param [in] pad                     Pad for which MIDI data is being checked.
/// @param [in] velocityAvailable       If set to true, MIDI velocity data will be refreshed for requested pad.
/// @param [in] aftertouchAvailable     If set to true, MIDI aftertouch data will be refreshed for requested pad.
/// @param [in] xAvailable              If set to true, CC MIDI data for X coordinate will be refreshed for requested pad.
/// @param [in] yAvailable              If set to true, CC MIDI data for Y coordinate will be refreshed for requested pad.
///
void Pads::checkLCDdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable)
{
    assert(PAD_CHECK(pad));

    static bool lcdCleared = true;
    static int8_t lastShownPad = -1;

    if (isPadPressed(pad))
    {
        lcdCleared = false;

        if (xAvailable)
        {
            if (bitRead(xSendEnabled, pad))
            {
                if (isCalibrationEnabled())
                {
                    if (lastXCCvalue[pad] != DEFAULT_XY_AT_VALUE)
                        display.displayXYvalue(coordinateX, midiMessageControlChange, board.getPadX(pad), lastXCCvalue[pad]);
                }
                else
                {
                    if (getPitchBendState(pad, coordinateX))
                        display.displayXYvalue(coordinateX, midiMessagePitchBend, lastXPitchBendValue[pad]);
                    else
                        display.displayXYvalue(coordinateX, midiMessageControlChange, ccXPad[pad], lastXCCvalue[pad]);
                }
            }
            else
            {
                //clear
                display.displayXYvalue(coordinateX);
            }
        }

        if (yAvailable)
        {
            if (bitRead(ySendEnabled, pad))
            {
                if (isCalibrationEnabled())
                {
                    if (lastYCCvalue[pad] != DEFAULT_XY_AT_VALUE)
                        display.displayXYvalue(coordinateY, midiMessageControlChange, board.getPadY(pad), lastYCCvalue[pad]);
                }
                else
                {
                    if (getPitchBendState(pad, coordinateY))
                        display.displayXYvalue(coordinateY, midiMessagePitchBend, lastYPitchBendValue[pad]);
                    else
                        display.displayXYvalue(coordinateY, midiMessageControlChange, ccYPad[pad], lastYCCvalue[pad]);
                }
            }
            else
            {
                display.displayXYvalue(coordinateY);
            }
        }

        if (aftertouchAvailable)
        {
            if (getMIDISendState(pad, functionOnOffAftertouch) && bitRead(aftertouchActivated, pad))
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
                //clear
                display.displayAftertouch();
            }
        }

        if (velocityAvailable)
        {
            isCalibrationEnabled() ? display.displayVelocity(getScaledPressure(pad, board.getPadPressure(pad), pressureAftertouch), board.getPadPressure(pad)) : display.displayVelocity(lastVelocityValue[pad]);

            if (!isCalibrationEnabled())
            {
                display.displayActivePadNotes();
                display.displayMIDIchannel(getMIDIchannel(pad));
            }
        }

        if (lastShownPad != pad)
        {
            lastShownPad = pad;
            display.displayPad(pad+1);
        }
    }
    else if (!getNumberOfPressedPads() && !lcdCleared)
    {
        display.clearPadPressData();
        lcdCleared = true;
        lastShownPad = -1;
    }
    else
    {
        //if (velocityAvailable)
            //display.clearAftertouch();
    }
}

/// @}