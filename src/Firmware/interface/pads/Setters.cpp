#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

///
/// \brief Changes active program.
/// @param [in] program New program which is being set.
/// \returns Result of changing program (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setProgram(int8_t program)
{
    //don't allow program change if pads are pressed
    if (pads.getNumberOfPressedPads())
        return releasePads;

    //input validation
    if (program < 0)
        program = 0;
    else if (program >= NUMBER_OF_PROGRAMS)
        program = NUMBER_OF_PROGRAMS-1;

    if (program != activeProgram)
    {
        database.update(DB_BLOCK_PROGRAM, programLastActiveProgramSection, (uint16_t)0, program);
        getProgramParameters();
        return valueChanged;
    }

    return noChange;
}

///
/// \brief Changes active scale.
/// @param [in] scale New scale which is being set.
/// \returns Result of changing scale (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setScale(int8_t scale)
{
    //don't allow scale change if pads are pressed
    if (pads.getNumberOfPressedPads())
        return releasePads;

    assert(SCALE_CHECK(scale));

    //input validation
    if (scale < 0)
        scale = 0;
    else if (scale > (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES))
        scale = (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES) - 1;

    if (activeScale != scale)
    {
        database.update(DB_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram, scale);
        getScaleParameters();
        return valueChanged;
    }

    return noChange;
}

///
/// \brief Changes active tonic.
/// @param [in] newTonic Tonic to be set.
/// @param [in] internalChange  If set to true, function is called from within Pads class and certain values are set differently.
/// This should never be called with true value outside of Pads class (set to false by default).
/// \returns Result of changing tonic (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setTonic(note_t newTonic, bool internalChange)
{
    //don't allow tonic change if pads are pressed
    //ignore internal setting here
    if (pads.getNumberOfPressedPads() && !internalChange)
        return releasePads;

    //safety check
    if (newTonic >= MIDI_NOTES)
        return outOfRange;

    if (newTonic == getTonic())
    {
        #ifdef DEBUG
        printf_P(PSTR("No change in tonic.\n"));
        #endif
        return noChange;
    }

    changeResult_t result = noChange;
    note_t currentScaleTonic;

    if (internalChange)
    {
        //internal change means that this function got called on startup
        //on startup, tonic isn't applied yet, so it's first note on first pad by default
        //this never gets called on startup for user scales
        currentScaleTonic = getTonicFromNote(padNote[0][0]);
    }
    else
    {
        currentScaleTonic = getTonic();
    }

    //determine distance between notes
    uint8_t changeDifference;

    bool changeAllowed = true;
    bool shiftDirection;

    if (currentScaleTonic == MIDI_NOTES)
        return notAllowed; //pad has no notes

    shiftDirection = (uint8_t)currentScaleTonic < (uint8_t)newTonic;

    changeDifference = abs((uint8_t)currentScaleTonic - (uint8_t)newTonic);

    if (!changeDifference)
        return noChange;

    changeAllowed = true;

    //check if all notes are within range before shifting
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        if (!changeAllowed)
            break;

        for (int j=0; j<NOTES_PER_PAD; j++)
        {
            if ((uint8_t)currentScaleTonic < (uint8_t)newTonic)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if ((padNote[i][j] + changeDifference) > 127)
                        changeAllowed = false;
                }
            }
            else if ((uint8_t)currentScaleTonic > (uint8_t)newTonic)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if ((padNote[i][j] - changeDifference) < 0)
                        changeAllowed = false;
                }
            }
        }
    }

    //change notes
    if (changeAllowed)
    {
        result = valueChanged;

        uint16_t noteID = ((uint16_t)activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        if (isPredefinedScale(activeScale) && !internalChange)
            database.update(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+(PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale)+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram), newTonic);

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            uint8_t newNote;

            for (int j=0; j<NOTES_PER_PAD; j++)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if (shiftDirection)
                        newNote = padNote[i][j] + changeDifference;
                    else
                        newNote = padNote[i][j] - changeDifference;

                    if (isUserScale(activeScale) && !internalChange)
                    {
                        //async write
                        database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i), newNote, true);
                    }

                    padNote[i][j] = newNote;
                }
            }
        }

        #ifdef DEBUG
        printf_P(PSTR("Tonic changed, active tonic %d\n"), newTonic);
        #endif

    }
    else
    {
        #ifdef DEBUG
        printf_P(PSTR("Unable to change tonic: one or more pad notes are too %s\n"), shiftDirection ? "high" : "low");
        #endif

        result = outOfRange;
    }

    return result;
}

///
/// \brief Enables or disables pad edit mode.
/// @param [in] state Pad edit state. Enabled if set to true, false otherwise.
/// @param [in] pad Pad which is being configured in pad edit mode. This parameter isn't required if edit mode is being disabled.
/// \returns Result of setting up pad edit mode (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setEditModeState(bool state, int8_t pad)
{
    if (pads.isPredefinedScale(activeScale))
        return notUserScale;

    if (!editModeActivated && pads.getNumberOfPressedPads())
        return releasePads;

    assert(PAD_CHECK(pad));

    switch(state)
    {
        case true:
        #ifdef DEBUG
        printf_P(PSTR("Editing pad %d\n"), pad);
        #endif

        editModeActivated = true;

        display.setupPadEditScreen(pad+1, getOctave(true), true);
        leds.displayActiveNoteLEDs(true, pad);
        break;

        case false:
        editModeActivated = false;
        display.setupHomeScreen();
        //after exiting from pad edit mode, restore note led states
        leds.displayActiveNoteLEDs();
        break;
    }

    return valueChanged;
}

///
/// \brief Enables or disables split mode.
/// @param [in] state Split state. Enabled if set to true, false otherwise.
/// \returns Result of setting up split mode (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setSplitState(bool state)
{
    if (pads.getNumberOfPressedPads())
        return releasePads;

    if (splitEnabled != state)
    {
        splitEnabled = state;
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), splitEnabled);
        getPadParameters();

        #ifdef DEBUG
        printf_P(PSTR("Split %s\n"), splitEnabled ? "on" : "off");
        #endif

        return valueChanged;
    }

    return noChange;
}

///
/// \brief Enables or disables sending of requested MIDI message.
/// @param [in] type MIDI message for which sending is being enabled or disabled. Enumerated type. See function_t enumeration.
/// @param [in] state If set to true, sending of requested MIDI message will be enabled. If set to false, sending will be disabled.
/// \returns Result of changing MIDI send state (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setMIDISendState(function_t type, bool state)
{
    uint16_t *variablePointer;
    uint16_t configurationID;
    uint8_t lastTouchedPad = getLastTouchedPad();

    switch(type)
    {
        case functionOnOffNotes:
        #ifdef DEBUG
        printf_P(PSTR("Notes "));
        #endif
        variablePointer = &noteSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID;
        break;

        case functionOnOffAftertouch:
        #ifdef DEBUG
        printf_P(PSTR("Aftertouch "));
        #endif
        variablePointer = &aftertouchSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID;
        break;

        case functionOnOffX:
        #ifdef DEBUG
        printf_P(PSTR("X "));
        #endif
        variablePointer = &xSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_X_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_X_ENABLE_ID;
        break;

        case functionOnOffY:
        #ifdef DEBUG
        printf_P(PSTR("Y "));
        #endif
        variablePointer = &ySendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_Y_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID;
        break;

        default:
        return outOfRange;
    }

    switch(splitEnabled)
    {
        case false:
        //global
        if (database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)) != state)
        {
            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
            for (int i=0; i<NUMBER_OF_PADS; i++)
                bitWrite(*variablePointer, i, state);
            #ifdef DEBUG
            printf_P(PSTR("%s for all pads\n"), state ? "on" : "off");
            #endif
        }
        else
        {
            return noChange;
        }
        break;

        case true:
        //local
        if (database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)lastTouchedPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)) != state)
        {
            database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)lastTouchedPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), state);
            bitWrite(*variablePointer, lastTouchedPad, state);
            #ifdef DEBUG
            printf_P(PSTR("%s for pad %d\n"), state ? "on" : "off", lastTouchedPad);
            #endif
        }
        else
        {
            return noChange;
        }
        break;
    }

    //extra checks
    if (!state && (type == functionOnOffNotes))
    {
        //if there are pressed pads, send notes off
        uint8_t startPad = splitEnabled ? lastTouchedPad : 0;
        uint8_t numberOfPads = splitEnabled ? 1 : NUMBER_OF_PADS;

        for (int i=startPad; i<numberOfPads; i++)
        {
            if (!isPadPressed(i))
                continue; //only send note off for released pads

            sendNotes(i, 0, false);
        }
    }

    return valueChanged;
}

///
/// \brief Changes aftertouch type.
/// @param [in] type New aftertouch type to be set (enumerated type). See aftertouchType_t enumeration.
/// \returns Result of changing aftertouch type (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setAftertouchType(aftertouchType_t type)
{
    if (pads.getNumberOfPressedPads())
        return releasePads;

    switch(type)
    {
        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return outOfRange; //wrong argument
    }

    if (database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_AFTERTOUCH_TYPE_ID) != type)
    {
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_AFTERTOUCH_TYPE_ID, (uint8_t)type);

        #ifdef DEBUG
        switch(type)
        {
            case aftertouchPoly:
            printf_P(PSTR("Changed aftertouch mode - Key aftertouch\n"));
            break;

            case aftertouchChannel:
            printf_P(PSTR("Changed aftertouch mode - Channel aftertouch\n"));
            break;

            default:
            break;
        }
        #endif

        aftertouchType = type;

        return valueChanged;
    }
    else
    {
        return noChange;
    }
}

///
/// \brief Changes MIDI channel on requested pad.
/// @param [in] pad Pad on which MIDI channel is being changed.
/// @param [in] channel Channel which is being applied to requested pad.
/// \returns Result of changing MIDI channel (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setMIDIchannel(int8_t pad, int8_t channel)
{
    if (pads.getNumberOfPressedPads())
        return releasePads;

    assert(PAD_CHECK(pad));

    //input validation
    if (channel < 1)
        channel = 1;
    else if (channel > 16)
        channel = 16;

    if (channel != midiChannel[pad])
    {
        if (!splitEnabled)
        {
            //apply to all pads
            for (int i=0; i<NUMBER_OF_PADS; i++)
                midiChannel[i] = channel;

            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram, channel);
        }
        else
        {
            //apply to single pad only
            midiChannel[pad] = channel;
            database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*pad+LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), channel);
        }

        return valueChanged;
    }

    return noChange;
}

///
/// \brief Changes velocity sensitivity.
/// @param [in] type Velocity sensitivity (enumerated type). See velocitySensitivity_t enumeration.
/// \returns Result of changing velocity sensitivity (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setVelocitySensitivity(velocitySensitivity_t sensitivity)
{
    if (velocitySensitivity != sensitivity)
    {
        velocitySensitivity = sensitivity;
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_SENSITIVITY_ID, sensitivity);

        //make sure to apply new values
        getPressureLimits();

        return valueChanged;
    }
    else
    {
        return noChange;
    }
}

///
/// \brief Changes velocity curve.
/// @param [in] curve Velocity curve (enumerated type). See curve_t enumeration.
/// \note Only Linear up, Log Up 1 and Exponential curves are allowed.
/// \returns Result of changing velocity curve (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setVelocityCurve(curve_t curve)
{
    switch(curve)
    {
        case curve_linear_up:
        case curve_log_up_1:
        case curve_exp_up:
        //this is fine
        break;

        default:
        return outOfRange;
    }

    if (velocityCurve != curve)
    {
        velocityCurve = curve;
        database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_CURVE_ID, curve);
        return valueChanged;
    }
    else
    {
        return noChange;
    }
}

///
/// \brief Changes curve for CC MIDI messages on requested coordinate.
/// @param [in] coordinate Coordinate on which curve is being changed (enumerated type). See padCoordinate_t enumeration.
/// Only X and Y coordinates are allowed.
/// @param [in] curve Curve which is being applied to requested coordinate (enumerated type). See curve_t enumeration.
/// \returns Result of changing CC curve (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setCCcurve(padCoordinate_t coordinate, int16_t curve)
{
    //input validation
    if (curve < 0)
        curve = 0;
    else if (curve >= NUMBER_OF_CURVES)
        curve = NUMBER_OF_CURVES-1;

    uint8_t startPad = !splitEnabled ? 0 : getLastTouchedPad();
    uint8_t *variablePointer;
    uint16_t configurationID;

    switch(coordinate)
    {
        case coordinateX:
        variablePointer = (uint8_t*)padCurveX;
        configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID : (uint16_t)LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID;
        break;

        case coordinateY:
        variablePointer = (uint8_t*)padCurveY;
        configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID : (uint16_t)LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID;
        break;

        default:
        return outOfRange;
    }

    if (curve == variablePointer[startPad])
        return noChange;

    #ifdef DEBUG
    if (coordinate == coordinateX)
        printf_P(PSTR("X curve for "));
    else if (coordinate == coordinateY)
        printf_P(PSTR("Y curve for "));
    #endif

    switch(splitEnabled)
    {
        case true:
        //local
        database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), curve);
        variablePointer[startPad] = curve;
        #ifdef DEBUG
        printf_P(PSTR("pad %d: %d\n"), startPad, curve);
        #endif
        break;

        case false:
        //global
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), curve);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            variablePointer[i] = curve;
        #ifdef DEBUG
        printf_P(PSTR("all pads: %d\n"), curve);
        #endif
        break;
    }

    return valueChanged;
}

///
/// \brief Changes CC value on requested coordinate and specified pad.
/// @param [in] coordinate Coordinate on which CC value is being changed (enumerated type). See padCoordinate_t enumeration.
/// Only X and Y coordinates are allowed.
/// @param [in] cc CC value which is being applied to requested coordinate (enumerated type). See curve_t enumeration.
/// \returns Result of changing CC value (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setCCvalue(padCoordinate_t coordinate, int16_t cc)
{
    //input validation
    if (cc < 0)
        cc = 0;
    else if (cc > 124)
        cc = 124;

    uint8_t startPad = !splitEnabled ? 0 : getLastTouchedPad();
    uint8_t *variablePointer;
    uint16_t configurationID;

    switch (coordinate)
    {
        case coordinateX:
        variablePointer = ccXPad;
        configurationID = !splitEnabled ? (uint8_t)GLOBAL_PROGRAM_SETTING_CC_X_ID : (uint8_t)LOCAL_PROGRAM_SETTING_CC_X_ID;
        break;

        case coordinateY:
        variablePointer = ccYPad;
        configurationID = !splitEnabled ? (uint8_t)GLOBAL_PROGRAM_SETTING_CC_Y_ID : (uint8_t)LOCAL_PROGRAM_SETTING_CC_Y_ID;
        break;

        default:
        return outOfRange;
    }

    if (cc == variablePointer[startPad])
        return noChange;

    #ifdef DEBUG
    if (coordinate == coordinateX)
        printf_P(PSTR("X CC for "));
    else if (coordinate == coordinateY)
        printf_P(PSTR("Y CC for "));
    #endif

    switch(splitEnabled)
    {
        case true:
        //local
        database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), cc);
        variablePointer[startPad] = cc;
        #ifdef DEBUG
        printf_P(PSTR("pad %d: %d\n"), startPad, cc);
        #endif
        break;

        case false:
        //global
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), cc);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            variablePointer[i] = cc;
        #ifdef DEBUG
        printf_P(PSTR("all pads: %d\n"), cc);
        #endif
        break;
    }

    return valueChanged;
}

///
/// \brief Changes CC limit value (minimum or maximum) on requested coordinate and specified pad.
/// @param [in] coordinate Coordinate on which CC limit value is being changed (enumerated type). See padCoordinate_t enumeration.
/// Only X and Y coordinates are allowed.
/// @param [in] limitType CC limit type - minimum or maximum. Enumerated type. See limitType_t enumeration.
/// @param [in] value CC limit value which is being applied to requested coordinate (enumerated type). See curve_t enumeration.
/// \returns True on success, false otherwise.
///
changeResult_t Pads::setCClimit(padCoordinate_t coordinate, limitType_t limitType, int16_t value)
{
    //input validation
    if (value < 0)
        value = 0;
    else if (value > 127)
        value = 127;

    uint8_t lastPressedPad = getLastTouchedPad();
    uint8_t startPad = !splitEnabled ? 0 : lastPressedPad;
    uint16_t configurationID;
    uint8_t *variablePointer;

    switch(coordinate)
    {
        case coordinateX:
        #ifdef DEBUG
        printf_P(PSTR("X "));
        #endif

        switch(limitType)
        {
            case limitTypeMax:
            #ifdef DEBUG
            printf_P(PSTR("max for "));
            #endif
            variablePointer = ccXmaxPad;
            configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_X_MAX_ID : (uint16_t)LOCAL_PROGRAM_SETTING_X_MAX_ID;
            break;

            case limitTypeMin:
            #ifdef DEBUG
            printf_P(PSTR("min for "));
            #endif
            variablePointer = ccXminPad;
            configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_X_MIN_ID : (uint16_t)LOCAL_PROGRAM_SETTING_X_MIN_ID;
            break;

            default:
            return outOfRange;
        }
        break;

        case coordinateY:
        #ifdef DEBUG
        printf_P(PSTR("Y "));
        #endif
        switch(limitType)
        {
            case limitTypeMax:
            #ifdef DEBUG
            printf_P(PSTR("max for "));
            #endif
            variablePointer = ccYmaxPad;
            configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_Y_MAX_ID : (uint16_t)LOCAL_PROGRAM_SETTING_Y_MAX_ID;
            break;

            case limitTypeMin:
            #ifdef DEBUG
            printf_P(PSTR("min for "));
            #endif
            variablePointer = ccYminPad;
            configurationID = !splitEnabled ? (uint16_t)GLOBAL_PROGRAM_SETTING_Y_MIN_ID : (uint16_t)LOCAL_PROGRAM_SETTING_Y_MIN_ID;
            break;

            default:
            return outOfRange;
        }
        break;

        default:
        return outOfRange;
    }

    if (value == variablePointer[startPad])
        return noChange;

    switch(splitEnabled)
    {
        case true:
        //local
        database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), value);
        variablePointer[startPad] = value;
        #ifdef DEBUG
        printf_P(PSTR("pad %d: %d\n"), startPad, value);
        #endif
        break;

        case false:
        //global
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), value);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            variablePointer[i] = value;
        #ifdef DEBUG
        printf_P(PSTR("all pads: %d\n"), value);
        #endif
        break;
    }

    return valueChanged;
}

///
/// \brief Enables or disables calibration mode for requested coordinate.
/// @param [in] state Calibration is enabled if set to true, disabled otherwise.
/// @param [in] type Coordinate on which calibration is being enabled or disabled.
///
void Pads::setCalibrationMode(bool state, padCoordinate_t type)
{
    calibrationEnabled = state;
    activeCalibration = type;
}

///
/// \brief Writes new upper calibration value for pressure on specified pad pressure zone.
/// @param [in] pad Pad for which new calibration value is being written.
/// @param [in] pressureZone Pressure zone which is being updated.
/// @param [in] limit New calibration value (0-1023).
/// \returns Result of changing calibration value (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::calibratePressure(int8_t pad, uint8_t pressureZone, int16_t limit)
{
    assert(RAW_ANALOG_VALUE_CHECK(limit));
    assert(PAD_CHECK(pad));

    if (database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection0+pressureZone, pad) != limit)
    {
        database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection0+pressureZone, pad, limit);
        getPressureLimits();
        getAftertouchLimits();
        return valueChanged;
    }
    else
    {
        return noChange;
    }
}

///
/// \brief Writes new lower or upper calibration value for X or Y coordinate on specified pad.
/// @param [in] pad Pad for which new calibration value is being written.
/// @param [in] type Pad coordinate (X or Y). Enumerated type. See padCoordinate_t enumeration.
/// @param [in] limitType Lower or upper calibration value (enumerated type). See limitType_t enumeration.
/// @param [in] limit New calibration value (0-1023).
/// \returns Result of changing calibration value (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::calibrateXY(int8_t pad, padCoordinate_t type, limitType_t limitType, int16_t limit)
{
    assert(RAW_ANALOG_VALUE_CHECK(limit));
    assert(PAD_CHECK(pad));

    uint16_t *variablePointer;
    uint8_t configurationSection;

    switch(type)
    {
        case coordinateX:
        switch(limitType)
        {
            case limitTypeMin:
            variablePointer = padXLimitLower;
            configurationSection = padCalibrationXlowerSection;
            break;

            case limitTypeMax:
            variablePointer = padXLimitUpper;
            configurationSection = padCalibrationXupperSection;
            break;

            default:
            return outOfRange;
        }
        break;

        case coordinateY:
        switch(limitType)
        {
            case limitTypeMin:
            variablePointer = padYLimitLower;
            configurationSection = padCalibrationYlowerSection;
            break;

            case limitTypeMax:
            variablePointer = padYLimitUpper;
            configurationSection = padCalibrationYupperSection;
            break;

            default:
            return outOfRange;
        }
        break;

        default:
        return outOfRange;
    }

    if ((uint16_t)limit != variablePointer[pad])
    {
        variablePointer[pad] = limit;
        database.update(DB_BLOCK_PAD_CALIBRATION, configurationSection, (uint16_t)pad, limit);
        return valueChanged;
    }

    return noChange;
}

///
/// \brief Adds or removes note to requested pad.
/// @param [in] pad Pad to which note is being added.
/// @param [in] note MIDI note to add (enumerated type). See note_t enumeration.
/// \returns Result of adding note to pad (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setPadNote(int8_t pad, note_t note)
{
    assert(PAD_CHECK(pad));

    uint16_t noteIndex = 0;
    //calculate note to be added or removed
    uint8_t newNote = getOctave(true)*MIDI_NOTES + (uint8_t)note;

    if (newNote > 127)
        return outOfRange;

    //note can added or removed, assume adding by default
    bool addOrRemove = true;

    //check if calculated note is already assigned to pad
    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] == newNote)
        {
            //note is already present - remove it
            addOrRemove = false;
            noteIndex = i;
            break;
        }
    }

    uint16_t noteID = ((uint16_t)activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

    //if it isn't, add it
    if (addOrRemove)
    {
        //get number of assigned notes to last touched pad
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] != BLANK_NOTE)
            noteIndex++;
        }

        //pads cannot have more than NOTES_PER_PAD notes
        if (noteIndex == NOTES_PER_PAD)
            return notAllowed;

        //assign new pad note
        padNote[pad][noteIndex] = newNote;
        database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+noteIndex+(NOTES_PER_PAD*(uint16_t)pad), newNote);

        #ifdef DEBUG
        printf_P(PSTR("Adding note "));
        #endif
    }
    else
    {
        //delete note (assign BLANK_NOTE)
        newNote = BLANK_NOTE;

        //assign new pad note
        padNote[pad][noteIndex] = newNote;

        //copy note array
        uint8_t tempNoteArray[NOTES_PER_PAD];

        for (int i=0; i<NOTES_PER_PAD; i++)
            tempNoteArray[i] = padNote[pad][i];

        //shift all notes so that BLANK_NOTE is at the end of array
        for (int i=noteIndex; i<(NOTES_PER_PAD-1); i++)
            padNote[pad][i] = tempNoteArray[i+1];

        padNote[pad][NOTES_PER_PAD-1] = BLANK_NOTE;

        for (int i=0; i<NOTES_PER_PAD; i++)
            database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+i+(NOTES_PER_PAD*(uint16_t)pad), padNote[pad][i]);

        #ifdef DEBUG
        printf_P(PSTR("Removing note "));
        #endif
    }

    #ifdef DEBUG
    printf_P(PSTR("%d, to pad %d"), (uint8_t)note, pad);
    #endif

    return valueChanged;
}

///
/// \brief Shifts scale up or down by requested number of octaves or changes active octave during pad edit mode.
/// @param [in] octave New octave which is being applied to current scale.
/// @param [in] padEditMode If set to true, active octave will be changed, but actual notes won't be changed at all.
/// This is used during pad edit mode to change the octave of notes which are being added or removed from pad.
/// \returns Result of changing octave (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setOctave(int8_t octave, bool padEditMode)
{
    if (pads.getNumberOfPressedPads())
        return releasePads;

    //safety checks
    if (octave < 0)
        octave = 0;
    else if (octave >= MIDI_NOTES)
        octave = MIDI_NOTES-1;

    #ifdef DEBUG
    printf_P(PSTR("Trying to setup octave %d.\n"), octave);
    #endif

    if (padEditMode)
    {
        activePadEditOctave = octave;
    }
    else
    {
        //all pads should shift notes immediately, except for the ones that are still pressed
        //pressed pads should shift note on release

        int8_t activeOctave = getOctave();

        //new octave is actually the difference between received argument and currently active octave
        int8_t difference = octave - activeOctave;

        if (!difference)
        {
            //no change
            #ifdef DEBUG
            printf_P(PSTR("No change in octave.\n"));
            #endif
            return noChange;
        }
        else
        {
            bool changeAllowed = true;
            bool direction = difference > 0;

            //check if notes are too low/high if octave is changed
            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (padNote[i][j] != BLANK_NOTE)
                    {
                        if (direction)
                        {
                            if (padNote[i][j]+MIDI_NOTES*abs(difference) > 127)
                            {
                                changeAllowed = false;
                                break;
                            }
                        }
                        else
                        {
                            if (padNote[i][j] < MIDI_NOTES*abs(difference))
                            {
                                changeAllowed = false;
                                break;
                            }
                        }
                    }

                    if (!changeAllowed)
                        break;
                }

                if (!changeAllowed)
                    break;
            }

            if (!changeAllowed)
            {
                #ifdef DEBUG
                printf_P(PSTR("Unable to perform octave change: one or more pad notes are too %s.\n"), direction ? "high" : "low");
                #endif
                return outOfRange;
            }
            else
            {
                if (isPredefinedScale(activeScale))
                {
                    //predefined scale
                    for (int i=0; i<NUMBER_OF_PADS; i++)
                    {
                        uint8_t newNote = padNote[i][0] + MIDI_NOTES*difference;

                        #ifdef DEBUG
                        printf_P(PSTR("New note for pad %d: %d\n"), i, newNote);
                        #endif

                        padNote[i][0] = newNote;
                    }

                    uint16_t octaveIndex_predefinedScale = PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale;
                    database.update(DB_BLOCK_SCALE, scalePredefinedSection, octaveIndex_predefinedScale, octave);
                }
                else
                {
                    //user scale
                    uint16_t noteID = ((uint16_t)activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);
                    uint8_t newNote;

                    for (int i=0; i<NUMBER_OF_PADS; i++)
                    {
                        #ifdef DEBUG
                        printf_P(PSTR("New notes for pad %d: "), i);
                        #endif

                        for (int j=0; j<NOTES_PER_PAD; j++)
                        {
                            if (padNote[i][j] != BLANK_NOTE)
                            {
                                newNote = padNote[i][j] + MIDI_NOTES*difference;
                                database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i), newNote, true);

                                #ifdef DEBUG
                                printf_P(PSTR("%d "), newNote);
                                #endif

                                padNote[i][j] = newNote;
                            }
                        }

                        #ifdef DEBUG
                        printf_P(PSTR("\n"));
                        #endif
                    }
                }
            }
        }

        //#ifdef DEBUG
        //printf_P(PSTR("Octave successfully set. Printing out all notes:\n"));
        //for (int i=0; i<NUMBER_OF_PADS; i++)
        //{
            //for (int j=0; j<NOTES_PER_PAD; j++)
                //printf_P(PSTR("Note %d for pad %: %d\n"), j, i, padNote[i][j]);
//
            //printf_P(PSTR("\n"));
        //}
        //#endif
    }

    return valueChanged;
}

///
/// \brief Shift entire predefined scale up or down without changing tonic (root note).
/// @param [in] shiftLevel Scale shift level (can be positive or negative).
/// @param [in] internalChange  If set to true, function is called from within Pads class and certain values are set differently.
/// This should never be called with true value outside of Pads class (set to false by default).
/// \returns Result of shifting scale (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::setScaleShiftLevel(int8_t shiftLevel, bool internalChange)
{
    //shift entire scale up or down
    //tonic remains the same, it just gets shifted to other pad

    if (isUserScale(activeScale))
        return notPredefinedScale;

    if (pads.getNumberOfPressedPads())
        return releasePads;

    #ifdef DEBUG
    printf_P(PSTR("Trying to set note shift level %d\n"), shiftLevel);
    #endif

    uint8_t scaleNotes = getPredefinedScaleNotes(activeScale);
    int16_t tempNoteArray_1[NUMBER_OF_PADS];
    int16_t tempNoteArray_2[NUMBER_OF_PADS];
    bool direction = shiftLevel >= 0;
    shiftLevel = abs(shiftLevel);

    for (int i=0; i<NUMBER_OF_PADS; i++)
        tempNoteArray_2[i] = padNote[i][0];

    if (direction)
    {
        //up
        for (int i=0; i<shiftLevel; i++)
        {
            //last note gets increased, other notes get shifted down
            tempNoteArray_1[NUMBER_OF_PADS-1] = tempNoteArray_2[NUMBER_OF_PADS-scaleNotes] + MIDI_NOTES*abs(shiftLevel);

            if (tempNoteArray_1[NUMBER_OF_PADS-1] > 127)
                return outOfRange;

            for (int j=0; j<NUMBER_OF_PADS-1; j++)
                tempNoteArray_1[j] = tempNoteArray_2[j+1];

            if (!internalChange)
                noteShiftLevel++;

            for (int j=0; j<NUMBER_OF_PADS; j++)
                tempNoteArray_2[j] = tempNoteArray_1[j];
        }
    }
    else
    {
        //down
        for (int i=0; i<abs(shiftLevel); i++)
        {
            //first note gets decreased, other notes get shifted up
            tempNoteArray_1[0] = tempNoteArray_2[scaleNotes-1] - MIDI_NOTES*abs(shiftLevel);

            if (tempNoteArray_1[0] < 0)
                return outOfRange;

            for (int j=0; j<NUMBER_OF_PADS-1; j++)
                tempNoteArray_1[j+1] = tempNoteArray_2[j];

            if (!internalChange)
                noteShiftLevel--;

            for (int j=0; j<NUMBER_OF_PADS; j++)
                tempNoteArray_2[j] = tempNoteArray_1[j];
        }
    }

    if (!internalChange)
    {
        if (abs(noteShiftLevel) == scaleNotes)
            noteShiftLevel = 0;
    }

    if (!internalChange)
        database.update(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale, noteShiftLevel);

    #ifdef DEBUG
    printf_P(PSTR("Notes shifted %s"), direction ? "up\n" : "down\n");
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
        padNote[i][0] = tempNoteArray_1[i];

    return valueChanged;
}

///
/// \brief Updates state of pad (pressed or released)
/// @param [in] pad Pad for which press state is being changed.
/// @param [in] state New pad press state (pressed if true, false otherwise).
///
void Pads::setPadPressState(int8_t pad, bool state)
{
    assert(PAD_CHECK(pad));

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bitWrite(padPressed, pad, state);
    }
}

///
/// \brief Sets correct state on function LEDs if split mode is enabled.
/// @param [in] pad Pad for which functions should be checked.
/// \returns Result of updating function LEDs (enumerated type). See changeResult_t enumeration.
///
changeResult_t Pads::updateFunctionLEDs(int8_t pad)
{
    assert(PAD_CHECK(pad));

    if (!splitEnabled)
        return notAllowed;

    //turn off function LEDs first
    leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
    leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
    leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
    leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);

    //turn on feature LEDs depending on enabled features for last touched pad
    leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getMIDISendState(pad, functionOnOffAftertouch) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_NOTES, getMIDISendState(pad, functionOnOffNotes) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_X, getMIDISendState(pad, functionOnOffX) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_Y, getMIDISendState(pad, functionOnOffY) ? ledStateFull : ledStateOff);

    return valueChanged;
}

///
/// \brief Sets correct state on note LEDs.
/// Used only when pad state has been changed.
/// @param [in] pad Pad for which state should be checked.
/// @param [in] state State of pad (true if pressed, false if released).
///
void Pads::updateNoteLEDs(int8_t pad, bool state)
{
    assert(PAD_CHECK(pad));

    uint8_t noteArray[NOTES_PER_PAD],
    noteCounter = 0;

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] != BLANK_NOTE)
        {
            noteArray[noteCounter] = padNote[pad][i];
            noteCounter++;
        }
    }

    switch(state)
    {
        case true:
        //note on
        uint8_t tonicArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++)
        {
            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            leds.setNoteLEDstate((note_t)tonicArray[i], ledStateFull);
        }
        break;

        case false:
        //note off
        //we need to set LEDs back to dim states for released pad, but only if
        //some other pad with same active note isn't already pressed
        bool noteActive;

        for (int z=0; z<noteCounter; z++)
        {
            //iterate over every note on released pad
            noteActive = false;

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                if (!isPadPressed(i))
                    continue; //skip released pad
                if (i == pad)
                    continue; //skip current pad

                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (getTonicFromNote(padNote[i][j]) == getTonicFromNote(noteArray[z]))
                        noteActive = true;
                }
            }

            if (!noteActive)
                leds.setNoteLEDstate(getTonicFromNote((note_t)noteArray[z]), ledStateDim);
        }
        break;
    }
}

///
/// \brief Updates press state for requested pad.
/// @param [in] pad Pad for which press state is being updated.
/// @param [in] state New pad state.
///
void Pads::updateLastPressedPad(int8_t pad, bool state)
{
    assert(PAD_CHECK(pad));

    uint8_t pressedPads = 0;

    if (state)
    {
        //pad is pressed, add it to touch history buffer
        if (pad != getLastTouchedPad())
        {
            //store currently pressed pad in buffer

            for (int i=0; i<NUMBER_OF_PADS; i++)
            if (isPadPressed(i))
            pressedPads++;

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
    }
    else
    {
        //pad released, clear it from buffer

        for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i))
        pressedPads++;

        if (pressedPads < 1)
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
}

///
/// \brief Saves current time into buffer so that notes for requested pad are sent after defined amount of time.
/// @param [in] pad Pad for which a note send event is being created.
///
void Pads::storeNotes(int8_t pad)
{
    assert(PAD_CHECK(pad));

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
    pad_note_timer_buffer[i] = rTimeMs();
    note_buffer_head = i;
}

///
/// \brief Resets predefined scale to its default notes without setting tonic, shift level or octave.
///
void Pads::resetPredefinedScale()
{
    uint8_t notesPerScale = getPredefinedScaleNotes(activeScale);
    uint8_t noteCounter = 0;

    for (int i=0; i<notesPerScale; i++)
    {
        padNote[i][0] = getScaleNote(activeScale, i);
        noteCounter++;
    }

    if (notesPerScale < NUMBER_OF_PADS)
    {
        noteCounter = 0;

        for (int i=notesPerScale; i<NUMBER_OF_PADS; i++)
        {
            padNote[i][0] = getScaleNote(activeScale, noteCounter);
            //these notes are actually in another octave
            padNote[i][0] += MIDI_NOTES;
            noteCounter++;
        }
    }

    //finally, reset all notes on pads except first one
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=1; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;
    }
}