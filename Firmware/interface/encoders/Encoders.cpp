#include "Encoders.h"

#ifdef ENCODERS_H_

#include "Encoders.h"
#include "EncoderSettings.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../pads/Pads.h"

#ifdef MODULE_BUTTONS
#include "../buttons/Buttons.h"
#endif

#ifdef MODULE_LCD
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#endif

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

        uint32_t timeDifference = rTimeMillis() - lastStepTime[i];
        uint8_t steps = ENCODER_SPEED_1;

        if (timeDifference > SPEED_TIMEOUT) steps = ENCODER_SPEED_1;
        else steps = ENCODER_SPEED_2;

        lastStepTime[i] = rTimeMillis();

        for (int j=0; j<abs(encoderSteps); j++)
            handleEncoder(i, (encoderSteps > 0), steps);

    }

}

void Encoders::handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps)   {

    //don't allow changing settings using encoders during pad edit mode
    if (pads.editModeActive()) {

        #ifdef MODULE_LCD
            display.displayPadEditChangeParametersError();
        #endif
        return;

    }

    //allow only program while in menu
    #ifdef MODULE_LCD
        if (menu.menuDisplayed() && (encoderNumber != PROGRAM_ENCODER)) return;
    #else
        if (encoderNumber != PROGRAM_ENCODER) return;
    #endif

    for (int i=0; i<MAX_PADS; i++)
    if (pads.isPadPressed(i)) {

        //disable encoders while pads are pressed
        #ifdef MODULE_LCD
            display.displayPadReleaseError(changeParameters);
        #endif
        return;

    }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    bool _splitState = pads.getSplitState();
    curveType_t activeCurve = curveTypeInvalid;
    uint8_t value;
    int8_t activePreset;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        #ifdef MODULE_BUTTONS
        if (buttons.modifierEnabled())    {

            //change midi channel in case it's pressed
            uint8_t midiChannel = pads.getMIDIchannel(lastTouchedPad);

            if (direction) midiChannel++;
            else           midiChannel--;

            if (midiChannel < 1)  midiChannel = 16;
            if (midiChannel > 16) midiChannel = 1;

            pads.setMIDIchannel(lastTouchedPad, midiChannel);

            #ifdef MODULE_LCD
                display.displayMIDIchannelChange(pads.getMIDIchannel(lastTouchedPad), _splitState, lastTouchedPad+1);
            #endif

        } else {
        #else
            {
        #endif

            #ifdef MODULE_LCD
            if (menu.menuDisplayed())   {

                menu.changeOption(direction);

            }   else {
            #else
                {
            #endif

                int8_t activeProgram = pads.getActiveProgram();

                if (direction) activeProgram++; else activeProgram--;
                if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
                else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
                pads.setActiveProgram(activeProgram);

                //get last active preset on current program
                uint8_t currentPreset = pads.getActivePreset();

                //preset is changed
                #ifdef MODULE_LEDS
                    leds.displayActiveNoteLEDs();
                #endif

                //display preset on display
                #ifdef MODULE_LCD
                    display.displayProgramAndPreset(activeProgram+1, currentPreset);
                #endif

            }

        }
        break;

        case PRESET_ENCODER:
        activePreset = pads.getActivePreset();
        if (direction) activePreset++; else activePreset--;
        if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
        else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActivePreset(activePreset);

        #ifdef MODULE_LEDS
            leds.displayActiveNoteLEDs();
        #endif

        //display preset on display
        #ifdef MODULE_LCD
            display.displayProgramAndPreset(pads.getActiveProgram()+1, activePreset);
        #endif
        break;

        case X_CC_ENCODER:
        pads.changeCC(direction, ccTypeX, steps);
        #ifdef MODULE_LCD
            display.displayCCchange(ccTypeX,  _splitState, pads.getCCvalue(ccTypeX, lastTouchedPad), lastTouchedPad+1);
        #endif
        break;

        case Y_CC_ENCODER:
        pads.changeCC(direction, ccTypeY, steps);
        #ifdef MODULE_LCD
            display.displayCCchange(ccTypeY,  _splitState, pads.getCCvalue(ccTypeY, lastTouchedPad), lastTouchedPad+1);
        #endif
        break;

        case X_MAX_ENCODER:
        pads.changeCClimits(direction, ccLimitTypeXmax, steps);
        value = pads.getCClimitValue(ccTypeX, ccLimitTypeXmax, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCClimitChange(ccLimitTypeXmax, _splitState, value, lastTouchedPad+1);
        #endif
        break;

        case X_MIN_ENCODER:
        pads.changeCClimits(direction, ccLimitTypeXmin, steps);
        value = pads.getCClimitValue(ccTypeX, ccLimitTypeXmin, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCClimitChange(ccLimitTypeXmin, _splitState, value, lastTouchedPad+1);
        #endif
        break;

        case Y_MAX_ENCODER:
        pads.changeCClimits(direction, ccLimitTypeYmax, steps);
        value = pads.getCClimitValue(ccTypeY, ccLimitTypeYmax, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCClimitChange(ccLimitTypeYmax, _splitState, value, lastTouchedPad+1);
        #endif
        break;

        case Y_MIN_ENCODER:
        pads.changeCClimits(direction, ccLimitTypeYmin, steps);
        value = pads.getCClimitValue(ccTypeY, ccLimitTypeYmin, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCClimitChange(ccLimitTypeYmin, _splitState, value, lastTouchedPad+1);
        #endif
        break;

        case X_CURVE_ENCODER:
        pads.changeCCcurve(direction, curveCoordinateX);
        activeCurve = pads.getCCcurve(curveCoordinateX, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCurveChange(curveCoordinateX, _splitState, activeCurve, lastTouchedPad+1);
        #endif
        break;

        case Y_CURVE_ENCODER:
        pads.changeCCcurve(direction, curveCoordinateY);
        activeCurve = pads.getCCcurve(curveCoordinateY, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCurveChange(curveCoordinateY, _splitState, activeCurve, lastTouchedPad+1);
        #endif
        break;

    }

}

Encoders encoders;
#endif