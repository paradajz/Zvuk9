#pragma once

#include "../../board/Board.h"
#include "../pads/DataTypes.h"
#include "DataTypes.h"

class LEDs
{
    public:
    LEDs();
    void init();

    void setAllOn();
    void setAllOff();

    void setBlinkState(uint8_t ledID, bool state, bool forceOn = false);
    bool getBlinkState(uint8_t ledID);
    void checkBlinkLEDs();

    void displayActiveNoteLEDs(bool padEditMode = false, uint8_t padNumber = 0);

    void setLEDstate(uint8_t ledID, ledState_t state);
    ledState_t getLEDstate(uint8_t ledID);
    void setFadeSpeed(uint8_t speed);
    void setBlinkSpeed(uint16_t blinkTime);

    uint8_t getLEDnumberFromTonic(note_t note);
    void tonicLEDsOff();
    void setNoteLEDstate(note_t note, ledState_t state);
    ledState_t getTonicLEDstate(note_t note);

    private:
    uint8_t getState(uint8_t ledID);
};

extern LEDs leds;
