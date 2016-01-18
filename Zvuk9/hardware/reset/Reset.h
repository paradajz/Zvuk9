#ifndef RESET_H_
#define RESET_H_

#include "Arduino.h"
#include "hardware/timer/TimerObject.h"

bool checkReset(bool buttonState);
void resetTeensy();


#endif /* RESET_H_ */