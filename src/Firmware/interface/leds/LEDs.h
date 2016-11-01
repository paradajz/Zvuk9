#pragma once

#include "../../board/Board.h"
#include "../pads/DataTypes.h"

class LEDs
{
    public:
    LEDs();
    void init();
    void update();
    void allLEDsOn();
    void allLEDsDim();
    void allLEDsOff();

    void displayActiveNoteLEDs(bool padEditMode = false, uint8_t padNumber = 0);

    void setLEDstate(uint8_t ledNumber, ledState_t state, bool blink = false);
    ledState_t getLEDstate(uint8_t ledNumber);
    void setFadeSpeed(uint8_t speed);

    uint8_t getLEDnumberFromTonic(note_t note);
    void tonicLEDsOff();
    void setNoteLEDstate(note_t note, ledState_t state);
    ledState_t getTonicLEDstate(note_t note);

    private:
};

extern LEDs leds;
