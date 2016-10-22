#include "Pads.h"
#include "../leds/LEDs.h"
#include "../../eeprom/Configuration.h"

//read from eeprom

void Pads::getConfiguration()   {

    //globals
    aftertouchType = configuration.readParameter(CONF_BLOCK_MIDI, 0, MIDI_SETTING_AFTERTOUCH_TYPE_ID);
    pressureSensitivity = (pressureSensitivity_t)configuration.readParameter(CONF_BLOCK_PRESSURE_SETTINGS, pressureSensitivitySection, 0);

    //read pad configuration from EEPROM
    getProgramParameters();
    getPadLimits();

}

void Pads::getProgramParameters()   {

    #ifdef DEBUG
        printf("Printing out program settings\n");
    #endif

    activeProgram = configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection, 0);
    activeScale = configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram);

    #ifdef DEBUG
        printf("Active program: %d\n", activeProgram+1);
        printf("Active scale: %d\n", activeScale);
    #endif

    getPadParameters();
    //scale is contained within program, apply new parameters
    getScaleParameters();

}

void Pads::getPadParameters()   {

    splitEnabled = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));

    #ifdef DEBUG
        printf("Printing out pad configuration\n");
    #endif

    if (!splitEnabled)  {   //split off

        //apply global settings to pads
        #ifdef DEBUG
            printf("All pad parameters are global - split is off\n");
        #endif

        for (int i=0; i<MAX_PADS; i++)    {

            xSendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ySendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            noteSendEnabled[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            aftertouchSendEnabled[i]        = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccXPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccXminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccXmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            ccYmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            padCurveX[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            padCurveY[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));
            midiChannel[i]                  = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram));

        }

        //#ifdef DEBUG
            //printf("X send enabled: %d\n", xSendEnabled[0]);
            //printf("Y send enabled: %d\n", ySendEnabled[0]);
            //printf("Note send enabled: %d\n", noteSendEnabled[0]);
            //printf("Aftertouch send enabled: %d\n", aftertouchSendEnabled[0]);
            //printf("CC X MIDI ID: %d\n", ccXPad[0]);
            //printf("CC Y MIDI ID: %d\n", ccYPad[0]);
            //printf("CC X lower limit: %d\n", ccXminPad[0]);
            //printf("CC X upper limit: %d\n", ccXmaxPad[0]);
            //printf("CC Y lower limit: %d\n", ccYminPad[0]);
            //printf("CC Y upper limit: %d\n", ccYmaxPad[0]);
            //printf("Pad curve for X: %d\n", padCurveX[0]);
            //printf("Pad curve for Y: %d\n", padCurveY[0]);
            //printf("MIDI channel: %d\n", midiChannel[0]);
        //#endif

    }   else {  //split on

        #ifdef DEBUG
            printf("Pads have individual settings\n");
        #endif

        //pads have individual settings
        for (int i=0; i<MAX_PADS; i++)    {

                xSendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ySendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                noteSendEnabled[i]          = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                aftertouchSendEnabled[i]    = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccXPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_X_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccYPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_Y_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccXminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccXmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccYminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                ccYmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                padCurveX[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                padCurveY[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));
                midiChannel[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram));

                //#ifdef DEBUG
                    //printf("Pad %d", i+1);
//
                    //printf("X send enabled: %d\n", xSendEnabled[i]);
                    //printf("Y send enabled: %d\n", ySendEnabled[i]);
                    //printf("Note send enabled: %d\n", noteSendEnabled[i]);
                    //printf("Aftertouch send enabled: %d\n", aftertouchSendEnabled[i]);
                    //printf("CC X MIDI ID: %d\n", ccXPad[i]);
                    //printf("CC Y MIDI ID: %d\n", ccYPad[i]);
                    //printf("CC X lower limit: %d\n", ccXminPad[i]);
                    //printf("CC X upper limit: %d\n", ccXmaxPad[i]);
                    //printf("CC Y lower limit: %d\n", ccYminPad[i]);
                    //printf("CC Y upper limit: %d\n", ccYmaxPad[i]);
                    //printf("Pad curve for X: %d\n", padCurveX[i]);
                    //printf("Pad curve for Y: %d\n", padCurveY[i]);
                    //printf("MIDI channel: %d\n", midiChannel[i]);
                //#endif

        }

    }

    uint8_t lastTouchedPad = getLastTouchedPad();

    leds.setLEDstate(LED_ON_OFF_SPLIT, splitEnabled ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getMIDISendState(onOff_aftertouch, lastTouchedPad) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_NOTES, getMIDISendState(onOff_notes, lastTouchedPad) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_X, getMIDISendState(onOff_x, lastTouchedPad) ? ledStateFull : ledStateOff);
    leds.setLEDstate(LED_ON_OFF_Y, getMIDISendState(onOff_y, lastTouchedPad) ? ledStateFull : ledStateOff);

}

void Pads::getScaleParameters()    {

    //#ifdef DEBUG
        //printf("Printing out scale settings\n");
        //printf("Scale: %d\n", activeScale);
    //#endif

    //clear all pad notes before assigning new ones
    for (int i=0; i<MAX_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    generateScale((scale_t)activeScale);

    //reset this variable first
    activeOctave = DEFAULT_OCTAVE;

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[0][i] != BLANK_NOTE)    {

            activeOctave = getOctaveFromNote(padNote[0][i]);
            #ifdef DEBUG
                printf("Active octave: %d\n", activeOctave);
            #endif
            break;

        }

    }

}

void Pads::generateScale(scale_t scale)    {

    if (isPredefinedScale(scale)) {

        //predefined scale
        uint8_t notesPerScale = getNotesPerScale(scale);
        uint8_t octave = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        note_t tonic = (note_t)configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);
        noteShiftLevel = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale);

        //#ifdef DEBUG
            //printf("Octave: %d\n", octave);
            //printf("Tonic: %d\n", tonic);
            //printf("Shift: %d\n", noteShiftLevel);
        //#endif

        uint8_t noteCounter = 0;

        for (int i=0; i<notesPerScale; i++) {

            padNote[i][0] = getScaleNote(scale, i);
            noteCounter++;

        }

        if (notesPerScale < MAX_PADS) {

            noteCounter = 0;

            for (int i=notesPerScale; i<MAX_PADS; i++)  {

                padNote[i][0] = getScaleNote(scale, noteCounter);
                //these notes are actually in another octave
                padNote[i][0] += MIDI_NOTES;
                noteCounter++;

            }

        }

        //default notes in scale are now applied to pads
        //apply saved octave

        for (int i=0; i<MAX_PADS; i++)    {

            padNote[i][0] += (MIDI_NOTES*octave);

        }

        //now apply saved tonic
        setTonic((note_t)tonic, true); //internal change, do not write anything to eeprom

        //finally, apply note shift
        if (noteShiftLevel < 0)  {

            noteShiftLevel *= -1;
            for (int i=0; i<noteShiftLevel; i++)
                shiftNote(false, true);

        }   else {

            for (int i=0; i<noteShiftLevel; i++)
                shiftNote(true, true);

        }

    }   else {  //user scales

        //#ifdef DEBUG
            //printf("User scale %d\n", scale-PREDEFINED_SCALES);
        //#endif

        uint16_t noteID = (scale - PREDEFINED_SCALES)*(MAX_PADS*NOTES_PER_PAD);

        for (int i=0; i<MAX_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                padNote[i][j] = configuration.readParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i));

            }

        }

    }

    //#ifdef DEBUG
        //printf("Printing out notes for pads\n");
        //for (int i=0; i<MAX_PADS; i++)    {
//
            //printf("Pad %d: \n", i+1);
//
            //for (int j=0; j<NOTES_PER_PAD; j++)
                //printf("%d\n", padNote[i][j]);
//
        //}
    //#endif

}

void Pads::getPadLimits()   {

    #ifdef DEBUG
    printf("Printing out limits for pads\n");
    #endif

    setPressureSensitivity(pressureSensitivity);
    //getPressureLimits(pressureSensitivity);
    getXLimits();
    getYLimits();
    getAftertouchLimits();

}

void Pads::getPressureLimits()  {

    uint8_t percentageIncrease = 0;

    switch(pressureSensitivity)
    {
        case pressure_soft:
        //default
        break;

        case pressure_medium:
        percentageIncrease = MEDIUM_PRESSURE_INCREASE_PERCENT;
        break;

        case pressure_hard:
        percentageIncrease = HARD_PRESSURE_INCREASE_PERCENT;
        break;

        default:
        return;
    }

    for (int i=0; i<MAX_PADS; i++) {

        padPressureLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, i);
        if (percentageIncrease)
        {
            padPressureLimitUpper[i] = padPressureLimitUpper[i] + (int32_t)((padPressureLimitUpper[i] * (int32_t)100) * (uint32_t)percentageIncrease) / 10000;
        }

        #ifdef DEBUG
        printf("Upper pressure limit for pad %d: %d\n", i, padPressureLimitUpper[i]);
        #endif

    }

}

void Pads::getXLimits()  {

    #ifdef DEBUG
        printf("Printing out X limits for pads\n");
    #endif

    for (int i=0; i<MAX_PADS; i++) {

        padXLimitLower[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, i);
        padXLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, i);

        #ifdef DEBUG
        printf("Lower X limit for pad %d: %d\n", i, padXLimitLower[i]);
        printf("Upper X limit for pad %d: %d\n", i, padXLimitUpper[i]);
        #endif

    }

}

void Pads::getYLimits()  {

    #ifdef DEBUG
    printf("Printing out Y limits for pads\n");
    #endif

    for (int i=0; i<MAX_PADS; i++) {

        padYLimitLower[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, i);
        padYLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, i);

        #ifdef DEBUG
        printf("Lower Y limit for pad %d: %d\n", i, padYLimitLower[i]);
        printf("Upper Y limit for pad %d: %d\n", i, padYLimitUpper[i]);
        #endif

    }

}

void Pads::getAftertouchLimits()    {

    #ifdef DEBUG
    printf("Printing out aftertouch limits for pads\n");
    #endif

    for (int i=0; i<MAX_PADS; i++)    {

        int32_t lowerLimit = padPressureLimitUpper[i] + (int32_t)((padPressureLimitUpper[i] * (int32_t)100) * (uint32_t)AFTERTOUCH_PRESSURE_RATIO_LOWER) / 10000;
        int32_t upperLimit = padPressureLimitUpper[i] + (int32_t)((padPressureLimitUpper[i] * (int32_t)100) * (uint32_t)AFTERTOUCH_PRESSURE_RATIO_UPPER) / 10000;
        padAftertouchLimitLower[i] = lowerLimit;
        padAftertouchLimitUpper[i] = upperLimit;

        #ifdef DEBUG
        printf("Lower aftertouch limit for pad %d: %d\n", i, padAftertouchLimitLower[i]);
        printf("Upper aftertouch limit for pad %d: %d\n", i, padAftertouchLimitUpper[i]);
        #endif

    }

}

bool Pads::getMIDISendState(onOff_t type, uint8_t padNumber)    {

    switch(type)    {

        case onOff_aftertouch:
        return aftertouchSendEnabled[padNumber];

        case onOff_notes:
        return noteSendEnabled[padNumber];

        case onOff_x:
        return xSendEnabled[padNumber];

        case onOff_y:
        return ySendEnabled[padNumber];

        default:
        return false;

    }

}

aftertouchType_t Pads::getAftertouchType()  {

    return (aftertouchType_t)aftertouchType;

}

bool Pads::getSplitState()    {

    return splitEnabled;

}

uint8_t Pads::getActiveProgram()    {

    return activeProgram;

}

uint8_t Pads::getActiveScale()    {

    return activeScale;

}

uint8_t Pads::getCCcurve(coordinateType_t coordinate, uint8_t padNumber)    {

    switch(coordinate)   {

        case coordinateX:
        return padCurveX[padNumber];

        case coordinateY:
        return padCurveY[padNumber];

        default:
        return 0;

    }

}

uint8_t Pads::getCClimitValue(coordinateType_t type, ccLimitType_t limitType, uint8_t padNumber)   {

    switch(type)  {

        case coordinateX:
        if (limitType == ccLimitTypeMax) return ccXmaxPad[padNumber];
        else return ccXminPad[padNumber];

        case coordinateY:
        if (limitType == ccLimitTypeMax) return ccYmaxPad[padNumber];
        else return ccYminPad[padNumber];

        default:
        return 0;

    }

}

uint8_t Pads::getCCvalue(coordinateType_t type, uint8_t padNumber)  {

    switch(type)  {

        case coordinateX:
        return ccXPad[padNumber];

        case coordinateY:
        return ccYPad[padNumber];

        default:
        return 0;

    }

}

uint8_t Pads::getMIDIchannel(uint8_t pad) {

    return midiChannel[pad];

}

uint8_t Pads::getActiveOctave()    {

    return activeOctave;

}

note_t Pads::getActiveTonic()    {

    if (isUserScale(activeScale)) {

        //currentScaleTonic is first found note on first pad on user scales

        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[0][i] != BLANK_NOTE)
                return getTonicFromNote(padNote[0][i]+noteShiftAmount[i]);

    }   else  {

        //predefined scale tonic is written in eeprom
        uint16_t tonicIndex = PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale;
        return (note_t)configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, tonicIndex);

    }   return MIDI_NOTES;

}

scaleType_t Pads::getScaleType(int8_t scale) {

    switch(scale)   {

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

bool Pads::isUserScale(uint8_t scale)   {

    return (scale >= PREDEFINED_SCALES);

}

bool Pads::isPredefinedScale(uint8_t scale) {

    return (scale < PREDEFINED_SCALES);

}

uint8_t Pads::getPadNote(uint8_t pad, uint8_t note) {

    if (pad >= MAX_PADS) return 0;
    if (note >= NOTES_PER_PAD) return 0;

    return padNote[pad][note];

}

bool Pads::noteActive(note_t note) {

    //return true if received note is among active notes on some pad

    for (int i=0; i<MAX_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            if (padNote[i][j] != BLANK_NOTE)
                if (getTonicFromNote(padNote[i][j]) == note)
                    return true;

    return false;

}

uint8_t Pads::getLastTouchedPad()   {

    return padPressHistory_buffer[padPressHistory_counter];

}

note_t Pads::getTonicFromNote(uint8_t note)    {

    if (note == BLANK_NOTE) return MIDI_NOTES;
    return (note_t)(note % MIDI_NOTES);

}

uint8_t Pads::getOctaveFromNote(uint8_t note)  {

    if (note == BLANK_NOTE) return MIDI_OCTAVE_RANGE;
    return note / MIDI_NOTES;

}

bool Pads::isPadPressed(uint8_t padNumber) {

    return bitRead(padPressed, padNumber);

}

bool Pads::allPadsReleased()   {

    //return true if all pads are released
    for (int i=0; i<MAX_PADS; i++)
        if (isPadPressed(i)) return false;

    return true;

}

uint8_t Pads::padsPressed() {

    //returns number of pressed pads

    uint8_t numberOfPressedPads = 0;
    for (int i=0; i<MAX_PADS; i++)
        if (isPadPressed(i))
            numberOfPressedPads++;

    return numberOfPressedPads;

}

pressureSensitivity_t Pads::getPressureSensitivity()   {

    return pressureSensitivity;

}