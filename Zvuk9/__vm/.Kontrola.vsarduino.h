/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy++ 2.0, Platform=avr, Package=teensy
*/

#define __AVR_AT90usb1286__
#define __AVR_AT90USB1286__
#define ARDUINO 106
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define USB_MIDI
#define LAYOUT_US_ENGLISH
#define TEENSYDUINO 120
extern "C" void __cxa_pure_virtual() {;}

void padsFunctionOnOff(uint8_t buttonNumber);
void padsOctaveUpDown(uint8_t direction, bool buttonState);
void padsChangeTonic(uint8_t tonic);
void setLCDprogram(uint8_t program);
void setLCDpreset(uint8_t preset);
void setLCDpadData(uint8_t note, uint8_t velocity, uint8_t aftertouch);
void setLCDxyData(uint8_t x, uint8_t y, uint8_t xEnabled, uint8_t yEnabled, uint8_t xChanged, uint8_t yChanged);
void clearPadData();
void setLEDTonicState(uint8_t tonic, ledIntensity tonicLEDstate);
void handleEncoder(uint8_t encoderNumber, bool direction);
void setLEDstate(uint8_t ledNumber, ledIntensity state);
void sendMIDIcc(uint8_t channel, uint8_t ccNumber, uint8_t ccValue);
void sendMIDInote(uint8_t channel, uint8_t noteNumber, uint8_t velocity, bool noteOn);
void sendMIDIafterTouch(uint8_t channel, uint8_t noteNumber, uint8_t pressure);
void sendMIDIsysEx(uint8_t arraySize, uint8_t *sysExArray);
void configurePadCallbacks();
void configureButtonCallbacks();
void configureEncoderCallbacks();
void configureCallbacks();
void configureHardwareMIDI();
void initHardware();
//
//

#include "D:\Applications\Arduino\hardware\teensy\cores\teensy\arduino.h"
#include <Kontrola.ino>
#include <Buttons.cpp>
#include <Buttons.h>
#include <Debug.h>
#include <Defs.h>
#include <EEPROMsettings.cpp>
#include <EEPROMsettings.h>
#include <EncoderSettings.h>
#include <Encoders.cpp>
#include <Encoders.h>
#include <HardwareIDs.h>
#include <KontrolaBreadBoardIDs.h>
#include <KontrolaPCBIDs.h>
#include <LCD.cpp>
#include <LCD.h>
#include <LCDsettings.h>
#include <LCDstrings.h>
#include <LEDs.cpp>
#include <LEDs.h>
#include <LEDsettings.h>
#include <MIDI_parameters.h>
#include <Modules.h>
#include <PadXYscales.h>
#include <Pads.cpp>
#include <Pads.h>
#include <PadsCalibration.h>
#include <PadsCallbacks.cpp>
#include <PadsHardwareControl.cpp>
#include <PadsHardwareRead.cpp>
#include <Types.h>
