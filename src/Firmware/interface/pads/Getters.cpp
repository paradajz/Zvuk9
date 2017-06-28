#include "Pads.h"
#include "../leds/LEDs.h"
#include "../../database/Database.h"

///
/// \brief Checks last pad which has been pressed.
/// \returns Last pressed pad.
///
uint8_t Pads::getLastTouchedPad()
{
    return padPressHistory_buffer[padPressHistory_counter];
}

///
/// \brief Checks for currently active program.
/// \returns Currently active program.
///
uint8_t Pads::getActiveProgram()
{
    return activeProgram;
}

///
/// \brief Checks for currently active scale.
/// \returns Currently active scale (integer), range 0-PREDEFINED_SCALES+NUMBER_OF_USER_SCALES.
///
uint8_t Pads::getActiveScale()
{
    return activeScale;
}

///
/// \brief Checks if pad editing is in progress.
/// \returns True if pad edit mode is enabled, false otherwise.
///
bool Pads::getEditModeState()
{
    return editModeActivated;
}

///
/// \brief Finds currently active tonic.
/// In user scales, tonic is first found note on first pad. If no notes are found, invalid value is returned (MIDI_NOTES).
/// In predefined scales, tonic is written in EEPROM since it's configurable.
/// \returns Currently active tonic (enumerated type). See note_t enumeration.
///
note_t Pads::getActiveTonic()
{
    if (isUserScale(activeScale))
    {
        //tonic is first found note on first pad on user scales
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[0][i] != BLANK_NOTE)
                return getTonicFromNote(padNote[0][i]+noteShiftAmount[i]);
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
/// \returns Currently active octave.
///
uint8_t Pads::getActiveOctave()
{
    return activeOctave;
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
bool Pads::getMIDISendState(int8_t pad, onOff_t type)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case onOff_aftertouch:
        return bitRead(aftertouchSendEnabled, pad);

        case onOff_notes:
        return bitRead(noteSendEnabled, pad);

        case onOff_x:
        return bitRead(xSendEnabled, pad);

        case onOff_y:
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
/// \returns Note from requested index on requested pad.
///
uint8_t Pads::getPadNote(int8_t pad, int8_t noteIndex)
{
    assert(PAD_CHECK(pad));
    assert(NOTE_ASSIGN_CHECK(noteIndex));

    return padNote[pad][noteIndex];
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
/// \returns Calculated octave.
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
/// \brief Checks for assigned CC number on requested coordinate and pad.
/// @param [in] pad     Pad on which CC number is checked
/// @param [in] type    Coordinate from which CC number is requested (X or Y) (enumerated type). See padCoordinate_t enumeration.
///
uint8_t Pads::getCC(int8_t pad, padCoordinate_t type)
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

uint8_t Pads::getCClimit(int8_t pad, padCoordinate_t type, ccLimitType_t limitType)
{
    assert(PAD_CHECK(pad));

    switch(type)
    {
        case coordinateX:
        if (limitType == ccLimitTypeMax)
        return ccXmaxPad[pad];
        else if (limitType == ccLimitTypeMin)
        return ccXminPad[pad];
        else
        return 0;

        case coordinateY:
        if (limitType == ccLimitTypeMax)
        return ccYmaxPad[pad];
        else if (limitType == ccLimitTypeMin)
        return ccYminPad[pad];
        else
        return 0;

        default:
        return 0;
    }
}

void Pads::getConfiguration()
{
    //globals
    aftertouchType = (aftertouchType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_AFTERTOUCH_TYPE_ID);
    pressureSensitivity = (pressureSensitivity_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsPressure, PRESSURE_SETTING_SENSITIVITY_ID);
    pressureCurve = (curve_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsPressure, PRESSURE_SETTING_CURVE_ID);

    //read pad configuration from EEPROM
    getProgramParameters();
    getPadLimits();
}

void Pads::getProgramParameters()
{
    #ifdef DEBUG
    printf_P(PSTR("----------------------\nPrinting out program settings\n----------------------\n"));
    #endif

    activeProgram = database.read(DB_BLOCK_PROGRAM, programLastActiveProgramSection, 0);
    activeScale = database.read(DB_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram);

    #ifdef DEBUG
    printf_P(PSTR("Active program: %d\n"), activeProgram+1);
    printf_P(PSTR("Active scale: %d\n"), activeScale);
    #endif

    getPadParameters();
    //scale is contained within program, apply new parameters
    getScaleParameters();
}

void Pads::getPadParameters()
{
    splitEnabled = database.read(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));

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
        printf_P(PSTR("X send enabled: %d\n"), bitRead(xSendEnabled, 0));
        printf_P(PSTR("Y send enabled: %d\n"), bitRead(ySendEnabled, 0));
        printf_P(PSTR("Note send enabled: %d\n"), bitRead(noteSendEnabled, 0));
        printf_P(PSTR("Aftertouch send enabled: %d\n"), bitRead(aftertouchSendEnabled, 0));
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
    leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getMIDISendState(lastTouchedPad, onOff_aftertouch) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_NOTES, getMIDISendState(lastTouchedPad, onOff_notes) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_X, getMIDISendState(lastTouchedPad, onOff_x) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_Y, getMIDISendState(lastTouchedPad, onOff_y) ? ledStateFull : ledStateOff);
}

void Pads::getScaleParameters()
{
    //#ifdef DEBUG
    //printf_P("Printing out scale settings\n");
    //printf_P("Scale: %d\n", activeScale);
    //#endif

    //clear all pad notes before assigning new ones
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;
    }

    generateScale((scale_t)activeScale);

    //reset this variable first
    activeOctave = DEFAULT_OCTAVE;

    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[0][i] != BLANK_NOTE)
        {
            activeOctave = getOctaveFromNote(padNote[0][i]);
            #ifdef DEBUG
            printf_P(PSTR("Active octave: %d\n"), activeOctave);
            #endif
            break;
        }
    }
}

void Pads::generateScale(scale_t scale)
{
    if (isPredefinedScale(scale))
    {
        //predefined scale
        uint8_t notesPerScale = getNotesPerScale(scale);
        uint8_t octave = database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        note_t tonic = (note_t)database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        noteShiftLevel = database.read(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);

        #ifdef DEBUG
        printf_P(PSTR("Octave: %d\n"), octave);
        printf_P(PSTR("Tonic: %d\n"), tonic);
        printf_P(PSTR("Shift: %d\n"), noteShiftLevel);
        #endif

        uint8_t noteCounter = 0;

        for (int i=0; i<notesPerScale; i++)
        {
            padNote[i][0] = getScaleNote(scale, i);
            noteCounter++;
        }

        if (notesPerScale < NUMBER_OF_PADS)
        {
            noteCounter = 0;

            for (int i=notesPerScale; i<NUMBER_OF_PADS; i++)
            {
                padNote[i][0] = getScaleNote(scale, noteCounter);
                //these notes are actually in another octave
                padNote[i][0] += MIDI_NOTES;
                noteCounter++;
            }
        }

        //default notes in scale are now applied to pads
        //apply saved octave

        for (int i=0; i<NUMBER_OF_PADS; i++)
            padNote[i][0] += (MIDI_NOTES*octave);

        //now apply saved tonic
        //internal change, do not write anything to eeprom
        setActiveTonic((note_t)tonic, true);

        //finally, apply note shift
        if (noteShiftLevel < 0)
        {
            uint8_t temp = abs(noteShiftLevel);
            for (int i=0; i<temp; i++)
                shiftNote(false, true);
        }
        else
        {
            for (int i=0; i<noteShiftLevel; i++)
                shiftNote(true, true);
        }
    }
    else
    {
        //user scales
        //#ifdef DEBUG
        //printf_P("User scale %d\n", scale-PREDEFINED_SCALES);
        //#endif

        uint16_t noteID = (scale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            for (int j=0; j<NOTES_PER_PAD; j++)
                padNote[i][j] = database.read(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i));
        }
    }

    //#ifdef DEBUG
    //printf_P("Printing out notes for pads\n");
    //for (int i=0; i<MAX_PADS; i++)
    //{
        //printf_P("Pad %d: \n", i+1);
//
        //for (int j=0; j<NOTES_PER_PAD; j++)
            //printf_P("%d\n", padNote[i][j]);
    //}
    //#endif
}

void Pads::getPadLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out limits for pads\n"));
    #endif
    setPressureSensitivity(pressureSensitivity);
    getXLimits();
    getYLimits();
}

void Pads::getPressureLimits()
{
    uint8_t percentageIncrease = 0;

    switch(pressureSensitivity)
    {
        case pressure_soft:
        //default
        break;

        case pressure_medium:
        percentageIncrease = VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM;
        break;

        case pressure_hard:
        percentageIncrease = VELOCITY_PRESSURE_RATIO_UPPER_HARD;
        break;

        default:
        return;
    }

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<PRESSURE_CALIBRATION_ZONES; j++)
        {
            padPressureLimitUpper[i][j] = database.read(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection0+j, i);

            if (percentageIncrease)
                padPressureLimitUpper[i][j] = padPressureLimitUpper[i][j] + (int32_t)((padPressureLimitUpper[i][j] * (int32_t)100) * (uint32_t)percentageIncrease) / 10000;

            #ifdef DEBUG
            printf_P(PSTR("Upper pressure limit for pad %d, pressure zone %d: %d\n"), i, j, padPressureLimitUpper[i][j]);
            #endif
        }
    }
}

void Pads::getAftertouchLimits()
{
    #ifdef DEBUG
    printf_P(PSTR("Printing out aftertouch limits for pads on "));
    #endif

    uint8_t percentageIncrease = 0;

    switch(pressureSensitivity)
    {
        case pressure_soft:
        #ifdef DEBUG
        printf_P(PSTR("soft sensitivity setting.\n"));
        #endif
        percentageIncrease = AFTERTOUCH_PRESSURE_RATIO_UPPER_LOW;
        break;

        case pressure_medium:
        #ifdef DEBUG
        printf_P(PSTR("medium sensitivity setting.\n"));
        #endif
        percentageIncrease = AFTERTOUCH_PRESSURE_RATIO_UPPER_MEDIUM;
        break;

        case pressure_hard:
        #ifdef DEBUG
        printf_P(PSTR("hard sensitivity setting.\n"));
        #endif
        percentageIncrease = AFTERTOUCH_PRESSURE_RATIO_UPPER_HARD;
        break;

        default:
        return;
    }

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<PRESSURE_CALIBRATION_ZONES; j++)
        {
            int32_t lowerLimit = padPressureLimitUpper[i][j] + (int32_t)((padPressureLimitUpper[i][j] * (int32_t)100) * (uint32_t)AFTERTOUCH_PRESSURE_RATIO_LOWER) / 10000;
            int32_t upperLimit = lowerLimit + (int32_t)((lowerLimit * (int32_t)100) * (uint32_t)percentageIncrease) / 10000;

            padAftertouchLimitLower[i][j] = lowerLimit;
            padAftertouchLimitUpper[i][j] = upperLimit;

            #ifdef DEBUG
            printf_P(PSTR("Lower aftertouch limit for pad %d, pressure zone %d: %d\n"), i, j, padAftertouchLimitLower[i][j]);
            printf_P(PSTR("Upper aftertouch limit for pad %d, pressure zone %d: %d\n"), i, j, padAftertouchLimitUpper[i][j]);
            #endif
        }
    }
}

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

uint16_t Pads::getCoordinateLimit(uint8_t pad, padCoordinate_t coordinate, calibrationDirection direction)
{
    switch(coordinate)
    {
        case coordinateX:
        return (direction == lower) ? padXLimitLower[pad] : padXLimitUpper[pad];
        break;

        case coordinateY:
        return (direction == lower) ? padYLimitLower[pad] : padYLimitUpper[pad];
        break;

        default:
        return 0;
    }
}

curve_t Pads::getCCcurve(int8_t pad, padCoordinate_t coordinate)
{
    switch(coordinate)
    {
        case coordinateX:
        return (curve_t)padCurveX[pad];

        case coordinateY:
        return (curve_t)padCurveY[pad];

        default:
        return NUMBER_OF_CURVES;
    }
}

uint8_t Pads::getMIDIchannel(uint8_t pad)
{
    return midiChannel[pad];
}

scaleType_t Pads::getScaleType(int8_t scale)
{
    switch(scale)
    {
        case scaleNaturalMinor:
        case scaleMelodicMinor:
        case scaleHarmonicMinor:
        case scaleMajor:
        case scaleHarmonicMajor:
        return sevenNoteScale;

        case scaleMinorPentatonic:
        case scaleMajorPentatonic:
        return fiveNoteScale;

        default:
        return userScale;
    }
}

bool Pads::isUserScale(uint8_t scale)
{
    return (scale >= PREDEFINED_SCALES);
}

bool Pads::isPredefinedScale(uint8_t scale)
{
    return (scale < PREDEFINED_SCALES);
}

bool Pads::isNoteAssigned(note_t note)
{
    //return true if received note is among active notes on some pad

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

bool Pads::isPadPressed(uint8_t padNumber)
{
    return bitRead(padPressed, padNumber);
}

bool Pads::allPadsReleased()
{
    //return true if all pads are released
    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (isPadPressed(i)) return false;

    return true;
}

pressureSensitivity_t Pads::getPressureSensitivity()
{
    return pressureSensitivity;
}

curve_t Pads::getPressureCurve()
{
    return pressureCurve;
}

padCalibrationSection Pads::getPressureZone(uint8_t pad)
{
    //find out pressure zone
    uint8_t row = PRESSURE_CALIBRATION_Y_ZONES - 1 - (scaleXY(pad, board.getPadY(pad), coordinateY, false) / PRESSURE_CALIBRATION_MAX_Y_ZONE_VALUE);
    //invert
    uint8_t column = scaleXY(pad, board.getPadX(pad), coordinateX, false) / PRESSURE_CALIBRATION_MAX_X_ZONE_VALUE;

    return (padCalibrationSection)(column + row*PRESSURE_CALIBRATION_X_ZONES);
}

bool Pads::isCalibrationEnabled()
{
    return calibrationEnabled;
}

padCoordinate_t Pads::getCalibrationMode()
{
    return activeCalibration;
}