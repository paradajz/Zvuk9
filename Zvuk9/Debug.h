#ifndef DEBUG_H_
#define DEBUG_H_

#include "hardware\pins\HardwareIDs.h"

//automatically disable serial mode on PCBs
#ifdef KONTROLA_PCB
#define MODE_SERIAL 0 //set to 1 to send data to serial monitor
#else

#define MODE_SERIAL 1
#endif


#endif /* DEBUG_H_ */ 
