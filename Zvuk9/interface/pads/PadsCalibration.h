#ifndef PADSCALIBRATION_H_
#define PADSCALIBRATION_H_

#define XY_INVERT                           0

#define PAD_PRESS_DEBOUNCE_TIME             3

#define PAD_RELEASE_DEBOUNCE_TIME           10

//start reading X/Y PAD_X_Y_DEBOUNCE_TIMEms after pressure
#define PAD_X_Y_DEBOUNCE_TIME               20

//send x/y and then notes after this delay
#define PAD_NOTE_SEND_DELAY                 20

#define GESTURE_ACTIVATION_TIME             2000    //activation time in ms

#define DEFAULT_AFTERTOUCH_PRESSURE_RATIO   100

#define GESTURE_NUMBER_OF_CHANGES           4

#define AFTERTOUCH_SEND_TIMEOUT             100
#define XY_SEND_TIMEOUT                     25

#define XY_SEND_TIMEOUT_IGNORE              5       //ignore xy reading change below this timeout
#define AFTERTOUCH_SEND_TIMEOUT_IGNORE      3       //ignore aftertouch reading change below this timeout

#define XY_SEND_TIMEOUT_STEP                3
#define AFTERTOUCH_SEND_TIMEOUT_STEP        2

#define AFTERTOUCH_SEND_AFTER_DIFFERENCE    10      //0-127 //difference initial pressure needs to exceed to send aftertouch

#define XY_CHANGE_AFTERTOUCH_RESET          5       //if x/y change by this value, reset aftertouch gesture counter

#endif