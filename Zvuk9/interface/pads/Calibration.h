#ifndef PADSCALIBRATION_H_
#define PADSCALIBRATION_H_

#include "hardware/pins/Pins.h"

#define PAD_PRESS_DEBOUNCE_TIME             10
#define PAD_RELEASE_DEBOUNCE_TIME           15

//send x/y and then notes after this delay
#define PAD_NOTE_SEND_DELAY                 20

#define DEFAULT_AFTERTOUCH_PRESSURE_RATIO   100

#define AFTERTOUCH_SEND_TIMEOUT             100
#define XY_SEND_TIMEOUT                     25

#define XY_SEND_TIMEOUT_IGNORE              5       //ignore xy reading change below this timeout
#define AFTERTOUCH_SEND_TIMEOUT_IGNORE      3       //ignore aftertouch reading change below this timeout

#define XY_SEND_TIMEOUT_STEP                3
#define AFTERTOUCH_SEND_TIMEOUT_STEP        2

#define AFTERTOUCH_SEND_AFTER_DIFFERENCE    10      //0-127 //difference initial pressure needs to exceed to send aftertouch

#define XY_CHANGE_AFTERTOUCH_RESET          5       //if x/y change by this value, reset aftertouch gesture counter

#ifdef VARIANT_2
#define XY_FLIP_AXIS                        1
#define XY_FLIP_VALUES                      0
#else
#define XY_FLIP_AXIS                        0
#define XY_FLIP_VALUES                      0
#endif

#endif