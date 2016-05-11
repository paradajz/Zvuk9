#ifndef MISC_STRINGS_H_
#define MISC_STRINGS_H_

#include <avr/pgmspace.h>

const char emptyLine_string[] PROGMEM = "                    ";
const char program_string[] PROGMEM = "P";
const char aftertouch_string[] PROGMEM = "at";
const char welcome_string[] PROGMEM = "Welcome!";
const char midiChannelChange_string[] PROGMEM = "MIDI channel ";
const char modifierEnabled_string[] PROGMEM = "Modifier enabled";
const char reset_string[] PROGMEM = "Press Stop to reset";
const char editingPad_string[] PROGMEM = "Editing pad ";
const char aftertouchClear_string[] PROGMEM = "     ";
const char midiChannelClear_string[] PROGMEM = "    ";
const char midiChannel_string[] PROGMEM = "ch";
const char dfu_string[] PROGMEM = "Firmware update mode";

#endif