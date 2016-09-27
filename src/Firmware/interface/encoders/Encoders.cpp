#include "Encoders.h"

#ifdef ENCODERS_H_

#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../eeprom/Configuration.h"

#define ENCODER_SPEED_1         1
#define ENCODER_SPEED_2         5

#define SPEED_TIMEOUT           100

#define ENCODER_DEBOUNCE_TIME   500

Encoders::Encoders()    {

    //default constructor
    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
        lastStepTime[i] = 0;

}

void Encoders::init()   {

    //nothing

}

void Encoders::update(bool process) {

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        int8_t encoderSteps = board.getEncoderState(i);
        if (encoderSteps == 0) continue;

        uint32_t timeDifference = rTimeMillis() - lastStepTime[i];
        uint8_t steps = ENCODER_SPEED_1;

        if (timeDifference > SPEED_TIMEOUT) steps = ENCODER_SPEED_1;
        else steps = ENCODER_SPEED_2;

        lastStepTime[i] = rTimeMillis();

        handleEncoder(i, (encoderSteps > 0), steps);

    }

}

void Encoders::handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps)   {

    //don't allow changing settings using encoders during pad edit mode
    if (pads.editModeActive()) {

        display.displayPadEditChangeParametersError();
        return;

    }

    //allow only program and preset encoder while in menu
    if (menu.menuDisplayed())   {

        if (!((encoderNumber == PROGRAM_ENCODER) || (encoderNumber == PRESET_ENCODER)))
            return;

    }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    bool _splitState = pads.getSplitState();
    uint8_t activeCurve = 0;
    uint8_t value;
    int8_t activePreset;
    bool padsReleased = pads.allPadsReleased();
    coordinateType_t coordinate = coordinateX;
    ccLimitType_t limit = ccLimitTypeMin;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        if (menu.menuDisplayed())
            menu.changeOption(direction);
        else {

            if (!padsReleased) {

                //disable encoders while pads are pressed
                display.displayPadReleaseError(changeProgram);
                return;

            }

            if (buttons.getButtonState(BUTTON_ON_OFF_SPLIT))    {   //change midi channel

                buttons.disable(BUTTON_ON_OFF_SPLIT);

                uint8_t midiChannel = pads.getMIDIchannel(lastTouchedPad);

                if (direction) midiChannel++;
                else           midiChannel--;

                if (midiChannel < 1)  midiChannel = 16;
                if (midiChannel > 16) midiChannel = 1;

                pads.setMIDIchannel(lastTouchedPad, midiChannel);

                display.displayMIDIchannelChange(pads.getMIDIchannel(lastTouchedPad), pads.getSplitState(), lastTouchedPad+1);

            }   else {

                int8_t activeProgram = pads.getActiveProgram();

                if (direction) activeProgram++; else activeProgram--;
                if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
                else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
                pads.setActiveProgram(activeProgram);

                //get last active preset on current program
                uint8_t currentPreset = pads.getActiveScale();

                //preset is changed
                leds.displayActiveNoteLEDs();

                //display preset on display
                display.displayProgramAndScale(activeProgram+1, currentPreset);

            }

        }
        break;

        case PRESET_ENCODER:
        if (menu.menuDisplayed())   {

            menu.confirmOption(direction);
            return;

        }

        if (!padsReleased) {

            //disable encoders while pads are pressed
            display.displayPadReleaseError(changePreset);
            return;

        }

        activePreset = pads.getActiveScale();
        if (direction) activePreset++; else activePreset--;
        if (activePreset == (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
        else if (activePreset < 0) activePreset = (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActiveScale(activePreset);
        leds.displayActiveNoteLEDs();

        //display preset on display
        display.displayProgramAndScale(pads.getActiveProgram()+1, activePreset);
        break;

        case X_CC_ENCODER:
        case Y_CC_ENCODER:
        switch(encoderNumber)   {

            case X_CC_ENCODER:
            coordinate = coordinateX;
            break;

            case Y_CC_ENCODER:
            coordinate = coordinateY;
            break;

        }

        if (!padsReleased) {

            //disable encoders while pads are pressed
            display.displayPadReleaseError(changeCCnumber);
            return;

        }

        pads.changeCC(direction, coordinate, steps);
        display.displayCCchange(coordinate,  _splitState, pads.getCCvalue(coordinate, lastTouchedPad), lastTouchedPad+1);
        break;

        case X_MIN_ENCODER:
        case X_MAX_ENCODER:
        case Y_MIN_ENCODER:
        case Y_MAX_ENCODER:
        switch(encoderNumber)   {

            case X_MIN_ENCODER:
            coordinate = coordinateX;
            limit = ccLimitTypeMin;
            break;

            case X_MAX_ENCODER:
            coordinate = coordinateX;
            limit = ccLimitTypeMax;
            break;

            case Y_MIN_ENCODER:
            coordinate = coordinateY;
            limit = ccLimitTypeMin;
            break;

            case Y_MAX_ENCODER:
            coordinate = coordinateY;
            limit = ccLimitTypeMax;
            break;

        }

        if (!padsReleased) {

            //disable encoders while pads are pressed
            display.displayPadReleaseError(changeCClimit);
            return;

        }

        pads.changeCClimits(direction, coordinate, limit, steps);
        value = pads.getCClimitValue(coordinate, limit, lastTouchedPad);
        display.displayCClimitChange(coordinate, limit, _splitState, value, lastTouchedPad+1);
        break;

        case X_CURVE_ENCODER:
        case Y_CURVE_ENCODER:
        switch(encoderNumber)   {

            case X_CURVE_ENCODER:
            coordinate = coordinateX;
            break;

            case Y_CURVE_ENCODER:
            coordinate = coordinateY;
            break;

        }

        if (!padsReleased) {

            //disable encoders while pads are pressed
            display.displayPadReleaseError(changeCurve);
            return;

        }

        pads.setCCcurve(direction, coordinate);
        activeCurve = pads.getCCcurve(coordinate, lastTouchedPad);
        display.displayCurveChange(coordinate, _splitState, activeCurve, lastTouchedPad+1);
        break;

    }

}

void Encoders::flush()    {

    update(false);

}

Encoders encoders;
#endif