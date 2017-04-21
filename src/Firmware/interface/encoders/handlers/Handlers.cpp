#include "Handlers.h"

#include "../../pads/Pads.h"
#include "../../lcd/LCD.h"
#include "../../lcd/menu/Menu.h"
#include "../../encoders/Encoders.h"
#include "../../../database/Database.h"

void handleProgram(uint8_t id, bool direction, uint8_t steps)
{
    if (menu.menuDisplayed())
    {
        menu.changeOption(direction);
        return;
    }

    if (!pads.allPadsReleased())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeProgram);
        return;
    }

    int8_t activeProgram = pads.getActiveProgram();

    if (direction)
        activeProgram++;
    else
        activeProgram--;

    if (activeProgram == NUMBER_OF_PROGRAMS)
        activeProgram = 0;
    else if (activeProgram < 0)
        activeProgram = (NUMBER_OF_PROGRAMS-1);
    pads.setActiveProgram(activeProgram);

    //get last active scale on current program
    uint8_t currentPreset = pads.getActiveScale();

    //scale is changed
    leds.displayActiveNoteLEDs();

    //display scale on display
    display.displayProgramAndScale(activeProgram+1, currentPreset);
}

void handleScale(uint8_t id, bool direction, uint8_t steps)
{
    if (menu.menuDisplayed())
    {
        menu.confirmOption(direction);
        return;
    }

    if (!pads.allPadsReleased())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changePreset);
        return;
    }

    if (rTimeMs() - menu.getExitTime() < SCALE_ENC_DISABLE_MENU_EXIT)
        return; //disable this encoder for a while after menu is exited to avoid accidental scale change

    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    if (buttons.getModifierState())
    {
        //change midi channel
        buttons.disable(BUTTON_ON_OFF_SPLIT);

        uint8_t midiChannel = pads.getMIDIchannel(lastTouchedPad);

        if (direction)
            midiChannel++;
        else
            midiChannel--;

        if (midiChannel < 1)
            midiChannel = 16;
        if (midiChannel > 16)
            midiChannel = 1;

        pads.setMIDIchannel(lastTouchedPad, midiChannel);
        display.displayMIDIchannelChange(pads.getMIDIchannel(lastTouchedPad), pads.getSplitState(), lastTouchedPad+1);
    }
    else
    {
        int8_t activePreset = pads.getActiveScale();

        if (direction)
        activePreset++;
        else
        activePreset--;

        if (activePreset == (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES))
        activePreset = 0;
        else if (activePreset < 0)
        activePreset = (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActiveScale(activePreset);
        leds.displayActiveNoteLEDs();

        //display scale on display
        display.displayProgramAndScale(pads.getActiveProgram()+1, activePreset);
    }
}

void handleCC(uint8_t id, bool direction, uint8_t steps)
{
    if (!pads.allPadsReleased())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCCnumber);
        return;
    }

    padCoordinate_t coordinate = coordinateX;
    bool splitState = pads.getSplitState();
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch(id)
    {
        case X_CC_ENCODER:
        coordinate = coordinateX;
        break;

        case Y_CC_ENCODER:
        coordinate = coordinateY;
        break;
    }

    pads.changeCCvalue(direction, coordinate, steps);
    display.displayCCchange(coordinate,  splitState, pads.getCCvalue(coordinate, lastTouchedPad), lastTouchedPad+1);
}

void handleLimit(uint8_t id, bool direction, uint8_t steps)
{
    if (!pads.allPadsReleased())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCClimit);
        return;
    }

    padCoordinate_t coordinate = coordinateX;
    ccLimitType_t limit = ccLimitTypeMin;
    bool splitState = pads.getSplitState();
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch(id)
    {
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

    pads.changeCClimitValue(direction, coordinate, limit, steps);
    uint8_t value = pads.getCClimitValue(coordinate, limit, lastTouchedPad);
    display.displayCClimitChange(coordinate, limit, splitState, value, lastTouchedPad+1);
}

void handleCurve(uint8_t id, bool direction, uint8_t steps)
{
    if (!pads.allPadsReleased())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCurve);
        return;
    }

    padCoordinate_t coordinate = coordinateX;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    bool splitState = pads.getSplitState();

    switch(id)
    {
        case X_CURVE_ENCODER:
        coordinate = coordinateX;
        break;

        case Y_CURVE_ENCODER:
        coordinate = coordinateY;
        break;
    }

    pads.setCCcurve(direction, coordinate);
    uint8_t activeCurve = pads.getCCcurve(coordinate, lastTouchedPad);
    display.displayCurveChange(coordinate, splitState, activeCurve, lastTouchedPad+1);
}
