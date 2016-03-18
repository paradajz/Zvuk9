#ifndef LCDSTRINGS_H_
#define LCDSTRINGS_H_

#include <avr/pgmspace.h>
#include "Arduino.h"

const char PROGMEM userEditError0[] PROGMEM = "Select user preset";
const char PROGMEM userEditError1[] PROGMEM = "to edit pad notes";
const char PROGMEM userEditError2[] PROGMEM = "Release pad";
const char PROGMEM userEditError3[] PROGMEM = "to edit";

//user messages
//Out of range
//Maximum number of
//notes set

//lcDisplay.displayMessage(1, "Release pad to");
//lcDisplay.displayMessage(2, "change tonic");
//lcDisplay.displayMessage(1, "Modifer enabled");
//lcDisplay.displayMessage(1, "Press STOP to reset");
//lcDisplay.displayMessage(1, "Exit pad mode to");
//lcDisplay.displayMessage(2, "edit parameters");
//lcDisplay.displayMessage(1, "Release pad to");
//lcDisplay.displayMessage(2, "change parameters");


PGM_P const padEditErrorArray[] PROGMEM = {

    userEditError0,
    userEditError1,
    userEditError2,
    userEditError3

};

#endif