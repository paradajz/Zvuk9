#include "Handlers.h"

#include "../../pads/Pads.h"
#include "../../lcd/LCD.h"
#include "../../lcd/menu/Menu.h"
#include "../../encoders/Encoders.h"
#include "../../../database/Database.h"

void handleProgram(uint8_t id, int8_t steps)
{
    if (menu.isMenuDisplayed())
    {
        menu.changeOption(steps > 0);
        return;
    }

    if (pads.getNumberOfPressedPads())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeProgram);
        return;
    }

    //allow only 1 step change
    steps = steps > 0 ? 1 : -1;

    int8_t newProgram = pads.getProgram()+steps;

    //rollover
    if (newProgram == NUMBER_OF_PROGRAMS)
        newProgram = 0;
    else if (newProgram < 0)
        newProgram = (NUMBER_OF_PROGRAMS-1);

    pads.setProgram(newProgram);

    //scale is changed
    leds.displayActiveNoteLEDs();

    //display scale on display
    display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
}

void handleScale(uint8_t id, int8_t steps)
{
    if (pads.getNumberOfPressedPads())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changePreset);
        return;
    }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    //allow only 1 step change
    steps = steps > 0 ? 1 : -1;

    if (buttons.getModifierState())
    {
        //change midi channel
        buttons.disable(BUTTON_ON_OFF_SPLIT);

        int8_t newMIDIchannel = pads.getMIDIchannel(lastTouchedPad) + steps;

        //rollover
        if (newMIDIchannel < 1)
            newMIDIchannel = 16;
        else if (newMIDIchannel > 16)
            newMIDIchannel = 1;

        pads.setMIDIchannel(lastTouchedPad, newMIDIchannel);
        display.displayMIDIchannelChange();
    }
    else
    {
        int8_t newScale = pads.getScale()+steps;

        //rollover
        if (newScale == (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES))
            newScale = 0;
        else if (newScale < 0)
            newScale = (PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setScale(newScale);
        leds.displayActiveNoteLEDs();

        //display scale on display
        display.displayProgramInfo(pads.getProgram()+1, pads.getScale(), pads.getTonic(), pads.getScaleShiftLevel());
    }
}

void handleCC(uint8_t id, int8_t steps)
{
    if (pads.getNumberOfPressedPads())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCCnumber);
        return;
    }

    padCoordinate_t coordinate = coordinateX;
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

    pads.setCCvalue(coordinate, pads.getCCvalue(lastTouchedPad, coordinate)+steps);
    display.displayCCchange(coordinate, pads.getCCvalue(lastTouchedPad, coordinate));
}

void handleLimit(uint8_t id, int8_t steps)
{
    if (pads.getNumberOfPressedPads() && !pads.isCalibrationEnabled())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCClimit);
        return;
    }

    if (menu.isMenuDisplayed() && !pads.getNumberOfPressedPads())
        return;

    if (!pads.isCalibrationEnabled() && menu.isMenuDisplayed())
        return;

    padCoordinate_t coordinate = coordinateX;
    limitType_t limit = limitTypeMin;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch(id)
    {
        case X_MIN_ENCODER:
        coordinate = coordinateX;
        limit = limitTypeMin;

        if (pads.isCalibrationEnabled())
        {
            steps = steps > 0 ? 1 : -1;
            pads.calibrateXY(lastTouchedPad, coordinateX, limitTypeMin, pads.getCalibrationLimit(lastTouchedPad, coordinateX, limitTypeMin) + steps);
            //refresh value on display
            uint16_t newValue = pads.getScaledXY(lastTouchedPad, board.getPadX(lastTouchedPad), coordinateX, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateX), newValue, 0, 127);
            //display.displayXYposition(newValue, coordinateX);
        }
        break;

        case X_MAX_ENCODER:
        coordinate = coordinateX;
        limit = limitTypeMax;

        if (pads.isCalibrationEnabled())
        {
            steps = steps > 0 ? 1 : -1;
            pads.calibrateXY(lastTouchedPad, coordinateX, limitTypeMax, pads.getCalibrationLimit(lastTouchedPad, coordinateX, limitTypeMax) + steps);
            //refresh value on display
            uint16_t newValue = pads.getScaledXY(lastTouchedPad, board.getPadX(lastTouchedPad), coordinateX, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateX), newValue, 0, 127);
            //display.displayXYposition(newValue, coordinateX);
        }
        break;

        case Y_MIN_ENCODER:
        coordinate = coordinateY;
        limit = limitTypeMin;

        if (pads.isCalibrationEnabled())
        {
            steps = steps > 0 ? 1 : -1;
            //invert lower/upper logic here
            pads.calibrateXY(lastTouchedPad, coordinateY, limitTypeMax, pads.getCalibrationLimit(lastTouchedPad, coordinateY, limitTypeMax) + steps);
            //refresh value on display
            uint16_t newValue = pads.getScaledXY(lastTouchedPad, board.getPadY(lastTouchedPad), coordinateY, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateY), newValue, 0, 127);
            //display.displayXYposition(newValue, coordinateY);
        }
        break;

        case Y_MAX_ENCODER:
        coordinate = coordinateY;
        limit = limitTypeMax;

        if (pads.isCalibrationEnabled())
        {
            steps = steps > 0 ? 1 : -1;
            //invert lower/upper logic here
            pads.calibrateXY(lastTouchedPad, coordinateY, limitTypeMin, pads.getCalibrationLimit(lastTouchedPad, coordinateY, limitTypeMin) + steps);
            //refresh value on display
            uint16_t newValue = pads.getScaledXY(lastTouchedPad, board.getPadY(lastTouchedPad), coordinateY, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateY), newValue, 0, 127);
            //display.displayXYposition(newValue, coordinateY);
        }
        break;
    }

    if (!pads.isCalibrationEnabled())
    {
        pads.setCClimit(coordinate, limit, pads.getCClimit(lastTouchedPad, coordinate, limit) + steps);
        uint8_t value = pads.getCClimit(lastTouchedPad, coordinate, limit);
        display.displayCClimitChange(coordinate, limit, value);
    }
    #ifdef DEBUG
    else
    {
        printf_P(PSTR("Pad %d\nLower limit: %d\nUpper limit: %d\n\n"), lastTouchedPad, pads.getCalibrationLimit(lastTouchedPad, coordinate, limitTypeMin), pads.getCalibrationLimit(lastTouchedPad, coordinate, limitTypeMax));
    }
    #endif
}

void handleCurve(uint8_t id, int8_t steps)
{
    if (pads.getNumberOfPressedPads())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCurve);
        return;
    }

    padCoordinate_t coordinate = coordinateX;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch(id)
    {
        case X_CURVE_ENCODER:
        coordinate = coordinateX;
        break;

        case Y_CURVE_ENCODER:
        coordinate = coordinateY;
        break;
    }

    pads.setCCcurve(coordinate, pads.getCCcurve(lastTouchedPad, coordinate)+steps);
    display.displayCurveChange(coordinate);
}
