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

void Encoders::update(bool process) {

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

    //allow only program and preset encoder while in menu
    #ifdef MODULE_LCD
    if (menu.menuDisplayed())   {

        if (!((encoderNumber == PROGRAM_ENCODER) || (encoderNumber == PRESET_ENCODER)))
            return;

    }
    #endif

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
        #ifdef MODULE_LCD
        if (menu.menuDisplayed())
            menu.changeOption(direction);
        else
        #endif
        {
            if (!padsReleased) {

                //disable encoders while pads are pressed
                #ifdef MODULE_LCD
                display.displayPadReleaseError(changeProgram);
                #endif
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

                #ifdef MODULE_LCD
                display.displayMIDIchannelChange(pads.getMIDIchannel(lastTouchedPad), pads.getSplitState(), lastTouchedPad+1);
                #endif

            }   else {

                int8_t activeProgram = pads.getActiveProgram();

                if (direction) activeProgram++; else activeProgram--;
                if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
                else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
                pads.setActiveProgram(activeProgram);

                //get last active preset on current program
                uint8_t currentPreset = pads.getActiveScale();

                //preset is changed
                #ifdef MODULE_LEDS
                leds.displayActiveNoteLEDs();
                #endif

                //display preset on display
                #ifdef MODULE_LCD
                display.displayProgramAndScale(activeProgram+1, currentPreset);
                #endif

            }

        }
        break;

        case PRESET_ENCODER:
        #ifdef MODULE_LCD
        if (menu.menuDisplayed())   {

            menu.confirmOption(direction);
            return;

        }
        #endif

        if (!padsReleased) {

            //disable encoders while pads are pressed
            #ifdef MODULE_LCD
            display.displayPadReleaseError(changePreset);
            #endif
            return;

        }

        activePreset = pads.getActiveScale();
        if (direction) activePreset++; else activePreset--;
        if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
        else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActiveScale(activePreset);

        #ifdef MODULE_LEDS
            leds.displayActiveNoteLEDs();
        #endif

        //display preset on display
        #ifdef MODULE_LCD
            display.displayProgramAndScale(pads.getActiveProgram()+1, activePreset);
        #endif
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
            #ifdef MODULE_LCD
            display.displayPadReleaseError(changeCCnumber);
            #endif
            return;

        }

        pads.changeCC(direction, coordinate, steps);
        #ifdef MODULE_LCD
            display.displayCCchange(coordinate,  _splitState, pads.getCCvalue(coordinate, lastTouchedPad), lastTouchedPad+1);
        #endif
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
            #ifdef MODULE_LCD
            display.displayPadReleaseError(changeCClimit);
            #endif
            return;

        }

        pads.changeCClimits(direction, coordinate, limit, steps);
        value = pads.getCClimitValue(coordinate, limit, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCClimitChange(coordinate, limit, _splitState, value, lastTouchedPad+1);
        #endif
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
            #ifdef MODULE_LCD
            display.displayPadReleaseError(changeCurve);
            #endif
            return;

        }

        pads.setCCcurve(direction, coordinate);
        activeCurve = pads.getCCcurve(coordinate, lastTouchedPad);
        #ifdef MODULE_LCD
            display.displayCurveChange(coordinate, _splitState, activeCurve, lastTouchedPad+1);
        #endif
        break;

    }

}

void Encoders::flush()    {

    update(false);

}

Encoders encoders;
#endif