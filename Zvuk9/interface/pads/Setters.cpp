#include "Pads.h"

void Pads::setCCXsendEnabled(uint8_t padNumber, uint8_t state)    {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            xSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        xSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setCCYsendEnabled(uint8_t padNumber, uint8_t state)    {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            ySendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        ySendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setNoteSendEnabled(uint8_t padNumber, uint8_t state)   {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            noteSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        noteSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state) {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            aftertouchSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        aftertouchSendEnabled[padNumber] = state;
        break;

    }

}

bool Pads::setLowerPressureLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padPressureLimitLower[pad])    {

        padPressureLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureLowerSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperPressureLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padPressureLimitUpper[pad])    {

        padPressureLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setLowerXLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padXLimitLower[pad])    {

        padXLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperXLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padXLimitUpper[pad])    {

        padXLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setLowerYLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padYLimitLower[pad])    {

        padYLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperYLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padYLimitUpper[pad])    {

        padYLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, (uint16_t)pad, limit);
        return true;

    }   return false;

}

bool Pads::setActiveProgram(int8_t program)   {

    if (program < 0 || program >= NUMBER_OF_PROGRAMS)    return false;

    if (program != activeProgram)   {

        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection, (uint16_t)0, program);
        getProgramParameters();
        return true;

    }   return false;

}

bool Pads::setActivePreset(uint8_t preset)  {

    if (preset < 0 || preset > 16)    return false;

    if (activePreset != preset) {

        activePreset = preset;
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram, preset);
        getPresetParameters();
        return true;

    }   return false;

}

void Pads::updateSplit() {

    splitCounter++;
    if (splitCounter == splitEnd)  splitCounter = splitOff;

    configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), splitCounter);
    getPadParameters();

    #if MODE_SERIAL > 0
        switch(splitCounter)    {

            case splitOff:
            Serial.println(F("Split off"));
            break;

            case splitXY:
            Serial.println(F("Split XY"));
            break;

            case splitXYFunctions:
            Serial.println(F("Split XY + functions"));
            break;

        }
    #endif

}

void Pads::changeAftertouchType()   {

    aftertouchType++;
    if (aftertouchType == AFTERTOUCH_TYPES) aftertouchType = 0;
    configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_TYPE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), aftertouchType);

    #if MODE_SERIAL > 0
    switch(aftertouchType)    {

        case aftertouchPoly:
        Serial.println(F("Key aftertouch"));
        break;

        case aftertouchChannel:
        Serial.println(F("Channel aftertouch"));
        break;

        default:
        break;

    }
    #endif

}

void Pads::changeActiveOctave(bool direction)   {

    //used in pad edit mode
    direction ? activeOctave++ : activeOctave--;

    //overflow check
    if (activeOctave >= MIDI_OCTAVE_RANGE) activeOctave--;
    if (activeOctave < 0) activeOctave++;

}

changeOutput_t Pads::shiftOctave(bool direction)  {

    //this function only checks for whether all notes can be shifted or not
    //actual shifting takes place in checkOctaveShift function when all pads are released

    bool changeAllowed = true;
    changeOutput_t result = outputChanged;

    //check if note/notes are too low/high if shifted
    for (int i=0; i<MAX_PADS; i++)    {

        if (!direction)  {  //shift down

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                if (padNote[i][j] < MIDI_NOTES)
                changeAllowed = false;

                if (!changeAllowed) break;

            }

        }   else {  //shift up

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                if ((padNote[i][j] + MIDI_NOTES) > MAX_MIDI_VALUE)
                changeAllowed = false;

                if (!changeAllowed) break;

            }

        }

    }

    if (!changeAllowed)    {

        #if MODE_SERIAL > 0
            Serial.print(F("Unable to do global shift: one or more pad notes are too "));
            direction ? Serial.println(F("high.")) : Serial.println(F("low"));
        #endif
        result = outOfRange;

    }   else {

        if (direction) octaveShiftAmount++;
        else octaveShiftAmount--;

        if (isPredefinedScale(activePreset))    {

            //octave is ALWAYS first note on pad
            activeOctave = getOctaveFromNote(padNote[0][0] + MIDI_NOTES*octaveShiftAmount);

        }   else {

            //user scale
            //activeOctave in user presets is always first found note on first pad
            //if pad has no assigned notes, active octave is DEFAULT_OCTAVE

            activeOctave = DEFAULT_OCTAVE;
            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[0][i] != BLANK_NOTE)    {

                    activeOctave = getOctaveFromNote(padNote[0][i] + MIDI_NOTES*octaveShiftAmount);
                    break;

                }

            }

        }

    }

    return result;

}

void Pads::checkOctaveShift()   {

    //internal
    //this function will shift octave if octave needs to be shifted only when all pads are released

    if (!octaveShiftAmount) return; //nothing to shift

    if (!allPadsReleased()) return; //shift only when all pads are released

    bool direction = octaveShiftAmount > 0;
    uint16_t octaveIndex_predefinedScale = PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activePreset;
    uint8_t currentOctave_predefinedScale = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex_predefinedScale);

    bool predefinedScale = isPredefinedScale(activePreset);

    switch(predefinedScale) {

        case true:
        //predefined scale
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex_predefinedScale, currentOctave_predefinedScale+octaveShiftAmount);
        for (int i=0; i<MAX_PADS; i++)
            direction ? padNote[i][0] += (MIDI_NOTES*abs(octaveShiftAmount)) : padNote[i][0] -= (MIDI_NOTES*abs(octaveShiftAmount));
        break;

        case false:
        //user scale
        octaveShiftAmount = abs(octaveShiftAmount);
        uint16_t noteID = ((uint16_t)activePreset - NUMBER_OF_PREDEFINED_SCALES)*(MAX_PADS*NOTES_PER_PAD);
        for (int i=0; i<MAX_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)    {

                    direction ? padNote[i][j] += (MIDI_NOTES*abs(octaveShiftAmount)) : padNote[i][j] -= (MIDI_NOTES*abs(octaveShiftAmount));
                    configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i), padNote[i][j]);

                }

            }

        }
        break;

    }

    #if MODE_SERIAL > 0
        direction ? Serial.print(F("Octave up, ")) : Serial.print(F("Octave down, "));
        Serial.print(F("active octave: "));
        Serial.println(activeOctave);
    #endif

    octaveShiftAmount = 0;

}

changeOutput_t Pads::setTonic(note_t newTonic, bool internalChange)  {

    changeOutput_t result = noChange;
    note_t currentScaleTonic;

    if (internalChange) {

        //internal change means that this function got called on startup
        //on startup, tonic isn't applied yet, so it's first note on first pad by default
        //this never gets called on startup for user presets
        currentScaleTonic = getTonicFromNote(padNote[0][0]);

    }   else currentScaleTonic = getActiveTonic();

    //determine distance between notes
    uint8_t changeDifference;

    bool changeAllowed = true;
    bool shiftDirection;

    if (currentScaleTonic == MIDI_NOTES) return result; //pad has no notes

    shiftDirection = (uint8_t)currentScaleTonic < (uint8_t)newTonic;

    changeDifference = abs((uint8_t)currentScaleTonic - (uint8_t)newTonic);
    if (!changeDifference) { result = noChange; return result; }
    changeAllowed = true;

    //check if all notes are within range before shifting
    for (int i=0; i<MAX_PADS; i++)    {

        if (!changeAllowed) break;

        for (int j=0; j<NOTES_PER_PAD; j++)  {

            if ((uint8_t)currentScaleTonic < (uint8_t)newTonic)      {

                if (padNote[i][j] != BLANK_NOTE)
                    if ((padNote[i][j] + changeDifference) > MAX_MIDI_VALUE)
                        changeAllowed = false;

            }

            else if ((uint8_t)currentScaleTonic > (uint8_t)newTonic) {

                if (padNote[i][j] != BLANK_NOTE)
                    if ((padNote[i][j] - changeDifference) < MIN_MIDI_VALUE)
                        changeAllowed = false;

            }

        }

    }

    //change notes
    if (changeAllowed)  {

        result = outputChanged;

        uint16_t noteID = ((uint16_t)activePreset - NUMBER_OF_PREDEFINED_SCALES)*(MAX_PADS*NOTES_PER_PAD);

        if (isPredefinedScale(activePreset) && !internalChange)
            configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+(PREDEFINED_SCALE_PARAMETERS*(uint16_t)activePreset)+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*(uint16_t)activeProgram), newTonic);

        for (int i=0; i<MAX_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)    {

                    if (shiftDirection) padNote[i][j] += changeDifference;
                    else                padNote[i][j] -= changeDifference;

                    if (isUserScale(activePreset) && !internalChange)  {

                        configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i), padNote[i][j]);

                    }

                }

            }

        }

        #if MODE_SERIAL > 0
            Serial.println(F("Tonic changed, active tonic "));
            Serial.print(newTonic);
            Serial.print(F("/"));
        #endif

    }   else {

        #if MODE_SERIAL > 0
            Serial.print(F("Unable to change tonic: one or more pad notes are too "));
            shiftDirection ? Serial.println(F("high.")) : Serial.println(F("low"));
        #endif

        result = outOfRange;

    }

    return result;

}

changeOutput_t Pads::changeCC(bool direction, ccType_t type, int8_t steps)  {

    //public function

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t startPad = globalShift ? 0 : getLastTouchedPad();
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch (type) {

        case ccTypeX:
        if (direction)  compareResult = ccXPad[startPad] + steps > compareValue;
        else            compareResult = ccXPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXPad[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXPad[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_CC_X_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                ccXPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccXPad[i] = changedValue;

            }

        }
        break;

        case ccTypeY:
        if (direction)  compareResult = ccYPad[startPad] + steps > compareValue;
        else            compareResult = ccYPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYPad[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYPad[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_CC_Y_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                ccYPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccYPad[i] = changedValue;

            }

        }
        break;

    }

    return result;

}

changeOutput_t Pads::changeCClimits(bool direction, ccLimitType_t ccType, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t lastPressedPad = getLastTouchedPad();
    uint8_t startPad = globalShift ? 0 : lastPressedPad;
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch(ccType)  {

        case ccLimitTypeXmax:
        if (direction)  compareResult = ccXmaxPad[startPad] + steps > compareValue;
        else            compareResult = ccXmaxPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXmaxPad[lastPressedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXmaxPad[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_X_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                ccXmaxPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccXmaxPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeXmin:
        if (direction)  compareResult = ccXminPad[startPad] + steps > compareValue;
        else            compareResult = ccXminPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXminPad[lastPressedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXminPad[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_X_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                ccXminPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccXminPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeYmax:
        if (direction)  compareResult = ccYmaxPad[startPad] + steps > compareValue;
        else            compareResult = ccYmaxPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYmaxPad[lastPressedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYmaxPad[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_Y_MAX_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                ccYmaxPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                ccYmaxPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeYmin:
        if (direction)  compareResult = ccYminPad[lastPressedPad] + steps > compareValue;
        else            compareResult = ccYminPad[lastPressedPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYminPad[lastPressedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYminPad[lastPressedPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_Y_MIN_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                    ccYminPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccYminPad[i] = changedValue;

            }

        }
        break;

    }

    return result;

}

changeOutput_t Pads::changeCCcurve(bool direction, curveCoordinate_t coordinate, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t lastPressedPad = getLastTouchedPad();
    uint8_t startPad = globalShift ? 0 : lastPressedPad;
    uint8_t compareValue = NUMBER_OF_SCALES;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch(coordinate)   {

        case curveCoordinateX:
        if (direction)  compareResult = padCurveX[startPad] + steps > compareValue;
        else            compareResult = padCurveX[startPad] + steps < compareValue;

        if (!compareResult) changedValue = padCurveX[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (padCurveX[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_X_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                    padCurveX[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    padCurveX[i] = changedValue;

            }

        }
        break;

        case curveCoordinateY:
        if (direction)  compareResult = padCurveY[startPad] + steps > compareValue;
        else            compareResult = padCurveY[startPad] + steps < compareValue;

        if (!compareResult) changedValue = padCurveY[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (padCurveY[startPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+LOCAL_PROGRAM_SETTING_Y_CURVE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), changedValue);
                padCurveY[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    padCurveY[i] = changedValue;

            }

        }
        break;

        case curveCoordinateInvalid:
        return notAllowed;

    }

    return result;

}

bool Pads::setMIDIchannel(uint8_t channel)  {

    if (channel != midiChannel) {

        midiChannel = channel;
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram, channel);
        return true;

    }   return false;

}

changeOutput_t Pads::assignPadNote(uint8_t pad, note_t note)    {

    //used in pad edit mode (user scale)
    //add or delete note on pressed pad

    uint16_t noteIndex = 0;
    //calculate note to be added or removed
    uint8_t newNote = getActiveOctave()*MIDI_NOTES + (uint8_t)note;
    if (newNote > 127) {

        #ifdef MODULE_LCD
            display.displayOutOfRange();
        #endif
        return outOfRange;

    }
    //note can added or removed, assume adding by default
    bool addOrRemove = true;

    //check if calculated note is already assigned to pad
    for (int i=0; i<NOTES_PER_PAD; i++)
        if (padNote[pad][i] == newNote) { addOrRemove = false; noteIndex = i; break; }

    uint16_t noteID = ((uint16_t)activePreset - NUMBER_OF_PREDEFINED_SCALES)*(MAX_PADS*NOTES_PER_PAD);

    //if it isn't, add it
    if (addOrRemove)    {

        //get number of assigned notes to last touched pad
        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[pad][i] != BLANK_NOTE)
                noteIndex++;

        //pads cannot have more than NOTES_PER_PAD notes
        if (noteIndex == NOTES_PER_PAD) {

            #ifdef MODULE_LCD
            display.displayMaxNotesSet();
            #endif
            return overflow;

        }

        //assign new pad note
        padNote[pad][noteIndex] = newNote;
        configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+noteIndex+(NOTES_PER_PAD*(uint16_t)pad), newNote);

        #if MODE_SERIAL > 0
            Serial.print(F("Adding note "));
        #endif

    }   else {

            newNote = BLANK_NOTE; //else delete note (assign BLANK_NOTE)

            //assign new pad note
            padNote[pad][noteIndex] = newNote;

            //copy note array
            uint8_t tempNoteArray[NOTES_PER_PAD];

            for (int i=0; i<NOTES_PER_PAD; i++)
                tempNoteArray[i] = padNote[pad][i];

            //shift all notes so that BLANK_NOTE is at the end of array
            for (int i=noteIndex; i<(NOTES_PER_PAD-1); i++) {

                padNote[pad][i] = tempNoteArray[i+1];

            }   padNote[pad][NOTES_PER_PAD-1] = BLANK_NOTE;

            for (int i=0; i<NOTES_PER_PAD; i++)
                configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+i+(NOTES_PER_PAD*(uint16_t)pad), padNote[pad][i]);

            #if MODE_SERIAL > 0
                Serial.print(F("Removing note "));
            #endif

    }

    #if MODE_SERIAL > 0
        Serial.print((uint8_t)note);
        Serial.print(F(" to pad "));
        Serial.println(pad);
    #endif

    return outputChanged;

}

changeOutput_t Pads::shiftNote(bool direction, bool internalChange) {

    //shift scale one note up or down
    //tonic remains the same, it just gets shifted to other pad

    scaleType_t currentScaleType = getScaleType(activePreset);
    if (currentScaleType == userScale) return notAllowed;
    int16_t tempNoteArray[MAX_PADS];

    switch(direction)   {

        case true:
        //up, one note higher
        //last note gets increased, other notes get shifted down
        tempNoteArray[MAX_PADS-1] = padNote[MAX_PADS-currentScaleType][0] + MIDI_NOTES;
        if (tempNoteArray[MAX_PADS-1] > 127) return outOfRange;
        for (int i=0; i<MAX_PADS-1; i++)    {

            tempNoteArray[i] = padNote[i+1][0];

        }
        noteShiftAmount++;
        break;

        case false:
        //down, one note lower
        //first note gets decreased, other notes get shifted up
        tempNoteArray[0] = padNote[currentScaleType-1][0] - MIDI_NOTES;
        if (tempNoteArray[0] < 0) return outOfRange;
        for (int i=0; i<MAX_PADS-1; i++)    {

            tempNoteArray[i+1] = padNote[i][0];

        }
        noteShiftAmount--;
        break;

    }

    if (abs(noteShiftAmount) == (uint8_t)currentScaleType)
        noteShiftAmount = 0;

    if (!internalChange)
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+(PREDEFINED_SCALE_PARAMETERS*(uint16_t)activePreset), noteShiftAmount);

    #if MODE_SERIAL > 0
        Serial.print("Shifted note: ");
        Serial.println(noteShiftAmount);
    #endif

    for (int i=0; i<MAX_PADS; i++)
        padNote[i][0] = tempNoteArray[i];

    return outputChanged;

}

void Pads::notesOnOff()    {

    bool newNotesState;

    if (splitCounter != 2)   {   //feature splitting is off

        newNotesState = !noteSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setNoteSendEnabled(i, newNotesState);

        #if MODE_SERIAL > 0
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

        if (!newNotesState) {

            //we have turned notes off for all pads
            //if there are pressed pads, send notes off
            for (int i=0; i<MAX_PADS; i++)    {

                if (!isPadPressed(i)) continue; //only send note off for released pads
                sendNotes(i, 0, false);

            }

        }

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newNotesState = !noteSendEnabled[lastPressedPad];

        setNoteSendEnabled(lastPressedPad, newNotesState);

        #if MODE_SERIAL > 0
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastPressedPad);
        #endif

        if (!newNotesState) {

            uint8_t pressedPads = 0;
            for (int i=0; i<MAX_PADS; i++)
                if (isPadPressed(i)) pressedPads++;

            sendNotes(getLastTouchedPad(), 0, false);

        }

    }

}

void Pads::xOnOff()    {

    bool newXState;

    if (splitCounter != 2)   {   //feature splitting is off

        newXState = !xSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setCCXsendEnabled(i, newXState);

        #if MODE_SERIAL > 0
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newXState = !xSendEnabled[lastPressedPad];

        setCCXsendEnabled(lastPressedPad, newXState);

        #if MODE_SERIAL > 0
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastPressedPad);
        #endif

    }

}

void Pads::yOnOff()    {

    bool newYState;

    if (splitCounter != 2)   {   //feature splitting is off

        newYState = !ySendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setCCYsendEnabled(i, newYState);

        #if MODE_SERIAL > 0
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newYState = !ySendEnabled[lastPressedPad];

        setCCYsendEnabled(lastPressedPad, newYState);

        #if MODE_SERIAL > 0
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastPressedPad);
        #endif

    }

}

void Pads::aftertouchOnOff()    {

    bool newAfterTouchState;

    if (splitCounter != 2)   {   //feature splitting is off

        newAfterTouchState = !aftertouchSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setAfterTouchSendEnabled(i, newAfterTouchState);

        #if MODE_SERIAL > 0
            Serial.print(F("Aftertouch "));
            newAfterTouchState ? Serial.print(F("on ")) : Serial.print(F("off "));
            Serial.println(F("for all pads"));
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newAfterTouchState = !aftertouchSendEnabled[lastPressedPad];

        setAfterTouchSendEnabled(lastPressedPad, newAfterTouchState);

        #if MODE_SERIAL > 0
            Serial.print(F("Aftertouch "));
            newAfterTouchState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastPressedPad);
        #endif

    }

}

void Pads::setPadPressed(uint8_t padNumber, bool padState) {

    bitWrite(padPressed, padNumber, padState);

}

void Pads::setFunctionLEDs(uint8_t padNumber)   {

    #ifdef MODULE_LEDS
        if (splitCounter == splitXYFunctions)  {

            //split features
            //turn off function LEDs first
            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_NOTES, ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_X, ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_Y, ledIntensityOff);

            //turn on feature LEDs depending on enabled features for last touched pad
            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_NOTES, getNoteSendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_X, getCCXsendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);
            leds.setLEDstate(LED_ON_OFF_Y, getCCYsendEnabled(padNumber) ? ledIntensityFull : ledIntensityOff);

        }
    #endif

}