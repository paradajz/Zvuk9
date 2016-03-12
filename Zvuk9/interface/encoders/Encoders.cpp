#include "Encoders.h"
#include "EncoderSettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/LCD.h"

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

void Encoders::update() {

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        int8_t encoderSteps = timers.getEncoderState(i);
        if (encoderSteps == 0) continue;

        uint32_t timeDifference = newMillis() - lastStepTime[i];
        uint8_t steps = ENCODER_SPEED_1;

        if (timeDifference > SPEED_TIMEOUT) steps = ENCODER_SPEED_1;
        else steps = ENCODER_SPEED_2;

        lastStepTime[i] = newMillis();

        for (int j=0; j<abs(encoderSteps); j++)
            handleEncoder(i, (encoderSteps > 0), steps);

    }

}

void Encoders::handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps)   {

    if (pads.editModeActive()) return; //don't allow changing settings using encoders during pad edit mode

    for (int i=0; i<NUMBER_OF_PADS; i++)
    if (pads.getPadPressed(i)) {

        //disable encoders while pads are pressed
        return;

    }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    splitState_t _splitState = pads.getSplitState();
    curveType_t activeCurve = curveTypeInvalid;
    uint8_t value;
    int8_t activePreset;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        if (buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))    {

            //stop button is modifier
            //disable it on release
            buttons.pauseButton(BUTTON_TRANSPORT_STOP);
            //change midi channel in case it's pressed
            uint8_t midiChannel = pads.getMIDIchannel();

            if (direction) midiChannel++;
            else           midiChannel--;

            if (midiChannel < 1)  midiChannel = 16;
            if (midiChannel > 16) midiChannel = 1;

            pads.setMIDIchannel(midiChannel);

            lcDisplay.displayMIDIchannelChange(pads.getMIDIchannel());

        } else {

            int8_t activeProgram = pads.getActiveProgram();

            if (direction) activeProgram++; else activeProgram--;
            if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
            else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
            pads.setActiveProgram(activeProgram);
            lcDisplay.setProgram(activeProgram+1);

            //get last active preset on current program
            uint8_t currentPreset = pads.getActivePreset();

            //preset is changed
            leds.displayActiveNoteLEDs();

            //display preset on display
            lcDisplay.setPreset(currentPreset);

        }
        break;

        case PRESET_ENCODER:
        activePreset = pads.getActivePreset();
        if (direction) activePreset++; else activePreset--;
        if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
        else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActivePreset(activePreset);

        leds.displayActiveNoteLEDs();

        //display preset on display
        lcDisplay.setPreset(activePreset);
        break;

        case X_CC_ENCODER:
        pads.changeCC(direction, ccTypeX, steps);
        lcDisplay.displayCCchangeMessage(ccTypeX,  _splitState, pads.getPadCCvalue(ccTypeX, lastTouchedPad), lastTouchedPad+1);
        break;

        case Y_CC_ENCODER:
        pads.changeCC(direction, ccTypeY, steps);
        lcDisplay.displayCCchangeMessage(ccTypeY,  _splitState, pads.getPadCCvalue(ccTypeY, lastTouchedPad), lastTouchedPad+1);
        break;

        case X_MAX_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeXmax, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmax, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeXmax, _splitState, value, lastTouchedPad+1);
        break;

        case X_MIN_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeXmin, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmin, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeXmin, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MAX_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeYmax, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmax, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeYmax, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MIN_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeYmin, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmin, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeYmin, _splitState, value, lastTouchedPad+1);
        break;

        case X_CURVE_ENCODER:
        pads.changeCurve(direction, curveCoordinateX);
        activeCurve = pads.getPadCurve(curveCoordinateX, lastTouchedPad);
        lcDisplay.displayCurveChangeMessage(curveCoordinateX, _splitState, activeCurve, lastTouchedPad+1);
        break;

        case Y_CURVE_ENCODER:
        pads.changeCurve(direction, curveCoordinateY);
        activeCurve = pads.getPadCurve(curveCoordinateY, lastTouchedPad);
        lcDisplay.displayCurveChangeMessage(curveCoordinateY, _splitState, activeCurve, lastTouchedPad+1);
        break;

    }

}

Encoders encoders;
