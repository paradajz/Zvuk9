#ifndef PADSCALIBRATION_H_
#define PADSCALIBRATION_H_

#include "hardware/pins/Pins.h"

//don't check velocity for PAD_PRESS_DEBOUNCE_TIMEms after pad has just been pressed
#define PAD_PRESS_DEBOUNCE_TIME             10
//pad is considered released PAD_RELEASE_DEBOUNCE_TIMEms after releasing pad
#define PAD_RELEASE_DEBOUNCE_TIME           15

//send x/y and then notes after this delay to reduce glide effect
#define PAD_NOTE_SEND_DELAY                 20

//lower aftertouch scale = upper pressure scale + AFTERTOUCH_PRESSURE_RATIO_LOWER%
#define AFTERTOUCH_PRESSURE_RATIO_LOWER     30

//upper aftertouch scale = upper pressure scale + AFTERTOUCH_PRESSURE_RATIO_UPPER%
#define AFTERTOUCH_PRESSURE_RATIO_UPPER     100
//change aftertouch step difference after this timeout
#define AFTERTOUCH_SEND_TIMEOUT             100
//ignore aftertouch reading change below this timeout
#define AFTERTOUCH_SEND_TIMEOUT_IGNORE      3
//threshold difference between current and last aftertouch value after AFTERTOUCH_SEND_TIMEOUT of no movement
#define AFTERTOUCH_SEND_TIMEOUT_STEP        2
//don't check aftertouch when pad is just pressed for AFTERTOUCH_INITIAL_VALUE_DELAYms
#define AFTERTOUCH_INITIAL_VALUE_DELAY      100

//change x/y step difference after this timeout
#define XY_SEND_TIMEOUT                     30
//ignore x/y readings change below this timeout
#define XY_SEND_TIMEOUT_IGNORE              10
//threshold difference between current and last x/y value after XY_SEND_TIMEOUT of no movement
#define XY_SEND_TIMEOUT_STEP                3

#ifdef VARIANT_2
#define XY_FLIP_AXIS                        1
#define XY_FLIP_VALUES                      0
#else
#define XY_FLIP_AXIS                        0
#define XY_FLIP_VALUES                      0
#endif

#endif