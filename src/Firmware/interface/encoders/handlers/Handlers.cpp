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
        display.displayMIDIchannelChange();
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
    display.displayCCchange(coordinate, pads.getCC(lastTouchedPad, coordinate));
}

void handleLimit(uint8_t id, bool direction, uint8_t steps)
{
    if (!pads.allPadsReleased() && !pads.isCalibrationEnabled())
    {
        //disable encoders while pads are pressed
        display.displayPadReleaseError(changeCClimit);
        return;
    }

    if (menu.menuDisplayed() && pads.allPadsReleased())
        return;

    if (!pads.isCalibrationEnabled() && menu.menuDisplayed())
        return;

    padCoordinate_t coordinate = coordinateX;
    ccLimitType_t limit = ccLimitTypeMin;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch(id)
    {
        case X_MIN_ENCODER:
        coordinate = coordinateX;
        limit = ccLimitTypeMin;

        if (pads.isCalibrationEnabled())
        {
            int8_t step = direction ? -1 : 1;
            pads.calibrateXY(coordinateX, lower, lastTouchedPad, pads.getCoordinateLimit(lastTouchedPad, coordinateX, lower) + step);
            //refresh value on display
            uint16_t newValue = pads.scaleXY(lastTouchedPad, board.getPadX(lastTouchedPad), coordinateX, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateX), newValue, 0, 127);
            display.displayXYposition(newValue, coordinateX);
        }
        break;

        case X_MAX_ENCODER:
        coordinate = coordinateX;
        limit = ccLimitTypeMax;

        if (pads.isCalibrationEnabled())
        {
            int8_t step = direction ? -1 : 1;
            pads.calibrateXY(coordinateX, upper, lastTouchedPad, pads.getCoordinateLimit(lastTouchedPad, coordinateX, upper) + step);
            //refresh value on display
            uint16_t newValue = pads.scaleXY(lastTouchedPad, board.getPadX(lastTouchedPad), coordinateX, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateX), newValue, 0, 127);
            display.displayXYposition(newValue, coordinateX);
        }
        break;

        case Y_MIN_ENCODER:
        coordinate = coordinateY;
        limit = ccLimitTypeMin;

        if (pads.isCalibrationEnabled())
        {
            int8_t step = direction ? 1 : -1;
            //invert lower/upper logic here
            pads.calibrateXY(coordinateY, upper, lastTouchedPad, pads.getCoordinateLimit(lastTouchedPad, coordinateY, upper) + step);
            //refresh value on display
            uint16_t newValue = pads.scaleXY(lastTouchedPad, board.getPadY(lastTouchedPad), coordinateY, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateY), newValue, 0, 127);
            display.displayXYposition(newValue, coordinateY);
        }
        break;

        case Y_MAX_ENCODER:
        coordinate = coordinateY;
        limit = ccLimitTypeMax;

        if (pads.isCalibrationEnabled())
        {
            int8_t step = direction ? 1 : -1;
            //invert lower/upper logic here
            pads.calibrateXY(coordinateY, lower, lastTouchedPad, pads.getCoordinateLimit(lastTouchedPad, coordinateY, lower) + step);
            //refresh value on display
            uint16_t newValue = pads.scaleXY(lastTouchedPad, board.getPadY(lastTouchedPad), coordinateY, true);
            newValue = curves.getCurveValue(pads.getCCcurve(lastTouchedPad, coordinateY), newValue, 0, 127);
            display.displayXYposition(newValue, coordinateY);
        }
        break;
    }

    if (!pads.isCalibrationEnabled())
    {
        pads.changeCClimitValue(direction, coordinate, limit, steps);
        uint8_t value = pads.getCClimit(lastTouchedPad, coordinate, limit);
        display.displayCClimitChange(coordinate, limit, value);
    }
    #ifdef DEBUG
    else
    {
        printf_P(PSTR("Pad %d\nLower limit: %d\nUpper limit: %d\n\n"), lastTouchedPad, pads.getCoordinateLimit(lastTouchedPad, coordinate, lower), pads.getCoordinateLimit(lastTouchedPad, coordinate, upper));
    }
    #endif
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

    switch(id)
    {
        case X_CURVE_ENCODER:
        coordinate = coordinateX;
        break;

        case Y_CURVE_ENCODER:
        coordinate = coordinateY;
        break;
    }

    int8_t curve = pads.getCCcurve(lastTouchedPad, coordinate);

    if (direction)
    {
        curve++;
        if (curve >= NUMBER_OF_CURVES)
            curve = NUMBER_OF_CURVES-1;
    }
    else
    {
        curve--;
        if (curve < 0)
            curve = 0;
    }

    pads.setCCcurve(coordinate, curve);
    display.displayCurveChange(coordinate);
}
