/*

    Hardware control functions for pads.

*/

#include "Pads.h"
#include <avr/cpufunc.h>

void Pads::initPadPins() {

    //init mux
    for (int i=0; i<4; i++) {

        pinMode(muxSelectPins[i], OUTPUT);
        pinMode(muxCommonPins[i], OUTPUT);
        digitalWrite(muxSelectPins[i], LOW);
        digitalWrite(muxCommonPins[i], HIGH);

    }

}

void Pads::switchMuxPins()   {

    for (int i=0; i<4; i++)
        digitalWrite(muxSelectPins[i], bitRead(selectedMuxChannel, i));

    selectedMuxChannel++;
    if (selectedMuxChannel == 16)   selectedMuxChannel = 0;

}

void Pads::setMuxInput(uint8_t muxInput)   {

    for (int i=0; i<4; i++)
        digitalWrite(muxSelectPins[i], bitRead(muxInput, i));

}

void Pads::setupPressure()  {

    pinMode(muxCommonPins[0], OUTPUT);
    pinMode(muxCommonPins[1], INPUT);
    pinMode(muxCommonPins[2], INPUT);
    pinMode(muxCommonPins[3], OUTPUT);

    digitalWrite(muxCommonPins[0], HIGH);
    digitalWrite(muxCommonPins[1], LOW);
    digitalWrite(muxCommonPins[2], LOW);
    digitalWrite(muxCommonPins[3], LOW);

    _NOP();

}

void Pads::setupX()  {

    pinMode(muxCommonPins[0], INPUT);
    pinMode(muxCommonPins[1], INPUT);
    pinMode(muxCommonPins[2], OUTPUT);
    pinMode(muxCommonPins[3], OUTPUT);

    digitalWrite(muxCommonPins[0], LOW);
    digitalWrite(muxCommonPins[1], LOW);
    digitalWrite(muxCommonPins[2], LOW);
    digitalWrite(muxCommonPins[3], HIGH);

    _NOP(); _NOP(); _NOP(); _NOP();

}

void Pads::setupY()  {

    //set pins for y
    pinMode(muxCommonPins[0], OUTPUT);
    pinMode(muxCommonPins[1], OUTPUT);
    pinMode(muxCommonPins[2], INPUT);
    pinMode(muxCommonPins[3], INPUT);

    digitalWrite(muxCommonPins[0], LOW);
    digitalWrite(muxCommonPins[1], HIGH);
    digitalWrite(muxCommonPins[2], LOW);
    digitalWrite(muxCommonPins[3], LOW);

    _NOP();

}
