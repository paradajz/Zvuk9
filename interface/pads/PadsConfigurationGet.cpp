#include "Pads.h"

//read from eeprom

void Pads::getPadConfig()   {

    //read pad configuration from EEPROM
    getProgramParameters();
    getPadLimits();

}

void Pads::getProgramParameters()   {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out program settings"));
        Serial.println(F("----------------------------------------"));
    #endif

    activeProgram = configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection, 0);
    activePreset = configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, activeProgram);
    midiChannel = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));

    #if MODE_SERIAL
        Serial.print(F("Active program: "));
        Serial.println(activeProgram+1);
        Serial.print(F("Active preset: "));
        Serial.println(activePreset);
        Serial.print(F("MIDI channel: "));
        Serial.println(midiChannel);
    #endif

    getPadParameters();

    //preset is contained within program, apply new parameters
    getPresetParameters();

}

void Pads::getPadParameters()   {

    splitCounter = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out pad configuration"));
        Serial.println(F("----------------------------------------"));
    #endif

    if (splitCounter == 0)  {   //split off

        //apply global settings to pads
        #if MODE_SERIAL > 0
            Serial.println("All pad parameters are global - split is off");
        #endif

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            xSendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ySendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            noteSendEnabled[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            aftertouchSendEnabled[i]        = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccXPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccYPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccXminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccXmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccYminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            ccYmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            padCurveX[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
            padCurveY[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));

        }

        #if MODE_SERIAL
            Serial.print(F("X send enabled: "));            Serial.println(xSendEnabled[0]);
            Serial.print(F("Y send enabled: "));            Serial.println(ySendEnabled[0]);
            Serial.print(F("Note send enabled: "));         Serial.println(noteSendEnabled[0]);
            Serial.print(F("Aftertouch send enabled: "));   Serial.println(aftertouchSendEnabled[0]);
            Serial.print(F("CC X MIDI ID: "));              Serial.println(ccXPad[0]);
            Serial.print(F("CC Y MIDI ID: "));              Serial.println(ccYPad[0]);
            Serial.print(F("CC X lower limit: "));          Serial.println(ccXminPad[0]);
            Serial.print(F("CC X upper limit: "));          Serial.println(ccXmaxPad[0]);
            Serial.print(F("CC Y lower limit: "));          Serial.println(ccYminPad[0]);
            Serial.print(F("CC Y upper limit: "));          Serial.println(ccYmaxPad[0]);
            Serial.print(F("Pad curve for X: "));           Serial.println(padCurveX[0]);
            Serial.print(F("Pad curve for Y: "));           Serial.println(padCurveY[0]);
        #endif

    }   else if (splitCounter == 1) {   //split x/y

            #if MODE_SERIAL > 0
                Serial.println("Pad functions are global, rest is local - split x/y is on");
            #endif

            for (int i=0; i<NUMBER_OF_PADS; i++)    {

                //functions are still global
                xSendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
                ySendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
                noteSendEnabled[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));
                aftertouchSendEnabled[i]        = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram));

                //pads have individual settings
                ccXPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_X_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_Y_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccXminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccXmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                padCurveX[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                padCurveY[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));

                #if MODE_SERIAL
                    if (!i) {

                        Serial.print(F("X send enabled: "));            Serial.println(xSendEnabled[i]);
                        Serial.print(F("Y send enabled: "));            Serial.println(ySendEnabled[i]);
                        Serial.print(F("Note send enabled: "));         Serial.println(noteSendEnabled[i]);
                        Serial.print(F("Aftertouch send enabled: "));   Serial.println(aftertouchSendEnabled[i]);

                    }

                    Serial.print(F("Pad "));
                    Serial.println(i+1);

                    Serial.print(F("CC X MIDI ID: "));              Serial.println(ccXPad[i]);
                    Serial.print(F("CC Y MIDI ID: "));              Serial.println(ccYPad[i]);
                    Serial.print(F("CC X lower limit: "));          Serial.println(ccXminPad[i]);
                    Serial.print(F("CC X upper limit: "));          Serial.println(ccXmaxPad[i]);
                    Serial.print(F("CC Y lower limit: "));          Serial.println(ccYminPad[i]);
                    Serial.print(F("CC Y upper limit: "));          Serial.println(ccYmaxPad[i]);
                    Serial.print(F("Pad curve for X: "));           Serial.println(padCurveX[i]);
                    Serial.print(F("Pad curve for Y: "));           Serial.println(padCurveY[i]);
                #endif

        }

    }   else {  //split xy + functions

        #if MODE_SERIAL > 0
            Serial.println(F("Pads have individual settings and functions - split x/y + functions"));
        #endif

        //pads have individual settings and functions
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

                xSendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ySendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                noteSendEnabled[i]          = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                aftertouchSendEnabled[i]    = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccXPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_X_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_CC_Y_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccXminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccXmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                ccYmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                padCurveX[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_X_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));
                padCurveY[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*i+LOCAL_PROGRAM_SETTING_Y_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*activeProgram));

                #if MODE_SERIAL

                    Serial.print(F("Pad "));
                    Serial.println(i+1);

                    Serial.print(F("X send enabled: "));            Serial.println(xSendEnabled[i]);
                    Serial.print(F("Y send enabled: "));            Serial.println(ySendEnabled[i]);
                    Serial.print(F("Note send enabled: "));         Serial.println(noteSendEnabled[i]);
                    Serial.print(F("Aftertouch send enabled: "));   Serial.println(aftertouchSendEnabled[i]);
                    Serial.print(F("CC X MIDI ID: "));              Serial.println(ccXPad[i]);
                    Serial.print(F("CC Y MIDI ID: "));              Serial.println(ccYPad[i]);
                    Serial.print(F("CC X lower limit: "));          Serial.println(ccXminPad[i]);
                    Serial.print(F("CC X upper limit: "));          Serial.println(ccXmaxPad[i]);
                    Serial.print(F("CC Y lower limit: "));          Serial.println(ccYminPad[i]);
                    Serial.print(F("CC Y upper limit: "));          Serial.println(ccYmaxPad[i]);
                    Serial.print(F("Pad curve for X: "));           Serial.println(padCurveX[i]);
                    Serial.print(F("Pad curve for Y: "));           Serial.println(padCurveY[i]);
                #endif

        }

    }

    sendLEDstateCallback(LED_ON_OFF_SPLIT, getSplitStateLEDvalue());
    sendLEDstateCallback(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendEnabled(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_NOTES, getNoteSendEnabled(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_X, getCCXsendEnabled(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_Y, getCCYsendEnabled(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);

}

void Pads::getPresetParameters()    {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out preset settings"));
        Serial.println(F("----------------------------------------"));
        Serial.print(F("Active preset: "));
        Serial.println(activePreset);
    #endif

    shiftAmount = 0;
    shiftedNote = 0;

    //clear all pad notes before assigning new ones
    for (int i=0; i<NUMBER_OF_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    generateScale((scale_t)activePreset);

    //reset this variable first
    localOctaveValue = DEFAULT_OCTAVE;

    for (int i=0; i<NOTES_PER_PAD; i++)
        if (padNote[0][i] != BLANK_NOTE)    {

            localOctaveValue = (padNote[0][i]) / MIDI_NOTES;
            break;

    }

}

void Pads::generateScale(scale_t scale)    {

    if (isPredefinedScale(scale)) {

        //predefined scale
        uint8_t notesPerScale = getNotesPerScale(scale);
        uint8_t octave = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram)+PREDEFINED_SCALE_PARAMETERS*activePreset);
        uint8_t tonic_ = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram)+PREDEFINED_SCALE_PARAMETERS*activePreset);
        int8_t shift = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram)+PREDEFINED_SCALE_PARAMETERS*activePreset);

        #if MODE_SERIAL > 0
            Serial.println(F("----------------------------------------"));
            Serial.println(F("Parameters for current scale: "));
            Serial.println(F("----------------------------------------"));
            Serial.print(F("Octave: "));    Serial.println(octave);
            Serial.print(F("Tonic: "));     Serial.println(tonic_);
            Serial.print(F("Shift: "));     Serial.println(shift);
        #endif

        uint8_t noteCounter = 0;

        for (int i=0; i<notesPerScale; i++) {

            padNote[i][0] = getScaleNote(scale, i);
            noteCounter++;

        }

        if (notesPerScale < NUMBER_OF_PADS) {

            noteCounter = 0;

            for (int i=notesPerScale; i<NUMBER_OF_PADS; i++)  {

                padNote[i][0] = getScaleNote(scale, noteCounter);
                //these notes are actually in another octave
                padNote[i][0] += MIDI_NOTES;
                noteCounter++;

            }

        }

        //default notes in scale are now applied to pads
        //apply saved octave

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            padNote[i][0] += (MIDI_NOTES*octave);

        }

        //now apply saved tonic
        setTonic((tonic_t)tonic_, true); //internal change, do not write anything to eeprom

        //finally, apply note shift
        if (shift < 0)  {

            shift *= -1;
            for (int i=0; i<shift; i++)
                shiftNote(false, true);

        }   else {

            for (int i=0; i<shift; i++)
                shiftNote(true, true);

        }

    }   else {  //user scales

        uint8_t noteID = (scale - NUMBER_OF_PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                padNote[i][j] = configuration.readParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i));

            }

        }

    }

    #if MODE_SERIAL
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out notes for pads"));
        Serial.println(F("----------------------------------------"));
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            Serial.print(F("Pad "));
            Serial.print(i+1);
            Serial.println(F(":"));

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)    {

                    Serial.println(padNote[i][j]);

                }

            }

        }
    #endif

}

void Pads::getPadLimits()   {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out limits for pads"));
        Serial.println(F("----------------------------------------"));
    #endif

    getPressureLimits();
    getXLimits();
    getYLimits();
    getAfterTouchUpperPressureLimits();

}

void Pads::getPressureLimits()  {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out pressure limits for pads"));
        Serial.println(F("----------------------------------------"));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padPressureLimitLower[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureLowerSection, i);
        padPressureLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, i);

        #if MODE_SERIAL > 0
            Serial.print(F("Lower pressure limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padPressureLimitLower[i]);

            Serial.print(F("Upper pressure limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padPressureLimitUpper[i]);
            Serial.println();
        #endif

    }

}

void Pads::getXLimits()  {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out X limits for pads"));
        Serial.println(F("----------------------------------------"));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padXLimitLower[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, i);
        padXLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, i);

        #if MODE_SERIAL > 0
            Serial.print(F("Lower X limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padXLimitLower[i]);

            Serial.print(F("Upper X limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padXLimitUpper[i]);
            Serial.println();
        #endif

    }

}

void Pads::getYLimits()  {

    #if MODE_SERIAL > 0
        Serial.println(F("----------------------------------------"));
        Serial.println(F("Printing out Y limits for pads"));
        Serial.println(F("----------------------------------------"));
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padYLimitLower[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, i);
        padYLimitUpper[i] = configuration.readParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, i);

        #if MODE_SERIAL > 0
            Serial.print(F("Lower Y limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padYLimitLower[i]);

            Serial.print(F("Upper Y limit for pad "));
            Serial.print(i);
            Serial.print(F(": "));
            Serial.println(padYLimitUpper[i]);
            Serial.println();
        #endif

    }

}

//read from eeprom end

//read from flash

void Pads::getAfterTouchUpperPressureLimits()    {

    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        int32_t afterTouchPressure = padPressureLimitUpper[i] + (int32_t)(((padPressureLimitUpper[i] - padPressureLimitLower[i]) * (int32_t)100) * (uint32_t)DEFAULT_AFTERTOUCH_PRESSURE_RATIO) / 10000;
        padPressureLimitUpperAfterTouch[i] = afterTouchPressure;

    }

}

bool Pads::getCCXsendEnabled(uint8_t padNumber)    {

    return xSendEnabled[padNumber];

}

bool Pads::getCCYsendEnabled(uint8_t padNumber)    {

    return ySendEnabled[padNumber];

}

bool Pads::getNoteSendEnabled(uint8_t padNumber)   {

    return noteSendEnabled[padNumber];

}

bool Pads::getAfterTouchSendEnabled(uint8_t padNumber) {

    return aftertouchSendEnabled[padNumber];

}

splitState_t Pads::getSplitState()    {

    switch(splitCounter)    {

        case 0:
        return splitOff;

        case 1:
        return splitXY;

        case 2:
        return splitXYFunctions;

    }   return splitOff;

}

uint8_t Pads::getActiveProgram()    {

    return activeProgram;

}

uint8_t Pads::getActivePreset()    {

    return activePreset;

}

curveType_t Pads::getPadCurve(curveCoordinate_t coordinate, uint8_t padNumber)    {

    switch(coordinate)   {

        case curveCoordinateX:
        return (curveType_t)padCurveX[padNumber];

        case curveCoordinateY:
        return (curveType_t)padCurveY[padNumber];

        default:
        return curveTypeInvalid;

    }

}

uint8_t Pads::getPadCClimitValue(ccType_t type, ccLimitType_t limitType, uint8_t padNumber)   {

    switch(type)  {

        case ccTypeX:
        if (limitType == ccLimitTypeXmax) return ccXmaxPad[padNumber];
        else return ccXminPad[padNumber];

        case ccTypeY:
        if (limitType == ccLimitTypeYmax) return ccYmaxPad[padNumber];
        else return ccYminPad[padNumber];

    }

    return 0;

}

uint8_t Pads::getPadCCvalue(ccType_t type, uint8_t padNumber)  {

    switch(type)  {

        case ccTypeX:
        return ccXPad[padNumber];

        case ccTypeY:
        return ccYPad[padNumber];

    }   return 0;

}

uint8_t Pads::getMIDIchannel() {

    return midiChannel;

}

uint8_t Pads::getActiveOctave()    {

    return localOctaveValue;

}

tonic_t Pads::getActiveTonic()    {

    if (isUserScale(activePreset)) {

        //currentScaleTonic is first found note on first pad on user presets

        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[0][i] != BLANK_NOTE)
                return getTonicFromNote(padNote[0][i]);

    }   else  {

        //predefined scale tonic is written in eeprom
        int16_t tonicIndex = PREDEFINED_SCALE_TONIC_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram)+PREDEFINED_SCALE_PARAMETERS*activePreset;
        return (tonic_t)configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, tonicIndex);

    }   return tonicInvalid;

}

scaleType_t Pads::getScaleType(uint8_t scale) {

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

    return (scale >= NUMBER_OF_PREDEFINED_SCALES);

}

bool Pads::isPredefinedScale(uint8_t scale) {

    return (scale < NUMBER_OF_PREDEFINED_SCALES);

}

//read from flash end