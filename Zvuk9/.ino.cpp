//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2016-03-11 01:02:46

#include "Arduino.h"
#include "interface/lcd/LCD.h"
#include "interface/pads/Pads.h"
#include "interface/buttons/Buttons.h"
#include "Debug.h"
#include "eeprom/EEPROMsettings.h"
#include "interface/leds/LEDs.h"
#include "interface/encoders/Encoders.h"
#include "hardware/pins/Pins.h"
#include "Types.h"
#include "hardware/timer/TimerObject.h"
#include "hardware/reset/Reset.h"
#include <util/delay.h>
#include "midi/MIDI.h"
void startUpAnimation() ;
void setTonicLEDs() ;
void handleTransportControl(uint8_t buttonNumber, buttonState_t state)  ;
bool checkOctaveUpDownEnabled() ;
void setLEDTonicStateEditMode(uint8_t pad) ;
void padsFunctionOnOff(uint8_t buttonNumber, buttonState_t state)    ;
void padsOctaveUpDown(uint8_t direction, bool buttonState)    ;
void handleTonicPress(tonic_t _tonic) ;
void displayActivePadNotes(uint8_t pad) ;
void setUpPadEditMode(uint8_t pad) ;
void clearPadEditMode() ;
void handlePadPress(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY)  ;
void handlePadRelease(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes)  ;
void setLCDAfterTouch(uint8_t pressure) ;
void setLCDxyData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable) ;
void clearPadData(uint8_t pad) ;
void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps)   ;
void setLEDstate(uint8_t ledNumber, ledIntensity_t state)   ;
void configurePadCallbacks()    ;
void configureButtonCallbacks() ;
void configureEncoderCallbacks()    ;
void configureCallbacks()   ;
void initHardware() ;
void setup()    ;
void loop()     ;

#include "Zvuk9.ino"

