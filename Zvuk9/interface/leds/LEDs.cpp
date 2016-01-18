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
    pinMode(LED_ROW_1_PIN, OUTPUT);
    pinMode(LED_ROW_2_PIN, OUTPUT);
    pinMode(LED_ROW_3_PIN, OUTPUT);

    digitalWrite(LED_ROW_1_PIN, LOW);
    digitalWrite(LED_ROW_2_PIN, LOW);
    digitalWrite(LED_ROW_3_PIN, LOW);

    pinMode(DECODER_OUT_1, OUTPUT);
    pinMode(DECODER_OUT_2, OUTPUT);
    pinMode(DECODER_OUT_3, OUTPUT);

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

void LEDs::setLEDstate(uint8_t ledNumber, ledIntensity state)    {

    timers.setLEDstate(ledNumber, state);

}

uint8_t LEDs::getLEDnumberFromTonic(tonic _tonic)  {

    return ledNoteArray[_tonic];

}

ledIntensity LEDs::getLEDstate(uint8_t ledNumber)    {

    return timers.getLEDstate(ledNumber);

}

void LEDs::setFadeSpeed(uint8_t speed)  {

    timers.setPWMsteps(speed);

}

void LEDs::tonicLEDsOff()   {

    for (int i=0; i<tonicInvalid; i++)
        timers.setLEDstate(ledNoteArray[i], ledIntensityOff);

}

void LEDs::setTonicLEDstate(tonic _tonic, ledIntensity state)   {

    uint8_t ledNumber = getLEDnumberFromTonic(_tonic);
    timers.setLEDstate(ledNumber, state);

}

ledIntensity LEDs::getTonicLEDstate(tonic _tonic)   {

    return timers.getLEDstate(getLEDnumberFromTonic(_tonic));

}

LEDs leds;