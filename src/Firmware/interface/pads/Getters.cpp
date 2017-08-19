#include "Pads.h"
#include "../leds/LEDs.h"
#include "../../database/Database.h"
#include "PredefinedScales.h"

///
/// \ingroup interfacePads
/// @{

///
/// \brief Checks last pad which has been pressed.
/// \returns Last pressed pad (0 - NUMBER_OF_PADS-1)
///
uint8_t Pads::getLastTouchedPad()
{
    return padPressHistory_buffer[padPressHistory_counter];
}

///
/// \brief Checks if requested pad is currently pressed.
/// @param [in] pad     Pad which is being checked.
/// \returns True if requested pad is pressed, false otherwise,
///
bool Pads::isPadPressed(int8_t pad)
{
    assert(SCALE_CHECK(pad));

    bool value;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        value = bitRead(padPressed, pad);
    }

    return value;
}

///
/// \brief Checks how many pads are pressed.
/// \returns Number of pressed pads.
///
uint8_t Pads::getNumberOfPressedPads()
{
    uint8_t pressedPads = 0;

    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i))
            pressedPads++;

    return pressedPads;
}

///
/// \brief Checks for currently active program.
/// \returns Currently active program (0 - NUMBER_OF_PROGRAMS-1)
///
uint8_t Pads::getProgram()
{
    return activeProgram;
}

///
/// \brief Checks for currently active scale.
/// \returns Currently active scale (0 - PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1).
///
uint8_t Pads::getScale()
{
    return activeScale;
}

///
/// \brief Checks if requested scale is user scale.
/// @param [in] scale   Scale which is being checked.
/// \returns True if requested scale is user scale, false otherwise (predefined scale).
///
bool Pads::isUserScale(int8_t scale)
{
    assert(SCALE_CHECK(scale));

    return (scale >= PREDEFINED_SCALES);
}

///
/// \brief Checks if requested scale is predefined scale.
/// @param [in] scale   Scale which is being checked.
/// \returns True if requested scale is predefined scale, false otherwise (user scale).
///
bool Pads::isPredefinedScale(int8_t scale)
{
    assert(SCALE_CHECK(scale));

    return (scale < PREDEFINED_SCALES);
}

///
/// \brief Checks if pad editing is in progress.
/// \returns True if pad edit mode is enabled, false otherwise.
///
bool Pads::getEditModeState()
{
    return editModeState;
}

///
/// \brief Finds currently active tonic.
/// In user scales, tonic is first found note on first pad. If no notes are found, invalid value is returned (MIDI_NOTES).
/// In predefined scales, tonic is written in EEPROM since it's configurable.
/// \returns Currently active tonic (enumerated type). See note_t enumeration.
///
note_t Pads::getTonic()
{
    if (isUserScale(activeScale))
    {
        //tonic is first found note on first pad on user scales
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[0][i] != BLANK_NOTE)
                return getTonicFromNote(padNote[0][i]);
        }

        return MIDI_NOTES;
    }
    else
    {
        //predefined scale tonic is written in eeprom
        uint16_t tonicIndex = PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale;
        return (note_t)database.read(DB_BLOCK_SCALE, scalePredefinedSection, tonicIndex);
    }
}

///
/// \brief Checks for currently active octave.
/// @param [in] padEditMode     If set to true, active octave used in pad edit mode is returned.
///                             Otherwise, calculated octave is returned based on active pad notes.
/// \returns Currently active octave (0 - MIDI_NOTES-1).
///
uint8_t Pads::getOctave(bool padEditMode)
{
    if (padEditMode)
    {
        return activePadEditOctave;
    }
    else
    {
        if (isPredefinedScale(activeScale))
        {
            //in predefined scales octave is always calculated using first note on first pad
            return getOctaveFromNote(padNote[0][0]);
        }
        else
        {
            //in user scales octave is calculated based on first found note
            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (padNote[i][j] != BLANK_NOTE)
                    {
                        return getOctaveFromNote(padNote[i][j]);
                    }
                }
            }
        }
    }

    //no notes found, return default octave
    return DEFAULT_OCTAVE;
}

///
/// \brief Checks for whether split function is active or not.
/// \returns True if split is enabled, false otherwise.
///
bool Pads::getSplitState()
{
    return splitEnabled;
}

///
/// \brief Checks for whether certain MIDI functionality (aftertouch, notes, CC/X and CC/Y) is enabled or disabled on requested pad.
/// @param [in] pad     Pad on which check is performed.
/// @param [in] type    MIDI functionality (enumerated type). See onOff_t enumeration.
/// \returns True if functionality is enabled, false otherwise.
///
bool Pads::getMIDISendState(int8_t pad, function_t type)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case functionOnOffAftertouch:
        return bitRead(aftertouchSendEnabled, pad);

        case functionOnOffNotes:
        return bitRead(noteSendEnabled, pad);

        case functionOnOffX:
        return bitRead(xSendEnabled, pad);

        case functionOnOffY:
        return bitRead(ySendEnabled, pad);

        default:
        return false;
    }
}

///
/// \brief Checks for currently active aftertouch type (channel or polyphonic).
/// \returns Currently active aftertouch type (enumerated type). See aftertouchType_t enumeration.
///
aftertouchType_t Pads::getAftertouchType()
{
    return (aftertouchType_t)aftertouchType;
}

///
/// \brief Gets note from pad.
/// @param [in] pad         Pad from which note is returned.
/// @param [in] noteIndex   Index of pad note.
/// \returns Note from requested index on requested pad (0 - 127).
///
uint8_t Pads::getPadNote(int8_t pad, int8_t noteIndex)
{
    assert(PAD_CHECK(pad));
    assert(NOTE_ASSIGN_CHECK(noteIndex));

    return padNote[pad][noteIndex];
}

///
/// \brief Checks if requested note is assigned to any pad.
/// @param [in] note    Note which is being checked.
/// \returns True if note is assigned to any pad, false otherwise.
///
bool Pads::isNoteAssigned(note_t note)
{
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<NOTES_PER_PAD; j++)
        {
            if (padNote[i][j] != BLANK_NOTE)
            {
                if (getTonicFromNote(padNote[i][j]) == note)
                    return true;
            }
        }
    }

    return false;
}

///
/// \brief Calculates tonic (root note) from received MIDI note.
/// @param [in] note    MIDI note.
/// \returns Calculated tonic/root note (enumerated type). See note_t enumeration.
///
note_t Pads::getTonicFromNote(int8_t note)
{
    if (note == BLANK_NOTE)
        return MIDI_NOTES;

    return (note_t)(note % MIDI_NOTES);
}

///
/// \brief Calculates octave from received MIDI note.
/// @param [in] note    MIDI note.
/// \returns Calculated octave (0 - MIDI_NOTES-1).
///
uint8_t Pads::getOctaveFromNote(int8_t note)
{
    if (note == BLANK_NOTE)
        return MIDI_NOTES;

    return note / MIDI_NOTES;
}

///
/// \brief Checks for amount of scale shift.
/// Notes in scale can be shifted in positive or negative direction.
/// \returns Current amount of scale shift.
///
int8_t Pads::getScaleShiftLevel()
{
    return noteShiftLevel;
}

///
/// \brief Checks for assigned CC value on requested coordinate and pad.
/// @param [in] pad     Pad on which CC value is being checked.
/// @param [in] type    Coordinate from which CC value is requested (X or Y) (enumerated type). See padCoordinate_t enumeration.
///
uint8_t Pads::getCCvalue(int8_t pad, padCoordinate_t type)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case coordinateX:
        return ccXPad[pad];

        case coordinateY:
        return ccYPad[pad];

        default:
        return 0;
    }
}

///
/// \brief Checks for minimum or maximum CC value on requested coordinate and pad.
/// @param [in] pad         Pad which is being checked.
/// @param [in] type        Coordinate on which CC limit is being checked (enumerated type) - only X or Y
///                         coordinates are allowed. See padCoordinate_t enumeration.
/// @param [in] limitType   Limit which is being checked (minimum or maximum - enumerated type). See limitType_t enumeration.
/// \returns CC limit (0-127).
///
uint8_t Pads::getCClimit(int8_t pad, padCoordinate_t type, limitType_t limitType)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case coordinateX:
        if (limitType == limitTypeMax)
            return ccXmaxPad[pad];
        else if (limitType == limitTypeMin)
            return ccXminPad[pad];
        else
            return 0;

        case coordinateY:
        if (limitType == limitTypeMax)
            return ccYmaxPad[pad];
        else if (limitType == limitTypeMin)
            return ccYminPad[pad];
        else
            return 0;

        default:
        return 0;
    }
}

///
/// \brief Checks for CC curve on requested coordinate and pad.
/// @param [in] pad     Pad which is being checked.
/// @param [in] type    Coordinate on which CC curve is being checked (enumerated type) - only X or Y
///                     coordinates are allowed. See padCoordinate_t enumeration.
///
curve_t Pads::getCCcurve(int8_t pad, padCoordinate_t coordinate)
{
    assert(PAD_CHECK(pad));

    switch(coordinate)
    {
        case coordinateX:
        return (curve_t)padCurveX[pad];

        case coordinateY:
        return (curve_t)padCurveY[pad];

        default:
        return curve_linear_up;
    }
}

///
/// \brief Checks for currently active pressure sensitivity setting.
/// \returns Active pressure sensitivity (enumerated type). See pressureSensitivity_t enumeration.
///
velocitySensitivity_t Pads::getVelocitySensitivity()
{
    return velocitySensitivity;
}

///
/// \brief Checks for currently active velocity curve.
/// Possible velocity curves are curve_linear_up, curve_log_up_1 and curve_exp_up.
/// \returns Active velocity curve (enumerated type). See curve_t enumeration.
///
curve_t Pads::getVelocityCurve()
{
    return velocityCurve;
}

///
/// \brief Checks currently assigned MIDI channel on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns MIDI channel on requested pad (1-16).
///
uint8_t Pads::getMIDIchannel(int8_t pad)
{
    assert(PAD_CHECK(pad));

    return midiChannel[pad];
}

///
/// \brief Checks if calibration mode is active.
/// \returns True if calibration is active, false otherwise.
///
bool Pads::isCalibrationEnabled()
{
    return calibrationEnabled;
}

///
/// \brief Checks for currently active calibration mode.
/// \returns Currently active calibration mode (enumerated type). See padCoordinate_t enumeration.
///
padCoordinate_t Pads::getCalibrationMode()
{
    return activeCalibration;
}

///
/// \brief Checks for minimum or maximum raw (calibration) limit on requested pad and coordinate.
/// @param [in] pad         Pad which is being checked.
/// @param [in] coordinate  Coordinate which is being checked (enumerated type). See padCoordinate_t enumeration.
/// @param [in] limitType   Limit which is being checked (minimum or maximum - enumerated type). See limitType_t enumeration.
///
uint16_t Pads::getCalibrationLimit(int8_t pad, padCoordinate_t coordinate, limitType_t limitType)
{
    assert(PAD_CHECK(pad));

    switch(coordinate)
    {
        case coordinateX:
        return (limitType == limitTypeMin) ? padXLimitLower[pad] : padXLimitUpper[pad];
        break;

        case coordinateY:
        return (limitType == limitTypeMin) ? padYLimitLower[pad] : padYLimitUpper[pad];
        break;

        default:
        return 0;
    }
}

///
/// \brief Checks if aftertouch is activated on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns True if aftertouch is activated on requested pad, false otherwise.
///
bool Pads::isAftertouchActivated(int8_t pad)
{
    assert(PAD_CHECK(pad));

    return bitRead(aftertouchActivated, pad);
}

///
/// \brief Checks for currently active pressure zone on requested pad.
/// @param [in] pad     Pad which is being checked.
/// \returns Currently active pressure zone (16 zones in total).
///
padCalibrationSection Pads::getPressureZone(int8_t pad)
{
    assert(PAD_CHECK(pad));

    //find out pressure zone
    int16_t scaledX = getScaledXY(pad, board.getPadX(pad), coordinateX, false);
    int16_t scaledY = getScaledXY(pad, board.getPadY(pad), coordinateY, false);

    uint8_t row = PRESSURE_CALIBRATION_Y_ZONES - 1 - (scaledY / PRESSURE_CALIBRATION_MAX_Y_ZONE_VALUE);
    //invert
    uint8_t column = scaledX / PRESSURE_CALIBRATION_MAX_X_ZONE_VALUE;

    return (padCalibrationSection)(column + row*PRESSURE_CALIBRATION_X_ZONES);
}

///
/// \brief Initializes all global, pad, scale and program data by reading values from database.
///
void Pads::getConfiguration()
{
    //globals
    aftertouchType = (aftertouchType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_AFTERTOUCH_TYPE_ID);
    velocitySensitivity = (velocitySensitivity_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_SENSITIVITY_ID);
    velocityCurve = (curve_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_CURVE_ID);
    pitchBendType = (pitchBendType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_PITCH_BEND_TYPE_ID);

    //read pad configuration from EEPROM
    getProgramParameters();
    getPadLimits();
}

///
/// \brief Initializes all program data by reading values from database.
///
void Pads::getProgramParameters()
{
    #ifdef DEBUG
    printf_P(PSTR("----------------------\nPrinting out program settings\n----------------------\n"));
    #endif

    activeProgram = database.read(DB_BLOCK_PROGRAM, programLastActiveProgramSection, 0);
    splitEnabled = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));

    #ifdef DEBUG
    printf_P(PSTR("Active program: %d\n"), activeProgram+1);
    printf_P(PSTR("Active scale: %d\n"), activeScale);
    #endif

    getPadParameters();
    //scale is contained within program, apply new parameters
    getScaleParameters();
}

///
/// \brief Initializes all pad data by reading values from database.
///
void Pads::getPadParameters()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out pad configuration\n"));
    #endif

    if (!splitEnabled)
    {
        //split off
        //apply global settings to pads
        #ifdef DEBUG
        printf_P(PSTR("All pad parameters are global - split is off.\n"));
        #endif

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            bitWrite(xSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            bitWrite(ySendEnabled, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            bitWrite(noteSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            bitWrite(aftertouchSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            bitWrite(pitchBendEnabledX, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            bitWrite(pitchBendEnabledY, i, database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram)));
            ccXPad[i]                       = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYPad[i]                       = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccXminPad[i]                    = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccXmaxPad[i]                    = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYminPad[i]                    = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYmaxPad[i]                    = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            padCurveX[i]                    = (curve_t)database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            padCurveY[i]                    = (curve_t)database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            midiChannel[i]                  = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
        }

        #ifdef DEBUG
        printf_P(PSTR("X send %s\n"), bitRead(xSendEnabled, 0) ? "enabled" : "disabled");
        printf_P(PSTR("Y send %s\n"), bitRead(ySendEnabled, 0) ? "enabled" : "disabled");
        printf_P(PSTR("Note send %s\n"), bitRead(noteSendEnabled, 0) ? "enabled" : "disabled");
        printf_P(PSTR("Aftertouch send %s\n"), bitRead(aftertouchSendEnabled, 0) ? "enabled" : "disabled");
        printf_P(PSTR("Pitch bend X send %s\n"), bitRead(pitchBendEnabledX, 0) ? "enabled" : "disabled");
        printf_P(PSTR("Pitch bend Y send %s\n"), bitRead(pitchBendEnabledY, 0) ? "enabled" : "disabled");
        printf_P(PSTR("CC X MIDI ID: %d\n"), ccXPad[0]);
        printf_P(PSTR("CC Y MIDI ID: %d\n"), ccYPad[0]);
        printf_P(PSTR("CC X lower limit: %d\n"), ccXminPad[0]);
        printf_P(PSTR("CC X upper limit: %d\n"), ccXmaxPad[0]);
        printf_P(PSTR("CC Y lower limit: %d\n"), ccYminPad[0]);
        printf_P(PSTR("CC Y upper limit: %d\n"), ccYmaxPad[0]);
        printf_P(PSTR("Pad curve for X: %d\n"), padCurveX[0]);
        printf_P(PSTR("Pad curve for Y: %d\n"), padCurveY[0]);
        printf_P(PSTR("MIDI channel: %d\n"), midiChannel[0]);
        #endif
    }
    else
    {
        //split on
        #ifdef DEBUG
        printf_P(PSTR("Pads have individual settings\n"));
        #endif

        //pads have individual settings
        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            bitWrite(xSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            bitWrite(ySendEnabled, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            bitWrite(noteSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            bitWrite(aftertouchSendEnabled, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            bitWrite(pitchBendEnabledX, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            bitWrite(pitchBendEnabledY, i, database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram)));
            ccXPad[i]                   = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_X_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            ccYPad[i]                   = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_Y_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            ccXminPad[i]                = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            ccXmaxPad[i]                = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            ccYminPad[i]                = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            ccYmaxPad[i]                = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            padCurveX[i]                = (curve_t)database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            padCurveY[i]                = (curve_t)database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));
            midiChannel[i]              = database.read(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram));

            #ifdef DEBUG
            printf_P(PSTR("----------------------\nPad %d\n----------------------\n"), i+1);
            printf_P(PSTR("X send enabled: %d\n"), bitRead(xSendEnabled, i));
            printf_P(PSTR("Y send enabled: %d\n"), bitRead(ySendEnabled, i));
            printf_P(PSTR("Note send enabled: %d\n"), bitRead(noteSendEnabled, i));
            printf_P(PSTR("Aftertouch send enabled: %d\n"), bitRead(aftertouchSendEnabled, i));
            printf_P(PSTR("CC X MIDI ID: %d\n"), ccXPad[i]);
            printf_P(PSTR("CC Y MIDI ID: %d\n"), ccYPad[i]);
            printf_P(PSTR("CC X lower limit: %d\n"), ccXminPad[i]);
            printf_P(PSTR("CC X upper limit: %d\n"), ccXmaxPad[i]);
            printf_P(PSTR("CC Y lower limit: %d\n"), ccYminPad[i]);
            printf_P(PSTR("CC Y upper limit: %d\n"), ccYmaxPad[i]);
            printf_P(PSTR("Pad curve for X: %d\n"), padCurveX[i]);
            printf_P(PSTR("Pad curve for Y: %d\n"), padCurveY[i]);
            printf_P(PSTR("MIDI channel: %d\n"), midiChannel[i]);
            #endif
        }
    }

    uint8_t lastTouchedPad = getLastTouchedPad();

    leds.setLEDstate(LED_ON_OFF_SPLIT, splitEnabled ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getMIDISendState(lastTouchedPad, functionOnOffAftertouch) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_NOTES, getMIDISendState(lastTouchedPad, functionOnOffNotes) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_X, getMIDISendState(lastTouchedPad, functionOnOffX) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_Y, getMIDISendState(lastTouchedPad, functionOnOffY) ? ledStateFull : ledStateOff);
}

///
/// \brief Initializes all scale data by reading values from database.
///
void Pads::getScaleParameters()
{
    activeScale = database.read(DB_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram);

    //clear all pad notes before assigning new ones
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;
    }

    if (isPredefinedScale(activeScale))
    {
        //predefined scale
        uint8_t octave = database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        note_t tonic = (note_t)database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        noteShiftLevel = database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);

        #ifdef DEBUG
        printf_P(PSTR("Octave: %d\n"), octave);
        printf_P(PSTR("Tonic: %d\n"), tonic);
        printf_P(PSTR("Shift: %d\n"), noteShiftLevel);
        #endif

        //apply default scale parameters
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

        //default notes in scale are now applied to pads
        //apply saved octave

        for (int i=0; i<NUMBER_OF_PADS; i++)
            padNote[i][0] += (MIDI_NOTES*octave);

        //now apply saved tonic
        //internal change, do not write anything to eeprom
        setTonic((note_t)tonic, true);

        if (noteShiftLevel)
        {
            //finally, apply note shift
            setScaleShiftLevel(noteShiftLevel, true);
        }
    }
    else
    {
        //user scales
        uint16_t noteID = (activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            for (int j=0; j<NOTES_PER_PAD; j++)
                padNote[i][j] = database.read(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i));
        }
    }
}

///
/// \brief Initializes all pad limits for all coordinates by reading values from database.
///
void Pads::getPadLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out limits for pads\n"));
    #endif

    getPressureLimits();
    getAftertouchLimits();
    getXLimits();
    getYLimits();
}

///
/// \brief Initializes all pad limits for pressure (Z coordinate) by reading values from database.
///
void Pads::getPressureLimits()
{
    uint8_t percentageIncrease = 0;

    switch(velocitySensitivity)
    {
        case velocity_soft:
        //default
        break;

        case velocity_medium:
        percentageIncrease = VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM;
        break;

        case velocity_hard:
        percentageIncrease = VELOCITY_PRESSURE_RATIO_UPPER_HARD;
        break;

        default:
        return;
    }

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        padPressureLimitUpper[i] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, i);

        if (percentageIncrease)
            padPressureLimitUpper[i] = padPressureLimitUpper[i] + (int32_t)((padPressureLimitUpper[i] * (int32_t)100) * (uint32_t)percentageIncrease) / 10000;

        #ifdef DEBUG
        printf_P(PSTR("Upper pressure limit for pad %d: %d\n"), i, padPressureLimitUpper[i]);
        #endif
    }
}

///
/// \brief Initializes all pad limits for aftertouch by scaling pressure limits.
///
void Pads::getAftertouchLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out aftertouch limits for pads."));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        uint16_t upperPressure_limit = padPressureLimitUpper[i];

        int32_t lowerLimit = upperPressure_limit + (int32_t)((upperPressure_limit * (int32_t)100) * (uint32_t)AFTERTOUCH_PRESSURE_RATIO_LOWER) / 10000;
        int32_t upperLimit = lowerLimit + (int32_t)((lowerLimit * (int32_t)100) * (uint32_t)AFTERTOUCH_PRESSURE_RATIO_UPPER) / 10000;

        padAftertouchLimitLower[i] = lowerLimit;
        padAftertouchLimitUpper[i] = upperLimit;

        #ifdef DEBUG
        printf_P(PSTR("Lower aftertouch limit for pad %d: %d\n"), i, padAftertouchLimitLower[i]);
        printf_P(PSTR("Upper aftertouch limit for pad %d: %d\n"), i, padAftertouchLimitUpper[i]);
        #endif
    }
}

///
/// \brief Initializes all pad limits for X coordinate by reading values from database.
///
void Pads::getXLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out X limits for pads\n"));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        padXLimitLower[i] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, i);
        padXLimitUpper[i] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, i);

        #ifdef DEBUG
        printf_P(PSTR("Lower X limit for pad %d: %d\n"), i, padXLimitLower[i]);
        printf_P(PSTR("Upper X limit for pad %d: %d\n"), i, padXLimitUpper[i]);
        #endif
    }

    #ifdef DEBUG
    printf_P(PSTR("\n"));
    #endif
}

///
/// \brief Initializes all pad limits for Y coordinate by reading values from database.
///
void Pads::getYLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out Y limits for pads\n"));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        padYLimitLower[i] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, i);
        padYLimitUpper[i] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, i);

        #ifdef DEBUG
        printf_P(PSTR("Lower Y limit for pad %d: %d\n"), i, padYLimitLower[i]);
        printf_P(PSTR("Upper Y limit for pad %d: %d\n"), i, padYLimitUpper[i]);
        #endif
    }

    #ifdef DEBUG
    printf_P(PSTR("\n"));
    #endif
}

///
/// \brief Checks how many notes there are in requested predefined scale.
/// @param [in] scale   Scale which is being checked.
/// \returns Number of notes in requested scale. If user scale is requested, -1 is returned.
///
int8_t Pads::getPredefinedScaleNotes(int8_t scale)
{
    assert(SCALE_CHECK(scale));

    //safety check
    if (isUserScale(scale))
        return -1;

    return scale_notes_sizes[scale];
}

///
/// \brief Returns specific note from specified scale.
/// @param [in] scale   Scale which is being checked.
/// @param [in] index   Note index which is being checked.
/// \returns Note at specified index from requested scale (enumerated type). See note_t enumeration.
///
note_t Pads::getScaleNote(int8_t scale, int8_t index)
{
    assert(SCALE_CHECK(scale));

    //safety checks

    if (isUserScale(scale))
        return MIDI_NOTES;

    if (index >= MIDI_NOTES)
        return MIDI_NOTES;

    if (index < 0)
        return MIDI_NOTES;

    if ((uint8_t)index >= scale_notes_sizes[scale])
        return MIDI_NOTES;

    return scale_notes[scale][index];
}

///
/// \brief Calculates scaled pressure value (0-127) from raw reading (0-1023) on specified pad.
/// @param [in] pad             Pad which is being checked.
/// @param [in] pressure        Raw pressure reading (0-1023).
/// @param [in] type            Pressure type (velocity or aftertouch). Enumerated type. See pressureType_t enumeration.
/// \returns Scaled pressure value.
///
uint8_t Pads::getScaledPressure(int8_t pad, uint16_t pressure, pressureType_t type)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case pressureAftertouch:
        return curves.map(constrain(pressure, padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad]), padAftertouchLimitLower[pad], padAftertouchLimitUpper[pad], 0, 127);
        break;

        case pressureVelocity:
        return curves.map(constrain(pressure, 0, padPressureLimitUpper[pad]), 0, padPressureLimitUpper[pad], 0, 127);
        break;
    }

    return 0;
}

///
/// \brief Calculates scaled X or Y value from raw reading (0-1023) on specified pad.
/// @param [in] pad         Pad which is being checked.
/// @param [in] xyValue     Raw X or Y value (0-1023).
/// @param [in] type        Coordinate which is being checked (X or Y). Enumerated type. See padCoordinate_t enumeration.
/// @param [in] midiScale   If set to true, requested value will be scaled to MIDI range (0-127), otherwise, value will be scaled to raw range (0-1023).
/// Scaling to raw range is useful in certain scenarios since lowest and largest X or Y values are never 0 and 1023 but some values in between.
///
uint16_t Pads::getScaledXY(int8_t pad, uint16_t xyValue, padCoordinate_t type, bool midiScale)
{
    assert(PAD_CHECK(pad));

    switch (type)
    {
        case coordinateX:
        return curves.map(constrain(xyValue, padXLimitLower[pad], padXLimitUpper[pad]), padXLimitLower[pad], padXLimitUpper[pad], 0, midiScale ? 127 : 1023);

        case coordinateY:
        return curves.map(constrain(xyValue, padYLimitLower[pad], padYLimitUpper[pad]), padYLimitLower[pad], padYLimitUpper[pad], 0, midiScale ? 127 : 1023);

        default:
        return 0;
    }
}

///
/// \brief Checks for currently active pitch bend type.
/// \returns Active pitch bend type (enumerated type). See pitchBend_t enumeration.
///
pitchBendType_t Pads::getPitchBendType()
{
    return pitchBendType;
}

///
/// \brief Checks if pitch bend is enabled on requested pad.
/// @param [in]     Pad which is being checked.
/// \returns True if pitch bend is enabled on requested pad, false otherwise.
///
bool Pads::getPitchBendState(int8_t pad, padCoordinate_t coordinate)
{
    assert(PAD_CHECK(pad));

    switch(coordinate)
    {
        case coordinateX:
        return bitRead(pitchBendEnabledX, pad);

        case coordinateY:
        return bitRead(pitchBendEnabledY, pad);

        default:
        return false;
    }
}

/// @}