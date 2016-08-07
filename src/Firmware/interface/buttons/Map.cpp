#include "Buttons.h"

#ifdef BUTTONS_H_

uint8_t buttonToLedArray[MAX_NUMBER_OF_BUTTONS];

void Buttons::mapButtonsToLEDs()    {

    buttonToLedArray[BUTTON_NOTE_C]             = LED_NOTE_C;
    buttonToLedArray[BUTTON_NOTE_C_SHARP]       = LED_NOTE_C_SHARP;
    buttonToLedArray[BUTTON_NOTE_D]             = LED_NOTE_D;
    buttonToLedArray[BUTTON_NOTE_D_SHARP]       = LED_NOTE_D_SHARP;
    buttonToLedArray[BUTTON_NOTE_E]             = LED_NOTE_E;
    buttonToLedArray[BUTTON_NOTE_F]             = LED_NOTE_F;
    buttonToLedArray[BUTTON_NOTE_F_SHARP]       = LED_NOTE_F_SHARP;
    buttonToLedArray[BUTTON_NOTE_G]             = LED_NOTE_G;
    buttonToLedArray[BUTTON_NOTE_G_SHARP]       = LED_NOTE_G_SHARP;
    buttonToLedArray[BUTTON_NOTE_A]             = LED_NOTE_A;
    buttonToLedArray[BUTTON_NOTE_A_SHARP]       = LED_NOTE_A_SHARP;
    buttonToLedArray[BUTTON_NOTE_B]             = LED_NOTE_B;

    buttonToLedArray[BUTTON_ON_OFF_NOTES]       = LED_ON_OFF_NOTES;
    buttonToLedArray[BUTTON_ON_OFF_AFTERTOUCH]  = LED_ON_OFF_AFTERTOUCH;
    buttonToLedArray[BUTTON_ON_OFF_X]           = LED_ON_OFF_X;
    buttonToLedArray[BUTTON_ON_OFF_Y]           = LED_ON_OFF_Y;
    buttonToLedArray[BUTTON_ON_OFF_SPLIT]       = LED_ON_OFF_SPLIT;

    buttonToLedArray[BUTTON_TRANSPORT_PLAY]     = LED_TRANSPORT_PLAY;
    buttonToLedArray[BUTTON_TRANSPORT_STOP]     = LED_TRANSPORT_STOP;
    buttonToLedArray[BUTTON_TRANSPORT_RECORD]   = LED_TRANSPORT_RECORD;

    buttonToLedArray[BUTTON_OCTAVE_DOWN]        = LED_OCTAVE_DOWN;
    buttonToLedArray[BUTTON_OCTAVE_UP]          = LED_OCTAVE_UP;

}

uint8_t Buttons::getLEDnumberFromButton(uint8_t buttonNumber)   {

    return buttonToLedArray[buttonNumber];

}

#endif