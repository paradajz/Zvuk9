#pragma once

#define PAD_DEBOUNCE_TIME                           15

//send x/y and then notes after this delay to reduce glide effect
#define PAD_NOTE_SEND_DELAY                         5

//lower aftertouch scale = upper pressure scale + AFTERTOUCH_PRESSURE_RATIO_LOWER%
#define AFTERTOUCH_PRESSURE_RATIO_LOWER             15
//upper aftertouch scale = lower aftertouch scale + AFTERTOUCH_PRESSURE_RATIO_UPPER%
#define AFTERTOUCH_PRESSURE_RATIO_UPPER_LOW         25
#define AFTERTOUCH_PRESSURE_RATIO_UPPER_MEDIUM      20
#define AFTERTOUCH_PRESSURE_RATIO_UPPER_HARD        10
//change aftertouch step difference after this timeout
#define AFTERTOUCH_SEND_TIMEOUT                     100
//threshold difference between current and last aftertouch value after AFTERTOUCH_SEND_TIMEOUT of no movement
#define AFTERTOUCH_SEND_TIMEOUT_STEP                3

//change x/y step difference after this timeout
#define XY_SEND_TIMEOUT                             35
//threshold difference between current and last x/y value after XY_SEND_TIMEOUT of no movement
#define XY_SEND_TIMEOUT_STEP                        3

//when doing interactive calibration, scrolling from left to right isn't 100% accurate
//compensate using offset
#define X_MIN_CALIBRATION_OFFSET                    10
#define X_MAX_CALIBRATION_OFFSET                    -10

//there are three different pressure levels - low, medium, hard
//medium pressure increase is low level +30%
//hard pressure increase is low level +50%
//only for velocity
#define MEDIUM_VELOCITY_INCREASE_PERCENT            30
#define HARD_VELOCITY_INCREASE_PERCENT              50