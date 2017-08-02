#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"

bool Pads::setActiveProgram(int8_t program)
{
    assert(PROGRAM_CHECK(program));

    if (program != activeProgram)
    {
        database.update(DB_BLOCK_PROGRAM, programLastActiveProgramSection, (uint16_t)0, program);
        getProgramParameters();
        return true;
    }

    return false;
}

void Pads::setMIDISendState(onOff_t type, bool state)
{
    uint16_t *variablePointer;
    uint16_t configurationID;
    uint8_t lastTouchedPad = getLastTouchedPad();

    switch(type)
    {
        case onOff_notes:
        #ifdef DEBUG
        printf_P(PSTR("Notes "));
        #endif
        variablePointer = &noteSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID;
        break;

        case onOff_aftertouch:
        #ifdef DEBUG
        printf_P(PSTR("Aftertouch "));
        #endif
        variablePointer = &aftertouchSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID;
        break;

        case onOff_x:
        #ifdef DEBUG
        printf_P(PSTR("X "));
        #endif
        variablePointer = &xSendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_X_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_X_ENABLE_ID;
        break;

        case onOff_y:
        #ifdef DEBUG
        printf_P(PSTR("Y "));
        #endif
        variablePointer = &ySendEnabled;
        configurationID = splitEnabled ? (uint16_t)LOCAL_PROGRAM_SETTING_Y_ENABLE_ID : (uint16_t)GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID;
        break;

        default:
        return;
    }

    switch(splitEnabled)
    {
        case false:
        //global
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), state);
        for (int i=0; i<NUMBER_OF_PADS; i++)
            bitWrite(*variablePointer, i, state);
        #ifdef DEBUG
        printf_P(PSTR("%s for all pads\n"), state ? "on" : "off");
        #endif
        break;

        case true:
        //local
        database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)lastTouchedPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), state);
        bitWrite(*variablePointer, lastTouchedPad, state);
        #ifdef DEBUG
        printf_P(PSTR("%s for pad %d\n"), state ? "on" : "off", lastTouchedPad);
        #endif
        break;
    }

    //extra checks
    if (!state && (type == onOff_notes))
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
}

bool Pads::setSplitState(bool state)
{
    if (splitEnabled != state)
    {
        splitEnabled = state;
        database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), splitEnabled);
        getPadParameters();

        #ifdef DEBUG
        printf_P(PSTR("Split %s\n"), splitEnabled ? "on" : "off");
        #endif

        return true;
    }

    return false;
}

bool Pads::calibrateXY(padCoordinate_t type, limitType_t limitType, uint8_t pad, uint16_t limit)
{
    if (limit > 1023)
        return false;

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
            return false;
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
            return false;
        }
        break;

        default:
        return false;
    }

    if (limit != variablePointer[pad])
    {
        variablePointer[pad] = limit;
        database.update(DB_BLOCK_PAD_CALIBRATION, configurationSection, (uint16_t)pad, limit);
        return true;
    }

    return false;
}

void Pads::calibratePressure(uint8_t pad, uint8_t pressureZone, uint16_t limit)
{
    database.update(DB_BLOCK_PAD_CALIBRATION, padCalibrationPressureUpperSection0+pressureZone, pad, limit);

    getPressureLimits();
    getAftertouchLimits();
}

void Pads::setCalibrationMode(bool state, padCoordinate_t type)
{
    calibrationEnabled = state;
    activeCalibration = type;
}

bool Pads::setActiveScale(int8_t scale)
{
    if (scale < 0 || scale > 16)
        return false;

    if (activeScale != scale)
    {
        activeScale = scale;
        database.update(DB_BLOCK_PROGRAM, programLastActiveScaleSection, (uint16_t)activeProgram, scale);
        getScaleParameters();
        return true;
    }

    return false;
}

void Pads::setAftertouchType(aftertouchType_t type)
{
    switch(type)
    {
        case aftertouchPoly:
        case aftertouchChannel:
        //nothing
        break;

        default:
        return; //wrong argument
    }

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
}

bool Pads::setActiveOctave(int8_t octave)
{
    if ((octave < 0) || (octave >= MIDI_NOTES))
        return false;

    activeOctave = octave;

    return true;
}

changeOutput_t Pads::changeCCvalue(bool direction, padCoordinate_t type, int8_t steps)
{
    //public function
    changeOutput_t result = outputChanged;
    uint8_t startPad = !splitEnabled ? 0 : getLastTouchedPad();
    uint8_t compareValue = 124; //last three values are reserved for play, stop and record
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;
    uint8_t *variablePointer;
    uint16_t configurationID;
    int16_t tempResult = 0;

    if (!direction)
    {
        steps *= -1;
        compareValue = 0;
    }

    switch (type)
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
        return noChange;
    }

    tempResult = variablePointer[startPad] + steps;

    if (direction)
        compareResult = tempResult > compareValue;
    else
        compareResult = tempResult < compareValue;

    if (!compareResult)
    {
        //value is fine
        changedValue = tempResult;
    }
    else
    {
        //result out of range
        //just assign compareValue if it's not already assigned
        if (variablePointer[startPad] != compareValue)
        {
            changedValue = compareValue;
        }
        else
        {
            changeAllowed = false;
            result = noChange;
        }
    }

    if (changeAllowed)
    {
        switch(splitEnabled)
        {
            case true:
            //local
            database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), changedValue);
            variablePointer[startPad] = changedValue;
            break;

            case false:
            //global
            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
            for (int i=0; i<NUMBER_OF_PADS; i++)
                variablePointer[i] = changedValue;
            break;
        }
    }

    return result;
}

changeOutput_t Pads::changeCClimitValue(bool direction, padCoordinate_t coordinate, limitType_t limitType, int8_t steps)
{
    changeOutput_t result = outputChanged;
    uint8_t lastPressedPad = getLastTouchedPad();
    uint8_t startPad = !splitEnabled ? 0 : lastPressedPad;
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;
    uint16_t configurationID;
    uint8_t *variablePointer;

    if (!direction)
    {
        steps *= -1;
        compareValue = 0;
    }

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
            return noChange;
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
            return noChange;
        }
        break;

        default:
        return noChange;
    }

    if (direction)
        compareResult = variablePointer[lastPressedPad] + steps > compareValue;
    else
        compareResult = variablePointer[lastPressedPad] + steps < compareValue;

    if (!compareResult)
    {
        changedValue = variablePointer[lastPressedPad]+steps;
    }
    else
    {
        //result out of range
        //just assign compareValue if it's not already assigned
        if (variablePointer[lastPressedPad] != compareValue)
        {
            changedValue = compareValue;
        }
        else
        {
            changeAllowed = false;
            result = noChange;
        }
    }

    if (changeAllowed)
    {
        switch(splitEnabled)
        {
            case true:
            //local
            database.update(DB_BLOCK_PROGRAM, programLocalSettingsSection, (LOCAL_PROGRAM_SETTINGS*(uint16_t)startPad+configurationID)+(LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PADS*(uint16_t)activeProgram), changedValue);
            variablePointer[startPad] = changedValue;
            #ifdef DEBUG
            printf_P(PSTR("pad %d: %d\n"), startPad, changedValue);
            #endif
            break;

            case false:
            //global
            database.update(DB_BLOCK_PROGRAM, programGlobalSettingsSection, configurationID+(GLOBAL_PROGRAM_SETTINGS*(uint16_t)activeProgram), changedValue);
            for (int i=0; i<NUMBER_OF_PADS; i++)
                variablePointer[i] = changedValue;
            #ifdef DEBUG
            printf_P(PSTR("all pads: %d\n"), changedValue);
            #endif
            break;
        }
    }

    return result;
}

bool Pads::setCCcurve(padCoordinate_t coordinate, uint8_t curve)
{
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
        return false;
    }

    if (curve == variablePointer[startPad])
        return false; //no change

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

    return true;
}

bool Pads::setMIDIchannel(uint8_t pad, uint8_t channel)
{
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

        return true;
    }

    return false;
}

changeOutput_t Pads::addNote(uint8_t pad, note_t note)
{
    //used in pad edit mode (user scale)
    //add or delete note on pressed pad

    uint16_t noteIndex = 0;
    //calculate note to be added or removed
    uint8_t newNote = getActiveOctave()*MIDI_NOTES + (uint8_t)note;

    if (newNote > 127)
    {
        display.displayOutOfRange();
        return outOfRange;
    }
    //note can added or removed, assume adding by default
    bool addOrRemove = true;

    //check if calculated note is already assigned to pad
    for (int i=0; i<NOTES_PER_PAD; i++)
    {
        if (padNote[pad][i] == newNote)
        {
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
        {
            display.displayMaxNotesSet();
            return overflow;
        }

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

    return outputChanged;
}

changeOutput_t Pads::shiftNote(bool direction, bool internalChange)
{
    //shift scale one note up or down
    //tonic remains the same, it just gets shifted to other pad

    scaleType_t currentScaleType = getScaleType(activeScale);

    if (currentScaleType == userScale)
        return notAllowed;

    int16_t tempNoteArray[NUMBER_OF_PADS];

    switch(direction)
    {
        case true:
        //up, one note higher
        //last note gets increased, other notes get shifted down
        tempNoteArray[NUMBER_OF_PADS-1] = padNote[NUMBER_OF_PADS-currentScaleType][0] + MIDI_NOTES;

        if (tempNoteArray[NUMBER_OF_PADS-1] > 127)
            return outOfRange;

        for (int i=0; i<NUMBER_OF_PADS-1; i++)
            tempNoteArray[i] = padNote[i+1][0];

        if (!internalChange)
            noteShiftLevel++;
        break;

        case false:
        //down, one note lower
        //first note gets decreased, other notes get shifted up
        tempNoteArray[0] = padNote[currentScaleType-1][0] - MIDI_NOTES;

        if (tempNoteArray[0] < 0)
            return outOfRange;

        for (int i=0; i<NUMBER_OF_PADS-1; i++)
            tempNoteArray[i+1] = padNote[i][0];

        if (!internalChange)
            noteShiftLevel--;
        break;

    }

    if (!internalChange)
    {
        if (abs(noteShiftLevel) == (uint8_t)currentScaleType)
            noteShiftLevel = 0;
    }

    if (!internalChange)
        database.update(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_SHIFT_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale, noteShiftLevel);

    #ifdef DEBUG
    printf_P(PSTR("Notes shifted %s"), direction ? "up\n" : "down\n");
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
        padNote[i][0] = tempNoteArray[i];

    return outputChanged;
}

changeOutput_t Pads::shiftOctave(bool direction)
{
    //this function shifts octave up or down
    //all pads should shift notes immediately, except for the ones that are still pressed
    //pressed pads should shift note on release

    bool changeAllowed = true;
    changeOutput_t result = outputChanged;

    //check if note/notes are too low/high if shifted
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        if (!direction)
        {
            //shift down
            for (int j=0; j<NOTES_PER_PAD; j++)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i]) < MIDI_NOTES)
                        changeAllowed = false;
                }

                if (!changeAllowed)
                    break;
            }
        }
        else
        {
            //shift up
            for (int j=0; j<NOTES_PER_PAD; j++)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if (padNote[i][j]+(MIDI_NOTES*octaveShiftAmount[i])+MIDI_NOTES > 127)
                        changeAllowed = false;
                }

                if (!changeAllowed)
                    break;
            }
        }
    }

    if (!changeAllowed)
    {
        #ifdef DEBUG
        printf_P(PSTR("Unable to do global shift: one or more pad notes are too %s\n"), direction ? "high" : "low");
        #endif
        result = outOfRange;
    }
    else
    {
        #ifdef DEBUG
        printf_P(PSTR("Octave %s.\n"), direction ? "up" : "down");
        #endif

        if (isPredefinedScale(activeScale))
        {
            //predefined scale
            uint16_t octaveIndex_predefinedScale = PREDEFINED_SCALE_OCTAVE_ID+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram)+PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale;
            uint8_t currentOctave_predefinedScale = database.read(DB_BLOCK_SCALE, scalePredefinedSection, octaveIndex_predefinedScale);
            uint8_t newOctave = currentOctave_predefinedScale;
            (direction) ? newOctave+=1 : newOctave-=1;
            database.update(DB_BLOCK_SCALE, scalePredefinedSection, octaveIndex_predefinedScale, newOctave);

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                uint8_t newNote = direction ? padNote[i][0] + MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES) : padNote[i][0] - MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES);

                #ifdef DEBUG
                printf_P(PSTR("New note for pad %d: %d\n"), i, newNote);
                #endif

                if (!isPadPressed(i))
                {
                    padNote[i][0] = newNote;
                }
                else
                {
                    bitWrite(octaveShiftPadBuffer, i, 1);
                    (direction) ? octaveShiftAmount[i]+=1 : octaveShiftAmount[i]-=1;
                }
            }

            //octave is ALWAYS first note on pad in predefined scales
            activeOctave = getOctaveFromNote(padNote[0][0]) + octaveShiftAmount[0];

            #ifdef DEBUG
            printf_P(PSTR("active octave: %d\n"), activeOctave);
            #endif
        }
        else
        {
            //user scale
            uint16_t noteID = ((uint16_t)activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);
            uint8_t newNote;

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                if (isPadPressed(i))
                {
                    bitWrite(octaveShiftPadBuffer, i, 1);
                    (direction) ? octaveShiftAmount[i]++ : octaveShiftAmount[i]--;
                }

                #ifdef DEBUG
                printf_P(PSTR("New notes for pad %d: "), i);
                #endif

                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (padNote[i][j] != BLANK_NOTE)
                    {
                        newNote = (direction) ? padNote[i][j] + MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES) : padNote[i][j] - MIDI_NOTES + (octaveShiftAmount[i]*MIDI_NOTES);
                        database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i), newNote, true);    //async write

                        #ifdef DEBUG
                        printf_P(PSTR("%d "), newNote);
                        #endif

                        if (!isPadPressed(i))
                            padNote[i][j] = newNote;
                    }
                }

                #ifdef DEBUG
                printf_P(PSTR("\n"));
                #endif
            }

            //activeOctave in user scales is always first found note on first pad
            //if pad has no assigned notes, active octave is DEFAULT_OCTAVE

            activeOctave = DEFAULT_OCTAVE;

            for (int i=0; i<NOTES_PER_PAD; i++)
            {
                if (padNote[0][i] != BLANK_NOTE)
                {
                    activeOctave = getOctaveFromNote(padNote[0][i] + (MIDI_NOTES*octaveShiftAmount[0]));
                    break;
                }
            }

            #ifdef DEBUG
            printf_P(PSTR("Octave %s.\n"), direction ? "up" : "down");
            printf_P(PSTR("Active octave: %d\n"), activeOctave);
            #endif
        }
    }

    return result;
}

void Pads::checkRemainingOctaveShift()
{
    if (octaveShiftPadBuffer == 0)
        return; //nothing left to shift

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        if (bitRead(octaveShiftPadBuffer, i))
        {
            if (!isPadPressed(i))
            {
                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (padNote[i][j] != BLANK_NOTE)
                        padNote[i][j] += (MIDI_NOTES*octaveShiftAmount[i]);
                }

                bitWrite(octaveShiftPadBuffer, i, 0);
                octaveShiftAmount[i] = 0;
            }
        }
    }
}

changeOutput_t Pads::setActiveTonic(note_t newTonic, bool internalChange)
{
    if (newTonic >= MIDI_NOTES)
        return outOfRange;

    changeOutput_t result = noChange;
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
        currentScaleTonic = getActiveTonic();
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
    {
        result = noChange;
        return result;
    }

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
                    if ((padNote[i][j] + noteShiftAmount[i] + changeDifference) > 127)
                    changeAllowed = false;
                }
            }
            else if ((uint8_t)currentScaleTonic > (uint8_t)newTonic)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if ((padNote[i][j] + noteShiftAmount[i] - changeDifference) < 0)
                        changeAllowed = false;
                }
            }
        }
    }

    //change notes
    if (changeAllowed)
    {
        result = outputChanged;

        uint16_t noteID = ((uint16_t)activeScale - PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

        if (isPredefinedScale(activeScale) && !internalChange)
            database.update(DB_BLOCK_SCALE, scalePredefinedSection, PREDEFINED_SCALE_TONIC_ID+(PREDEFINED_SCALE_PARAMETERS*(uint16_t)activeScale)+((PREDEFINED_SCALE_PARAMETERS*PREDEFINED_SCALES)*(uint16_t)activeProgram), newTonic);

        for (int i=0; i<NUMBER_OF_PADS; i++)
        {
            uint8_t newNote;

            if (isPadPressed(i))
            {
                shiftDirection ? noteShiftAmount[i] += changeDifference : noteShiftAmount[i] -= changeDifference;
                bitWrite(scaleShiftPadBuffer, i, 1);
            }

            for (int j=0; j<NOTES_PER_PAD; j++)
            {
                if (padNote[i][j] != BLANK_NOTE)
                {
                    if (shiftDirection)
                        newNote = padNote[i][j] + changeDifference + noteShiftAmount[i];
                    else
                        newNote = padNote[i][j] - changeDifference + noteShiftAmount[i];

                    if (isUserScale(activeScale) && !internalChange)
                    {
                        //async write
                        database.update(DB_BLOCK_SCALE, scaleUserSection, noteID+j+(NOTES_PER_PAD*i), newNote, true);
                    }

                    if (!isPadPressed(i))
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

void Pads::checkRemainingNoteShift()
{
    if (scaleShiftPadBuffer == 0)
        return; //nothing left to shift

    uint8_t tempPadNotes[NUMBER_OF_PADS][NOTES_PER_PAD];

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<NOTES_PER_PAD; j++)
            tempPadNotes[i][j] = padNote[i][j];
    }

    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        if (bitRead(scaleShiftPadBuffer, i))
        {
            if (!isPadPressed(i))
            {
                for (int j=0; j<NOTES_PER_PAD; j++)
                {
                    if (padNote[i][j] != BLANK_NOTE)
                        padNote[i][j] += noteShiftAmount[i];
                }

                bitWrite(scaleShiftPadBuffer, i, 0);
                noteShiftAmount[i] = 0;
            }
        }
    }

    //if note isn't available anymore, make sure to turn the note led off
    for (int i=0; i<NUMBER_OF_PADS; i++)
    {
        for (int j=0; j<NOTES_PER_PAD; j++)
        {
            if (tempPadNotes[i][j] != BLANK_NOTE)
            {
                note_t note = getTonicFromNote(tempPadNotes[i][j]);

                if (!isNoteAssigned(note))
                    leds.setNoteLEDstate(note, ledStateOff);
            }
        }
    }

}

void Pads::setPadPressState(uint8_t padNumber, bool padState)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bitWrite(padPressed, padNumber, padState);
    }
}

void Pads::setFunctionLEDs(uint8_t padNumber)
{
    if (splitEnabled)
    {
        //split is on
        //turn off function LEDs first
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledStateOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, ledStateOff);
        leds.setLEDstate(LED_ON_OFF_X, ledStateOff);
        leds.setLEDstate(LED_ON_OFF_Y, ledStateOff);

        //turn on feature LEDs depending on enabled features for last touched pad
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getMIDISendState(padNumber, onOff_aftertouch) ? ledStateFull : ledStateOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, getMIDISendState(padNumber, onOff_notes) ? ledStateFull : ledStateOff);
        leds.setLEDstate(LED_ON_OFF_X, getMIDISendState(padNumber, onOff_x) ? ledStateFull : ledStateOff);
        leds.setLEDstate(LED_ON_OFF_Y, getMIDISendState(padNumber, onOff_y) ? ledStateFull : ledStateOff);
    }
}

void Pads::setVelocitySensitivity(velocitySensitivity_t type)
{
    velocitySensitivity = type;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_SENSITIVITY_ID, type);

    getPressureLimits();
}

void Pads::setVelocityCurve(curve_t curve)
{
    velocityCurve = curve;
    database.update(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsVelocitySensitivity, VELOCITY_SETTING_CURVE_ID, curve);
}