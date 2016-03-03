#include "Pads.h"

void Pads::setCCXsendEnabled(uint8_t padNumber, uint8_t state)    {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), state);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            xSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_ENABLE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*padNumber), state);
        xSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setCCYsendEnabled(uint8_t padNumber, uint8_t state)    {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), state);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            ySendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_ENABLE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*padNumber), state);
        ySendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setNoteSendEnabled(uint8_t padNumber, uint8_t state)   {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), state);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            noteSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*padNumber), state);
        noteSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state) {

    switch(splitCounter)    {

        case splitOff:
        case splitXY:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), state);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            aftertouchSendEnabled[i] = state;
        break;

        case splitXYFunctions:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*padNumber), state);
        aftertouchSendEnabled[padNumber] = state;
        break;

    }

}

bool Pads::setLowerPressureLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padPressureLimitLower[pad])    {

        padPressureLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitPressureLowerSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperPressureLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padPressureLimitUpper[pad])    {

        padPressureLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitPressureUpperSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setLowerXLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padXLimitLower[pad])    {

        padXLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitXlowerSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperXLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padXLimitUpper[pad])    {

        padXLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitXupperSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setLowerYLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padYLimitLower[pad])    {

        padYLimitLower[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitYlowerSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setUpperYLimit(uint8_t pad, uint16_t limit)   {

    if ((int)limit != padYLimitUpper[pad])    {

        padYLimitUpper[pad] = limit;
        configuration.writeParameter(CONF_BLOCK_LIMIT, limitYupperSection, pad, limit);
        return true;

    }   return false;

}

bool Pads::setActiveProgram(int8_t program)   {

    if (program < 0 || program >= NUMBER_OF_PROGRAMS)    return false;

    if (program != activeProgram)   {

        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection, 0, program);
        getProgramParameters();
        return true;

    }   return false;

}

bool Pads::setActivePreset(uint8_t preset)  {

    if (preset < 0 || preset > 16)    return false;

    if (activePreset != preset) {

        activePreset = preset;
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, activeProgram, preset);
        getPresetParameters();
        return true;

    }   return false;

}

void Pads::splitFunctions() {

    splitCounter++;
    if (splitCounter == splitEnd)  splitCounter = splitOff;

    configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), splitCounter);
    getPadParameters();

    #if MODE_SERIAL
    switch(splitCounter)    {

        case 0:
        Serial.println(F("Split off"));
        break;

        case 1:
        Serial.println(F("Split XY"));
        break;

        case 2:
        Serial.println(F("Split XY + functions"));
        break;

    }

    #endif

}

void Pads::changeActiveOctave(bool direction)   {

    //used in pad edit mode

    if (direction)  localOctaveValue++;
    else localOctaveValue--;

    //overflow check
    if (localOctaveValue >= (MIDI_OCTAVE_RANGE-2)) localOctaveValue--;
    if (localOctaveValue < -2) localOctaveValue++;

}

changeOutput_t Pads::shiftOctave(bool direction)  {

    //this function only checks for whether all notes can be shifted or not
    //actual shifting takes place in checkOctaveShift function when all pads are released

    bool notesChanged = true;
    changeOutput_t result = outputChanged;

    //check if note/notes are too low/high if shifted
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        if (!direction)  {  //shift down

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                if (padNote[i][j] < MIDI_OCTAVE_RANGE)
                notesChanged = false;

                if (!notesChanged) break;

            }

        }   else {  //shift up

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                if ((padNote[i][j] + MIDI_OCTAVE_RANGE) > MAX_MIDI_VALUE)
                notesChanged = false;

                if (!notesChanged) break;

            }

        }

    }

    if (!notesChanged)    {

        #if MODE_SERIAL
            Serial.print("Unable to do global shift: one or more pad notes are too ");
            direction ? Serial.println("high.") : Serial.println("low");
        #endif
        result = outOfRange;

    }   else {

        if (direction) shiftAmount++;
        else shiftAmount--;

        if (isPredefinedScale(activePreset))    {

            //octave is ALWAYS first note on pad
            localOctaveValue = getOctaveFromNote(padNote[0][0] + MIDI_NOTES*shiftAmount);

        }   else {  //user scale

            localOctaveValue = DEFAULT_OCTAVE;
            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[0][i] != BLANK_NOTE)    {

                    localOctaveValue = getOctaveFromNote(padNote[0][i]);
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

    if (!shiftAmount) return; //nothing to shift

    if (checkPadsPressed()) return; //shift only when all pads are released

    static uint8_t localOctaveValueEmptyPad = DEFAULT_OCTAVE;
    bool direction = shiftAmount > 0;
    bool emptyPad = true;
    int16_t octaveIndex = PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram)+PREDEFINED_SCALE_PARAMETERS*activePreset;
    int8_t currentOctave = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex);

    bool predefinedScale = isPredefinedScale(activePreset);

    switch(predefinedScale) {

        case true:
        //predefined scale
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex, currentOctave+shiftAmount);
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            direction ? padNote[i][0] += (MIDI_NOTES*abs(shiftAmount)) : padNote[i][0] -= (MIDI_NOTES*abs(shiftAmount));

        }
        break;

        case false:
        //user scale
        shiftAmount = abs(shiftAmount);
        uint8_t noteID = (activePreset - NUMBER_OF_PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)    {

                    direction ? padNote[i][j] += (MIDI_NOTES*abs(shiftAmount)) : padNote[i][j] -= (MIDI_NOTES*abs(shiftAmount));
                    configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i), padNote[i][j]);

                }

            }

        }
        break;

    }

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[0][i] != BLANK_NOTE) {

            //local octave value is octave that gets displayed on LCD as changed octave
            localOctaveValue = getOctaveFromNote(padNote[0][i]);
            emptyPad = false;
            break;

        }

    }

    if (emptyPad)   {

        //there are no notes on first pad
        //in this case, increase or decrease default octave value and assign it as localOctaveValue

        if (direction) localOctaveValueEmptyPad++; else localOctaveValueEmptyPad--;
        localOctaveValue = localOctaveValueEmptyPad;

    }

    #if MODE_SERIAL > 0
        direction ? Serial.println(F("Octave up")) : Serial.println(F("Octave down"));
    #endif

    shiftAmount = 0;

}

changeOutput_t Pads::setTonic(tonic_t _tonic, bool internalChange)  {

    changeOutput_t result = noChange;
    tonic_t currentScaleTonic;

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

    if (currentScaleTonic == tonicInvalid) return result; //pad has no notes

    shiftDirection = (uint8_t)currentScaleTonic < (uint8_t)_tonic;

    changeDifference = abs((uint8_t)currentScaleTonic - (uint8_t)_tonic);
    if (!changeDifference) { result = noChange; return result; }
    changeAllowed = true;

    //check if all notes are within range before shifting
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        if (!changeAllowed) break;

        for (int j=0; j<NOTES_PER_PAD; j++)  {

            if ((uint8_t)currentScaleTonic < (uint8_t)_tonic)      {

                if (padNote[i][j] != BLANK_NOTE)
                    if ((padNote[i][j] + changeDifference) > MAX_MIDI_VALUE)
                        changeAllowed = false;

            }

            else if ((uint8_t)currentScaleTonic > (uint8_t)_tonic) {

                if (padNote[i][j] != BLANK_NOTE)
                    if ((padNote[i][j] - changeDifference) < MIN_MIDI_VALUE)
                        changeAllowed = false;

            }

        }

    }

    //change notes
    if (changeAllowed)  {

        result = outputChanged;

        uint8_t noteID = (activePreset - NUMBER_OF_PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        if (isPredefinedScale(activePreset) && !internalChange)
            configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+(PREDEFINED_SCALE_PARAMETERS*activePreset)+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*activeProgram), _tonic);

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

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

    }   else {

        #if MODE_SERIAL
            //Serial.print("Unable to change tonic: one or more pad notes are too ");
            //(currentScaleTonic < tonic) ? Serial.println("high.") : Serial.println("low");
        #endif

        result = outOfRange;

    }

    return result;

}

changeOutput_t Pads::changeCC(bool direction, ccType_t type, int8_t steps)  {

    //public function

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_X_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                ccXPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_Y_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                ccYPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                    ccYPad[i] = changedValue;

            }

        }
        break;

    }

    return result;

}

changeOutput_t Pads::changeXYlimits(bool direction, ccLimitType_t ccType, int8_t steps)  {

    Serial.print("direction: "); Serial.println(direction);

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch(ccType)  {

        case ccLimitTypeXmax:
        if (direction)  compareResult = ccXmaxPad[startPad] + steps > compareValue;
        else            compareResult = ccXmaxPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXmaxPad[lastTouchedPad]+steps;
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MAX_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                ccXmaxPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                    ccXmaxPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeXmin:
        if (direction)  compareResult = ccXminPad[startPad] + steps > compareValue;
        else            compareResult = ccXminPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXminPad[lastTouchedPad]+steps;
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MIN_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                ccXminPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                    ccXminPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeYmax:
        if (direction)  compareResult = ccYmaxPad[startPad] + steps > compareValue;
        else            compareResult = ccYmaxPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYmaxPad[lastTouchedPad]+steps;
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MAX_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                ccYmaxPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                ccYmaxPad[i] = changedValue;

            }

        }
        break;

        case ccLimitTypeYmin:
        if (direction)  compareResult = ccYminPad[lastTouchedPad] + steps > compareValue;
        else            compareResult = ccYminPad[lastTouchedPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYminPad[lastTouchedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYminPad[lastTouchedPad] != compareValue)
            changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            switch(globalShift) {

                case false:
                //local
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MIN_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                    ccYminPad[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                    ccYminPad[i] = changedValue;

            }

        }
        break;

    }

    return result;

}

changeOutput_t Pads::changeCurve(bool direction, curveCoordinate_t coordinate, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput_t result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_CURVE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                    padCurveX[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
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
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_CURVE_ID+((LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS)*activeProgram)+(LOCAL_PROGRAM_SETTINGS*startPad), changedValue);
                padCurveY[startPad] = changedValue;
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*activeProgram), changedValue);
                for (int i=0; i<NUMBER_OF_PADS; i++)
                    padCurveY[i] = changedValue;

            }

        }
        break;

        case curveCoordinateInvalid:
        return notAllowed;

    }

    return result;

}

changeOutput_t Pads::setMIDIchannel(uint8_t channel)  {

    if (channel != midiChannel) {

        midiChannel = channel;
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+GLOBAL_PROGRAM_SETTINGS*activeProgram, channel);
        return outputChanged;

    }

    return noChange;

}

changeOutput_t Pads::assignPadNote(uint8_t tonic)    {

    //used in pad edit mode (user scale)
    //add or delete note on pressed pad

    uint8_t noteIndex = 0;
    //calculate note to be added or removed
    uint8_t note = getActiveOctave()*MIDI_OCTAVE_RANGE + tonic;
    //note can added or removed, assume adding by default
    bool addOrRemove = true;

    //check if calculated note is already assigned to pad
    for (int i=0; i<NOTES_PER_PAD; i++)
    if (padNote[lastTouchedPad][i] == note) { addOrRemove = false; noteIndex = i; break; }

    uint8_t noteID = (activePreset - NUMBER_OF_PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

    //if it isn't, add it
    if (addOrRemove)    {

        //get number of assigned notes to last touched pad
        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[lastTouchedPad][i] != BLANK_NOTE)
                noteIndex++;

        //pads cannot have more than NOTES_PER_PAD notes
        if (noteIndex == NOTES_PER_PAD) return outOfRange;

        //assign new pad note
        padNote[lastTouchedPad][noteIndex] = note;
        configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+noteIndex+(NOTES_PER_PAD*lastTouchedPad), note);

        #if MODE_SERIAL >0
            Serial.print(F("Adding note "));
        #endif

    }   else {

        note = BLANK_NOTE; //else delete note (assign BLANK_NOTE)

        //assign new pad note
        padNote[lastTouchedPad][noteIndex] = note;

        //copy note array
        uint8_t tempNoteArray[NOTES_PER_PAD];

        for (int i=0; i<NOTES_PER_PAD; i++)
            tempNoteArray[i] = padNote[lastTouchedPad][i];

        //shift all notes so that BLANK_NOTE is at the end of array
        for (int i=noteIndex; i<(NOTES_PER_PAD-1); i++) {

            padNote[lastTouchedPad][i] = tempNoteArray[i+1];

        }   padNote[lastTouchedPad][NOTES_PER_PAD-1] = BLANK_NOTE;

        for (int i=0; i<NOTES_PER_PAD; i++)
            configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+i+(NOTES_PER_PAD*lastTouchedPad), padNote[lastTouchedPad][i]);

        #if MODE_SERIAL >0
            Serial.print(F("Removing note "));
        #endif

    }

    #if MODE_SERIAL > 0
        Serial.print(tonic);
        Serial.print(F(" to pad "));
        Serial.println(lastTouchedPad);
    #endif

    return outputChanged;

}


changeOutput_t Pads::shiftNote(bool direction, bool internalChange) {

    //shift scale one note up or down
    //tonic remains the same, it just gets shifted to other pad

    scaleType_t currentScaleType = getScaleType(activePreset);
    if (currentScaleType == userScale) return notAllowed;
    int16_t tempNoteArray[NUMBER_OF_PADS];

    switch(direction)   {

        case true:
        //up, one note higher
        //last note gets increased, other notes get shifted down
        tempNoteArray[NUMBER_OF_PADS-1] = padNote[NUMBER_OF_PADS-currentScaleType][0] + MIDI_OCTAVE_RANGE;
        if (tempNoteArray[NUMBER_OF_PADS-1] > 127) return outOfRange;
        for (int i=0; i<NUMBER_OF_PADS-1; i++)    {

            tempNoteArray[i] = padNote[i+1][0];

        }
        shiftedNote++;
        break;

        case false:
        //down, one note lower
        //first note gets decreased, other notes get shifted up
        tempNoteArray[0] = padNote[currentScaleType-1][0] - MIDI_OCTAVE_RANGE;
        if (tempNoteArray[0] < 0) return outOfRange;
        for (int i=0; i<NUMBER_OF_PADS-1; i++)    {

            tempNoteArray[i+1] = padNote[i][0];

        }
        shiftedNote--;
        break;

    }

    if (abs(shiftedNote) == (uint8_t)currentScaleType)
        shiftedNote = 0;

    if (!internalChange)
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+(PREDEFINED_SCALE_PARAMETERS*activePreset), shiftedNote);

    #if MODE_SERIAL
        Serial.print("Shifted note: ");
        Serial.println(shiftedNote);
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
        padNote[i][0] = tempNoteArray[i];

    return outputChanged;

}

void Pads::notesOnOff()    {

    bool newNotesState;

    if (splitCounter != 2)   {   //feature splitting is off

        newNotesState = !noteSendEnabled[0];

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setNoteSendEnabled(i, newNotesState);

        #if MODE_SERIAL
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newNotesState = !noteSendEnabled[lastTouchedPad];

        setNoteSendEnabled(lastTouchedPad, newNotesState);

        #if MODE_SERIAL
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

void Pads::xOnOff()    {

    bool newXState;

    if (splitCounter != 2)   {   //feature splitting is off

        newXState = !xSendEnabled[0];

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setCCXsendEnabled(i, newXState);

        #if MODE_SERIAL
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newXState = !xSendEnabled[lastTouchedPad];

        setCCXsendEnabled(lastTouchedPad, newXState);

        #if MODE_SERIAL
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

void Pads::yOnOff()    {

    bool newYState;

    if (splitCounter != 2)   {   //feature splitting is off

        newYState = !ySendEnabled[0];

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setCCYsendEnabled(i, newYState);

        #if MODE_SERIAL
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newYState = !ySendEnabled[lastTouchedPad];

        setCCYsendEnabled(lastTouchedPad, newYState);

        #if MODE_SERIAL
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

void Pads::aftertouchOnOff()    {

    bool newAfterTouchState;

    if (splitCounter != 2)   {   //feature splitting is off

        newAfterTouchState = !aftertouchSendEnabled[0];

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setAfterTouchSendEnabled(i, newAfterTouchState);

        #if MODE_SERIAL
            Serial.print(F("Aftertouch "));
            newAfterTouchState ? Serial.print(F("on ")) : Serial.print(F("off "));
            Serial.println(F("for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newAfterTouchState = !aftertouchSendEnabled[lastTouchedPad];

        setAfterTouchSendEnabled(lastTouchedPad, newAfterTouchState);

        #if MODE_SERIAL
            Serial.print(F("Aftertouch "));
            newAfterTouchState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}