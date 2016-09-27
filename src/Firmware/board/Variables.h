#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "Constants.h"

//LEDs
extern volatile int8_t      activeColumnInterrupt;
extern uint8_t              ledState[NUMBER_OF_LEDS];
extern int16_t              transitionCounter[NUMBER_OF_LEDS];
extern volatile uint8_t     pwmSteps;
extern bool                 blinkEnabled;
extern uint16_t             ledBlinkTimer[NUMBER_OF_LEDS];

//encoders
extern volatile int8_t      encoderMoving[NUMBER_OF_ENCODERS];
extern uint16_t             encoderData[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort1Array[NUMBER_OF_ENCODERS];
extern volatile uint8_t     *encoderPort2Array[NUMBER_OF_ENCODERS];

//millis
extern volatile uint32_t    rTime_ms;

#endif