/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy++ 2.0, Platform=teensy, Package=teensy
*/

#define __HARDWARE_AT90usb1286__
#define __HARDWARE_AT90USB1286__
#define ARDUINO 167
#define ARDUINO_MAIN
#define printf iprintf
#define __TEENSY__
#define TEENSYDUINO 127
#define ARDUINO 167
#define F_CPU 16000000L
#define ARDUINO_ARCH_AVR
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
extern "C" void __cxa_pure_virtual() {;}

void startUpAnimation();
void setTonicLEDs();
void handleTransportControl(uint8_t buttonNumber, buttonState_t state);
bool checkOctaveUpDownEnabled();
void setLEDTonicStateEditMode(uint8_t pad);
void padsFunctionOnOff(uint8_t buttonNumber, buttonState_t state);
void padsOctaveUpDown(uint8_t direction, bool buttonState);
void handleTonicPress(tonic_t _tonic);
void displayActivePadNotes(uint8_t pad);
void setUpPadEditMode(uint8_t pad);
void clearPadEditMode();
void handlePadPress(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY);
void handlePadRelease(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes);
void setLCDAfterTouch(uint8_t pressure);
void setLCDxyData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
void clearPadData(uint8_t pad);
void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps);
void setLEDstate(uint8_t ledNumber, ledIntensity_t state);
void configurePadCallbacks();
void configureButtonCallbacks();
void configureEncoderCallbacks();
void configureCallbacks();
void initHardware();
//
//

#include "D:\Applications\Arduino167_zvuk9_mod\hardware\teensy\avr\cores\teensy\arduino.h"
#include <..\Zvuk9\Zvuk9.ino>
#include <interface\pads\Aftertouch.cpp>
#include <eeprom\Blocks.h>
#include <interface\buttons\Buttons.cpp>
#include <interface\buttons\Buttons.h>
#include <..\Zvuk9\Debug.h>
#include <eeprom\Defaults.h>
#include <eeprom\EEPROMsettings.cpp>
#include <eeprom\EEPROMsettings.h>
#include <interface\encoders\EncoderSettings.h>
#include <interface\encoders\Encoders.cpp>
#include <interface\encoders\Encoders.h>
#include <midi\hw_midi\HwMIDI.cpp>
#include <midi\hw_midi\HwMIDI.h>
#include <interface\lcd\LCD.cpp>
#include <interface\lcd\LCD.h>
#include <interface\lcd\LCDsettings.h>
#include <interface\lcd\LCDstrings.h>
#include <interface\leds\LEDs.cpp>
#include <interface\leds\LEDs.h>
#include <interface\leds\LEDsettings.h>
#include <midi\MIDI.cpp>
#include <midi\MIDI.h>
#include <midi\MIDI_parameters.h>
#include <interface\pads\PadXYscales.h>
#include <interface\pads\Pads.cpp>
#include <interface\pads\Pads.h>
#include <interface\pads\PadsCalibration.h>
#include <interface\pads\PadsCallbacks.cpp>
#include <interface\pads\PadsConfigurationGet.cpp>
#include <interface\pads\PadsConfigurationSet.cpp>
#include <interface\pads\PadsHardwareControl.cpp>
#include <interface\pads\PadsHardwareRead.cpp>
#include <hardware\pins\PinManipulation.h>
#include <hardware\pins\Pins.h>
#include <hardware\reset\Reset.cpp>
#include <hardware\reset\Reset.h>
#include <..\Zvuk9\Scales.cpp>
#include <..\Zvuk9\Scales.h>
#include <eeprom\Sections.h>
#include <hardware\timer\TimerObject.cpp>
#include <hardware\timer\TimerObject.h>
#include <..\Zvuk9\Types.h>
#include <hardware\uart\UART.cpp>
#include <hardware\uart\UART.h>
#include <eeprom\UniqueID.h>
#include <..\Zvuk9\Version.h>
#include <hardware\pins\board_v1.h>
#include <hardware\lcd\hd44780.cpp>
#include <hardware\lcd\hd44780.h>
#include <hardware\lcd\hd44780_settings.h>
