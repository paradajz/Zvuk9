#ifndef MISC_STRINGS_H_
#define MISC_STRINGS_H_

#include <avr/pgmspace.h>

const char PROGMEM emptyLine_string[] PROGMEM = "                    ";
const char PROGMEM program_string[] PROGMEM = "P";
const char PROGMEM velocity_string[] PROGMEM = "v";
const char PROGMEM xyPosition_string[] PROGMEM = "pos";
const char PROGMEM aftertouch_string[] PROGMEM = "at";
const char PROGMEM welcome_string[] PROGMEM = "Welcome!";
const char PROGMEM midiChannelChange_string[] PROGMEM = "MIDI channel ";
const char PROGMEM modifierEnabled_string[] PROGMEM = "Modifier enabled";
const char PROGMEM reset_string[] PROGMEM = "Press Stop to reset";
const char PROGMEM editingPad_string[] PROGMEM = "Editing pad ";
const char PROGMEM aftertouchClear_string[] PROGMEM = "     ";
const char PROGMEM velocityClear_string[] PROGMEM = "    ";
const char PROGMEM midiChannelClear_string[] PROGMEM = "    ";
const char PROGMEM midiChannel_string[] PROGMEM = "ch";
const char PROGMEM dfu_string[] PROGMEM = "Firmware update mode";

#endif