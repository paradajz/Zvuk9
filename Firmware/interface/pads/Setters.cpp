#include "Pads.h"

void Pads::setCCXsendEnabled(uint8_t padNumber, bool state)    {

    switch(splitState)    {

        case false:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            xSendEnabled[i] = state;
        break;

        case true:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_X_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        xSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setCCYsendEnabled(uint8_t padNumber, bool state)    {

    switch(splitState)    {

        case false:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            ySendEnabled[i] = state;
        break;

        case true:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_Y_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        ySendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setNoteSendEnabled(uint8_t padNumber, bool state)   {

    switch(splitState)    {

        case false:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            noteSendEnabled[i] = state;
        break;

        case true:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        noteSendEnabled[padNumber] = state;
        break;

    }

}

void Pads::setAfterTouchSendEnabled(uint8_t padNumber, bool state) {

    switch(splitState)    {

        case false:
        //global
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<MAX_PADS; i++)
            aftertouchSendEnabled[i] = state;
        break;

        case true:
        //local
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)padNumber+LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), state);
        aftertouchSendEnabled[padNumber] = state;
        break;

    }

}

bool Pads::calibrate(coordinateType_t type, calibrationDirection direction, uint8_t pad, uint16_t limit)   {

    switch(type) {

        case coordinateZ:
        //pressure
        switch(direction)   {

            case lower:
            if ((int)limit != padPressureLimitLower[pad])    {

                padPressureLimitLower[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureLowerSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

            case upper:
            if ((int)limit != padPressureLimitUpper[pad])    {

                padPressureLimitUpper[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

        }
        break;

        case coordinateX:
        switch(direction)   {

            case lower:
            if ((int)limit != padXLimitLower[pad])    {

                padXLimitLower[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXlowerSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

            case upper:
            if ((int)limit != padXLimitUpper[pad])    {

                padXLimitUpper[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationXupperSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

        }
        break;

        case coordinateY:
        switch(direction)   {

            case lower:
            if ((int)limit != padYLimitLower[pad])    {

                padYLimitLower[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYlowerSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

            case upper:
            if ((int)limit != padYLimitUpper[pad])    {

                padYLimitUpper[pad] = limit;
                configuration.writeParameter(CONF_BLOCK_PAD_CALIBRATION, padCalibrationYupperSection, (uint16_t)pad, limit);
                return true;

            }   return false;
            break;

        }
        break;

    }   return false;

}

void Pads::setCalibrationMode(bool state, coordinateType_t type)  {

    calibrationEnabled = state;
    activeCalibration = type;
    resetCalibration();

}

void Pads::resetCalibration()   {

    minCalibrationValue = 9999;
    maxCalibrationValue = -9999;

}

bool Pads::setActiveProgram(int8_t program)   {

    if (program < 0 || program >= NUMBER_OF_PROGRAMS)    return false;

    if (program != activeProgram)   {

        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection, (uint16_t)0, program);
        getProgramParameters();
        return true;

    }   return false;

}

bool Pads::setActivePreset(int8_t preset)  {

    if (preset < 0 || preset > 16)    return false;

    if (activePreset != preset) {

        activePreset = preset;
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram, preset);
        getPresetParameters();
        return true;

    }   return false;

}

void Pads::splitOnOff() {

    splitState = !splitState;

    configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), splitState);
    getPadParameters();

    #if MODE_SERIAL > 0
        splitState ? printf("Split on\n") : printf("Split off\n");
    #endif

}

void Pads::setAftertouchType(aftertouchType_t type)   {

    switch(type)    {

        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return; //wrong argument

    }

    configuration.writeParameter(CONF_BLOCK_MIDI, 0, MIDI_SETTING_AFTERTOUCH_TYPE_ID, (uint8_t)type);

    #if MODE_SERIAL > 0
    switch(type)    {

        case aftertouchPoly:
        printf("Changed aftertouch mode - Key aftertouch\n");
        break;

        case aftertouchChannel:
        printf("Changed aftertouch mode - Channel aftertouch\n");
        break;

        default:
        break;

    }
    #endif

    aftertouchType = type;

}

void Pads::changeActiveOctave(bool direction)   {

    //used in pad edit mode
    direction ? activeOctave++ : activeOctave--;

    //overflow check
    if (activeOctave >= MIDI_OCTAVE_RANGE) activeOctave--;
    if (activeOctave < 0) activeOctave++;

}

changeOutput_t Pads::changeCC(bool direction, ccType_t type, int8_t steps)  {

    //public function

    bool globalShift = (splitState == 0);
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

    bool globalShift = (splitState == 0);
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
                #if MODE_SERIAL > 0
                    printf("X max for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccXmaxPad[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("X max for all pads: %d\n", changedValue);
                #endif
                break;

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
                #if MODE_SERIAL > 0
                    printf("X min for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccXminPad[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("X min for all pads: %d\n", changedValue);
                #endif
                break;

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
                #if MODE_SERIAL > 0
                    printf("Y max for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccYmaxPad[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("Y max for all pads: %d\n", changedValue);
                #endif
                break;

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
                #if MODE_SERIAL > 0
                    printf("Y min for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    ccYminPad[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("Y min for all pads: %d\n", changedValue);
                #endif
                break;

            }

        }
        break;

    }

    return result;

}

changeOutput_t Pads::changeCCcurve(bool direction, curveCoordinate_t coordinate, int8_t steps)  {

    bool globalShift = (splitState == 0);
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
                #if MODE_SERIAL > 0
                    printf("X curve for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    padCurveX[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("X curve for all pads: %d\n", changedValue);
                #endif
                break;

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
                #if MODE_SERIAL > 0
                    printf("Y curve for %d pad: %d\n", startPad, changedValue);
                #endif
                break;

                case true:
                //global
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
                for (int i=0; i<MAX_PADS; i++)
                    padCurveY[i] = changedValue;
                #if MODE_SERIAL > 0
                    printf("Y curve for all pads: %d\n", changedValue);
                #endif
                break;

            }

        }
        break;

        case curveCoordinateInvalid:
        return notAllowed;

    }

    return result;

}

bool Pads::setMIDIchannel(uint8_t pad, uint8_t channel)  {

    if (channel != midiChannel[pad]) {

        if (!splitState)    {

            //apply to all pads
            for (int i=0; i<MAX_PADS; i++)
                midiChannel[i] = channel;

            configuration.writeParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID+GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram, channel);

        }   else {

                //apply to single pad only
                midiChannel[pad] = channel;
                configuration.writeParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*pad+LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID)+(LOCAL_PROGRAM_SETTINGS*MAX_PADS*(uint16_t)activeProgram), channel);

        }

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
            printf("Adding note ");
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
                printf("Removing note ");
            #endif

    }

    #if MODE_SERIAL > 0
        printf("%d, to pad %d", (uint8_t)note, pad);
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
        noteShiftLevel++;
        break;

        case false:
        //down, one note lower
        //first note gets decreased, other notes get shifted up
        tempNoteArray[0] = padNote[currentScaleType-1][0] - MIDI_NOTES;
        if (tempNoteArray[0] < 0) return outOfRange;
        for (int i=0; i<MAX_PADS-1; i++)    {

            tempNoteArray[i+1] = padNote[i][0];

        }
        noteShiftLevel--;
        break;

    }

    if (abs(noteShiftLevel) == (uint8_t)currentScaleType)
        noteShiftLevel = 0;

    if (!internalChange)
        configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+(PREDEFINED_SCALE_PARAMETERS*(uint16_t)activePreset), noteShiftLevel);

    #if MODE_SERIAL > 0
        printf("Shifted note: %d\n", noteShiftLevel);
    #endif

    for (int i=0; i<MAX_PADS; i++)
        padNote[i][0] = tempNoteArray[i];

    return outputChanged;

}

changeOutput_t Pads::shiftOctave(bool direction)  {

    //this function shifts octave up or down
    //all pads should shift notes immediately, except for the ones that are still pressed
    //pressed pads should shift note on release

    bool changeAllowed = true;
    changeOutput_t result = outputChanged;

    //check if note/notes are too low/high if shifted
    for (int i=0; i<MAX_PADS; i++)    {

        if (!direction)  {  //shift down

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i]) != BLANK_NOTE)
                if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i]) < MIDI_NOTES)
                changeAllowed = false;

                if (!changeAllowed) break;

            }

            }   else {  //shift up

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i]) != BLANK_NOTE)
                if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i])+MIDI_NOTES > MAX_MIDI_VALUE)
                changeAllowed = false;

                if (!changeAllowed) break;

            }

        }

    }

    if (!changeAllowed)    {

        #if MODE_SERIAL > 0
            printf("Unable to do global shift: one or more pad notes are too ");
            direction ? printf("high\n") : printf("low\n");
        #endif
        result = outOfRange;

    }   else {

        #if MODE_SERIAL > 0
            direction ? printf("Octave up\n") : printf("Octave down\n");
        #endif

        if (isPredefinedScale(activePreset))    {

            //predefined scale
            uint16_t octaveIndex_predefinedScale = PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activePreset;
            uint8_t currentOctave_predefinedScale = configuration.readParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex_predefinedScale);
            uint8_t newOctave = currentOctave_predefinedScale;
            (direction) ? newOctave+=1 : newOctave-=1;
            configuration.writeParameter(CONF_BLOCK_PROGRAM, programScalePredefinedSection, octaveIndex_predefinedScale, newOctave);
            for (int i=0; i<MAX_PADS; i++)  {

                uint8_t newNote = direction ? padNote[i][0] + MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES) : padNote[i][0] - MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES);

                #if MODE_SERIAL > 0
                    printf("New note for pad %d: %d\n", i, newNote);
                #endif

                if (!isPadPressed(i))
                    padNote[i][0] = newNote;
                else {

                    bitWrite(octaveShiftPadBuffer, i, 1);
                    (direction) ? octaveShiftAmount[i]++ : octaveShiftAmount[i]--;

                }

            }

            //octave is ALWAYS first note on pad in predefined presets
            activeOctave = getOctaveFromNote(padNote[0][0]+(octaveShiftAmount[0]*MIDI_NOTES));

            #if MODE_SERIAL > 0
                printf("active octave: %d\n", activeOctave);
            #endif

        }   else {

            //user scale
            uint16_t noteID = ((uint16_t)activePreset - NUMBER_OF_PREDEFINED_SCALES)*(MAX_PADS*NOTES_PER_PAD);
            uint8_t newNote;
            for (int i=0; i<MAX_PADS; i++)    {

                if (isPadPressed(i))    {

                    bitWrite(octaveShiftPadBuffer, i, 1);
                    (direction) ? octaveShiftAmount[i]++ : octaveShiftAmount[i]--;

                }

                #if MODE_SERIAL > 0
                    printf("New notes for pad %d: ", i);
                #endif

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (padNote[i][j] != BLANK_NOTE)    {

                        newNote = (direction) ? padNote[i][j] + MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES) : padNote[i][j] - MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES);
                        configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i), newNote, true);    //async write

                        #if MODE_SERIAL > 0
                            printf("%d ", newNote);
                        #endif

                        if (!isPadPressed(i))
                            padNote[i][j] = newNote;

                    }

                }

                #if MODE_SERIAL > 0
                    printf("\n");
                #endif

            }

            //activeOctave in user presets is always first found note on first pad
            //if pad has no assigned notes, active octave is DEFAULT_OCTAVE

            activeOctave = DEFAULT_OCTAVE;

            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[0][i] != BLANK_NOTE)    {

                    activeOctave = getOctaveFromNote(padNote[0][i] + (MIDI_NOTES*octaveShiftAmount[0]));
                    break;

                }

            }

            #if MODE_SERIAL > 0
                direction ? printf("Octave up, ") : printf("Octave down, ");
                printf("active octave: %d\n", activeOctave);
            #endif

        }

    }

    return result;

}

void Pads::checkRemainingOctaveShift()  {

    if (octaveShiftPadBuffer == 0) return; //nothing left to shift

    for (int i=0; i<MAX_PADS; i++)  {

        if (bitRead(octaveShiftPadBuffer, i))   {

            if (!isPadPressed(i))   {

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (padNote[i][j] != BLANK_NOTE)
                        padNote[i][j] += (MIDI_NOTES*octaveShiftAmount[i]);

                }

                bitWrite(octaveShiftPadBuffer, i, 0);
                octaveShiftAmount[i] = 0;

            }

        }

    }

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

                if (padNote[i][j]+  noteShiftAmount[i] != BLANK_NOTE)
                if ((padNote[i][j] + noteShiftAmount[i] + changeDifference) > MAX_MIDI_VALUE)
                changeAllowed = false;

            }

            else if ((uint8_t)currentScaleTonic > (uint8_t)newTonic) {

                if (padNote[i][j] + noteShiftAmount[i] != BLANK_NOTE)
                if ((padNote[i][j] + noteShiftAmount[i] - changeDifference) < MIN_MIDI_VALUE)
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

            uint8_t newNote;

            if (isPadPressed(i))   {

                shiftDirection ? noteShiftAmount[i] += changeDifference : noteShiftAmount[i] -= changeDifference;
                bitWrite(scaleShiftPadBuffer, i, 1);

            }

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] + noteShiftAmount[i] != BLANK_NOTE)    {

                    if (shiftDirection) newNote = padNote[i][j] + changeDifference + noteShiftAmount[i];
                    else                newNote = padNote[i][j] - changeDifference + noteShiftAmount[i];

                    if (isUserScale(activePreset) && !internalChange)  {

                        configuration.writeParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+j+(NOTES_PER_PAD*i), newNote, true); //async write

                    }

                    if (!isPadPressed(i))
                        padNote[i][j] = newNote;

                }

            }

        }

        #if MODE_SERIAL > 0
            printf("Tonic changed, active tonic %d\n", newTonic);
        #endif

        }   else {

        #if MODE_SERIAL > 0
            printf("Unable to change tonic: one or more pad notes are too ");
            shiftDirection ? printf("high\n") : printf("low\n");
        #endif

        result = outOfRange;

    }

    return result;

}

void Pads::checkRemainingNoteShift()    {

    if (scaleShiftPadBuffer == 0) return; //nothing left to shift

    uint8_t tempPadNotes[MAX_PADS][NOTES_PER_PAD];

    for (int i=0; i<MAX_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            tempPadNotes[i][j] = padNote[i][j];

    for (int i=0; i<MAX_PADS; i++)  {

        if (bitRead(scaleShiftPadBuffer, i))   {

            if (!isPadPressed(i))   {

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (padNote[i][j] != BLANK_NOTE)
                        padNote[i][j] += noteShiftAmount[i];

                }

                bitWrite(scaleShiftPadBuffer, i, 0);
                noteShiftAmount[i] = 0;

            }

        }

    }

    //if note isn't available anymore, make sure to turn the note led off
    for (int i=0; i<MAX_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            if (tempPadNotes[i][j] != BLANK_NOTE)   {

                note_t note = getTonicFromNote(tempPadNotes[i][j]);
                #ifdef MODULE_LEDS
                if (!noteActive(note))
                    leds.setNoteLEDstate(note, ledStateOff);
                #endif

            }

}

void Pads::notesOnOff()    {

    bool newNotesState;

    if (!splitState)   {   //feature splitting is off

        newNotesState = !noteSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setNoteSendEnabled(i, newNotesState);

        #if MODE_SERIAL > 0
            printf("Notes ");
            newNotesState ? printf("on") : printf("off");
            printf(" for all pads\n");
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
            printf("Notes ");
            newNotesState ? printf("on") : printf("off");
            printf(" for pad %d\n", lastPressedPad);
        #endif

        if (!newNotesState) {

            uint8_t pressedPads = 0;
            for (int i=0; i<MAX_PADS; i++)
                if (isPadPressed(i)) pressedPads++;

            sendNotes(lastPressedPad, 0, false);

        }

    }

}

void Pads::xOnOff()    {

    bool newXState;

    if (!splitState)   {   //feature splitting is off

        newXState = !xSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setCCXsendEnabled(i, newXState);

        #if MODE_SERIAL > 0
            printf("X ");
            newXState ? printf("on") : printf("off");
            printf(" for all pads\n");
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newXState = !xSendEnabled[lastPressedPad];

        setCCXsendEnabled(lastPressedPad, newXState);

        #if MODE_SERIAL > 0
            printf("X ");
            newXState ? printf("on") : printf("off");
            printf(" for pad %d\n", lastPressedPad);
        #endif

    }

}

void Pads::yOnOff()    {

    bool newYState;

    if (!splitState)   {   //feature splitting is off

        newYState = !ySendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setCCYsendEnabled(i, newYState);

        #if MODE_SERIAL > 0
            printf("Y ");
            newYState ? printf("on") : printf("off");
            printf(" for all pads\n");
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newYState = !ySendEnabled[lastPressedPad];

        setCCYsendEnabled(lastPressedPad, newYState);

        #if MODE_SERIAL > 0
            printf("Y ");
            newYState ? printf("on") : printf("off");
            printf(" for pad %d\n", lastPressedPad);
        #endif

    }

}

void Pads::aftertouchOnOff()    {

    bool newAfterTouchState;

    if (!splitState)   {   //feature splitting is off

        newAfterTouchState = !aftertouchSendEnabled[0];

        for (int i=0; i<MAX_PADS; i++)
            setAfterTouchSendEnabled(i, newAfterTouchState);

        #if MODE_SERIAL > 0
            printf("Aftertouch ");
            newAfterTouchState ? printf("on ") : printf("off ");
            printf("for all pads\n");
        #endif

    }

    else {  //feature splitting is on

        uint8_t lastPressedPad = getLastTouchedPad();
        newAfterTouchState = !aftertouchSendEnabled[lastPressedPad];

        setAfterTouchSendEnabled(lastPressedPad, newAfterTouchState);

        #if MODE_SERIAL > 0
            printf("Aftertouch ");
            newAfterTouchState ? printf("on") : printf("off");
            printf(" for pad %d\n", lastPressedPad);
        #endif

    }

}

void Pads::setPadPressed(uint8_t padNumber, bool padState) {

    bitWrite(padPressed, padNumber, padState);

}

void Pads::setFunctionLEDs(uint8_t padNumber)   {

    #ifdef MODULE_LEDS
        if (splitState)  {

            //split is on
            //turn off function LEDs first
            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
            leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
            leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
            leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);

            //turn on feature LEDs depending on enabled features for last touched pad
            leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendEnabled(padNumber) ? ledStateFull : ledStateOff);
            leds.setLEDstate(LED_ON_OFF_NOTES, getNoteSendEnabled(padNumber) ? ledStateFull : ledStateOff);
            leds.setLEDstate(LED_ON_OFF_X, getCCXsendEnabled(padNumber) ? ledStateFull : ledStateOff);
            leds.setLEDstate(LED_ON_OFF_Y, getCCYsendEnabled(padNumber) ? ledStateFull : ledStateOff);

        }
    #endif

}