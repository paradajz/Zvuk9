#include "Encoders.h"
#include "EncoderSettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#include "../lcd/MessageBuilder.h"

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

    //don't allow changing settings using encoders during pad edit mode
    if (pads.editModeActive()) {

        lcDisplay.displayMessage(1, "Exit pad mode to");
        lcDisplay.displayMessage(2, "edit parameters");
        return;

    }

    //allow only program while in menu
    if (menu.menuDisplayed() && (encoderNumber != PROGRAM_ENCODER)) return;

    for (int i=0; i<NUMBER_OF_PADS; i++)
    if (pads.getPadPressed(i)) {

        //disable encoders while pads are pressed
        lcDisplay.displayMessage(1, "Release pad to");
        lcDisplay.displayMessage(2, "change parameters");
        return;

    }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    splitState_t _splitState = pads.getSplitState();
    curveType_t activeCurve = curveTypeInvalid;
    uint8_t value;
    int8_t activePreset;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        if (buttons.modifierEnabled())    {

            //change midi channel in case it's pressed
            uint8_t midiChannel = pads.getMIDIchannel();

            if (direction) midiChannel++;
            else           midiChannel--;

            if (midiChannel < 1)  midiChannel = 16;
            if (midiChannel > 16) midiChannel = 1;

            pads.setMIDIchannel(midiChannel);

            messageBuilder.displayMIDIchannelChange(pads.getMIDIchannel());

        } else {

            if (menu.menuDisplayed())   {

                menu.changeOption(direction);

            }   else {

                int8_t activeProgram = pads.getActiveProgram();

                if (direction) activeProgram++; else activeProgram--;
                if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
                else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
                pads.setActiveProgram(activeProgram);

                //get last active preset on current program
                uint8_t currentPreset = pads.getActivePreset();

                //preset is changed
                leds.displayActiveNoteLEDs();

                //display preset on display
                messageBuilder.displayProgramAndPreset(activeProgram+1, currentPreset);

            }

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
        messageBuilder.displayProgramAndPreset(pads.getActiveProgram()+1, activePreset);
        break;

        case X_CC_ENCODER:
        pads.changeCC(direction, ccTypeX, steps);
        messageBuilder.displayCCchange(ccTypeX,  _splitState, pads.getPadCCvalue(ccTypeX, lastTouchedPad), lastTouchedPad+1);
        break;

        case Y_CC_ENCODER:
        pads.changeCC(direction, ccTypeY, steps);
        messageBuilder.displayCCchange(ccTypeY,  _splitState, pads.getPadCCvalue(ccTypeY, lastTouchedPad), lastTouchedPad+1);
        break;

        case X_MAX_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeXmax, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmax, lastTouchedPad);
        messageBuilder.displayCClimitChange(ccLimitTypeXmax, _splitState, value, lastTouchedPad+1);
        break;

        case X_MIN_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeXmin, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmin, lastTouchedPad);
        messageBuilder.displayCClimitChange(ccLimitTypeXmin, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MAX_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeYmax, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmax, lastTouchedPad);
        messageBuilder.displayCClimitChange(ccLimitTypeYmax, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MIN_ENCODER:
        pads.changeXYlimits(direction, ccLimitTypeYmin, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmin, lastTouchedPad);
        messageBuilder.displayCClimitChange(ccLimitTypeYmin, _splitState, value, lastTouchedPad+1);
        break;

        case X_CURVE_ENCODER:
        pads.changeCurve(direction, curveCoordinateX);
        activeCurve = pads.getPadCurve(curveCoordinateX, lastTouchedPad);
        messageBuilder.displayCurveChange(curveCoordinateX, _splitState, activeCurve, lastTouchedPad+1);
        break;

        case Y_CURVE_ENCODER:
        pads.changeCurve(direction, curveCoordinateY);
        activeCurve = pads.getPadCurve(curveCoordinateY, lastTouchedPad);
        messageBuilder.displayCurveChange(curveCoordinateY, _splitState, activeCurve, lastTouchedPad+1);
        break;

    }

}

Encoders encoders;
