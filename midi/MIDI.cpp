#include "MIDI.h"
#include "MIDI_parameters.h"
#include "hw_midi/HwMIDI.h"
#include "../Debug.h"

MIDI::MIDI()    {

    //default constructor

}

void MIDI::init() {

    hwMIDI.begin(DEFAULT_MIDI_CHANNEL);
    //usb midi started automatically

}

void MIDI::sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)  {

    #if MODE_SERIAL < 1
    usbMIDI.sendNoteOn(note, velocity, channel);
    hwMIDI.sendNoteOn(note, velocity, channel);
    #endif

}

void MIDI::sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)  {

    #if MODE_SERIAL < 1
    usbMIDI.sendNoteOff(note, velocity, channel);
    hwMIDI.sendNoteOff(note, velocity, channel);
    #endif

}

void MIDI::sendControlChange(uint8_t channel, uint8_t ccNumber, uint8_t ccValue) {

    #if MODE_SERIAL < 1
    usbMIDI.sendControlChange(ccNumber, ccValue, channel);
    hwMIDI.sendControlChange(ccNumber, ccValue, channel);
    #endif

}

void MIDI::sendAfterTouch(uint8_t channel, uint8_t noteNumber, uint8_t pressure)   {

    #if MODE_SERIAL < 1
    hwMIDI.sendAfterTouch(pressure, channel);
    usbMIDI.sendAfterTouch(pressure, channel);
    #endif

}

void MIDI::sendSysEx(uint8_t *sysExArray, uint8_t arraySize)   {

    #if MODE_SERIAL < 1
    hwMIDI.sendSysEx(arraySize, sysExArray, true);
    usbMIDI.sendSysEx(arraySize, sysExArray);
    #endif

}

MIDI midi;