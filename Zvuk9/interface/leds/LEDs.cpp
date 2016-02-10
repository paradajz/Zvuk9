#include "LEDs.h"
#include "hardware/timer/TimerObject.h"

const uint8_t ledNoteArray[] = {

    LED_NOTE_C,
    LED_NOTE_C_SHARP,
    LED_NOTE_D,
    LED_NOTE_D_SHARP,
    LED_NOTE_E,
    LED_NOTE_F,
    LED_NOTE_F_SHARP,
    LED_NOTE_G,
    LED_NOTE_G_SHARP,
    LED_NOTE_A,
    LED_NOTE_A_SHARP,
    LED_NOTE_B

};

LEDs::LEDs()    {

    //default constructor
    setOutputMacro(LED_ROW_1_DDR, LED_ROW_1_PIN_INDEX);
    setOutputMacro(LED_ROW_2_DDR, LED_ROW_2_PIN_INDEX);
    setOutputMacro(LED_ROW_3_DDR, LED_ROW_3_PIN_INDEX);

    setLowMacro(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setLowMacro(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setLowMacro(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setOutputMacro(DECODER_OUT_1_DDR, DECODER_OUT_1_PIN_INDEX);
    setOutputMacro(DECODER_OUT_2_DDR, DECODER_OUT_2_PIN_INDEX);
    setOutputMacro(DECODER_OUT_3_DDR, DECODER_OUT_3_PIN_INDEX);

}

void LEDs::init()   {

    //

}

void LEDs::allLEDsOff() {

    for (int i=0; i<NUMBER_OF_LEDS; i++)
        timers.setLEDstate(i, ledIntensityOff);

}

void LEDs::allLEDsOn() {

    for (int i=0; i<NUMBER_OF_LEDS; i++)
        timers.setLEDstate(i, ledIntensityFull);

}

void LEDs::setLEDstate(uint8_t ledNumber, ledIntensity_t state)    {

    timers.setLEDstate(ledNumber, state);

}

uint8_t LEDs::getLEDnumberFromTonic(tonic_t _tonic)  {

    return ledNoteArray[_tonic];

}

ledIntensity_t LEDs::getLEDstate(uint8_t ledNumber)    {

    return timers.getLEDstate(ledNumber);

}

void LEDs::setFadeSpeed(uint8_t speed)  {

    timers.setPWMsteps(speed);

}

void LEDs::tonicLEDsOff()   {

    for (int i=0; i<tonicInvalid; i++)
        timers.setLEDstate(ledNoteArray[i], ledIntensityOff);

}

void LEDs::setNoteLEDstate(tonic_t _tonic, ledIntensity_t state)   {

    uint8_t ledNumber = getLEDnumberFromTonic(_tonic);
    timers.setLEDstate(ledNumber, state);

}

ledIntensity_t LEDs::getTonicLEDstate(tonic_t _tonic)   {

    return timers.getLEDstate(getLEDnumberFromTonic(_tonic));

}

LEDs leds;