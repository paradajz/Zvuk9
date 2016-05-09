#ifndef ERRORS_STRINGS_H_
#define ERRORS_STRINGS_H_

#include <avr/pgmspace.h>

const char outOfRange_string[] PROGMEM = "Out of range";

const char editModeEnterError0_string[] PROGMEM = "Select user preset";
const char editModeEnterError1_string[] PROGMEM = "to edit pad notes";

const char relasePad_string[] PROGMEM = "Release pad to";
const char changeParameters_string[] PROGMEM = "change parameters";
const char changeProgram_string[] PROGMEM = "change program";
const char changePreset_string[] PROGMEM = "change preset";
const char changeCurve_string[] PROGMEM = "change curve";
const char changeCC_string[] PROGMEM = "change CC";
const char changeCClimit_string[] PROGMEM = "change CC limit";
const char enterPadEditMode_string[] PROGMEM = "enter pad edit mode";

const char exitPadMode_string[] PROGMEM = "Exit pad mode to";

const char maxNotesSet0_string[] PROGMEM = "Maximum number of";
const char maxNotesSet1_string[] PROGMEM = "notes set";

#endif