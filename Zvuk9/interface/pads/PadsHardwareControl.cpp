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

    //pressure is read from x+/y+
    //set 0/5V across x-/y-

    pinMode(muxCommonPins[0], INPUT); //x+
    pinMode(muxCommonPins[1], OUTPUT); //x-
    pinMode(muxCommonPins[2], INPUT); //y+
    pinMode(muxCommonPins[3], OUTPUT); //y-

    digitalWrite(muxCommonPins[1], HIGH); //x+
    digitalWrite(muxCommonPins[3], LOW); //y-

    _NOP(); _NOP(); _NOP(); _NOP();

}

void Pads::setupX()  {

    //x is read from y+
    //set 0/5V across x+/x-

    pinMode(muxCommonPins[0], OUTPUT); //x+
    pinMode(muxCommonPins[1], OUTPUT); //x-
    pinMode(muxCommonPins[2], INPUT); //y+
    pinMode(muxCommonPins[3], INPUT); //y-

    digitalWrite(muxCommonPins[0], LOW);
    digitalWrite(muxCommonPins[1], HIGH);

    _NOP(); _NOP(); _NOP(); _NOP();

}

void Pads::setupY()  {

    //y is read from x+
    //set 0/5V across y+/y-

    pinMode(muxCommonPins[0], INPUT); //x+
    pinMode(muxCommonPins[1], INPUT); //x-
    pinMode(muxCommonPins[2], OUTPUT); //y+
    pinMode(muxCommonPins[3], OUTPUT); //y-

    digitalWrite(muxCommonPins[2], LOW);
    digitalWrite(muxCommonPins[3], HIGH);

    _NOP(); _NOP(); _NOP(); _NOP();

}
