#ifndef MISC_STRINGS_H_
#define MISC_STRINGS_H_

#include <avr/pgmspace.h>
#include "Arduino.h"

const String emptyLine = "                    ";

const char PROGMEM program_string[] PROGMEM = "P";
const char PROGMEM welcome_string[] PROGMEM = "Welcome!";
const char PROGMEM midiChannel_string[] PROGMEM = "MIDI channel ";

#endif