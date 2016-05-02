#include "MIDI.h"
#include "MIDI_parameters.h"
#include "../hardware/midi/MIDI.h"
#include "../Debug.h"
#include "../interface/pads/Pads.h"

MIDI::MIDI()    {

    //default constructor

}

void MIDI::init() {

    //enable only midi out
    //listen only on channel 1
    hwMIDI.init(false, true, dinInterface);
    hwMIDI.init(false, true, usbInterface);
    hwMIDI.setInputChannel(DEFAULT_MIDI_CHANNEL);

}

void MIDI::sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)  {

    #if MODE_SERIAL < 1
        hwMIDI.sendNoteOn(note, velocity, channel, dinInterface);
        hwMIDI.sendNoteOn(note, velocity, channel, usbInterface);
    #endif

}

void MIDI::sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)  {

    #if MODE_SERIAL < 1
        hwMIDI.sendNoteOff(note, velocity, channel, dinInterface);
        hwMIDI.sendNoteOff(note, velocity, channel, usbInterface);
    #endif

}

void MIDI::sendControlChange(uint8_t channel, uint8_t ccNumber, uint8_t ccValue) {

    #if MODE_SERIAL < 1
        hwMIDI.sendControlChange(ccNumber, ccValue, channel, dinInterface);
        hwMIDI.sendControlChange(ccNumber, ccValue, channel, usbInterface);
    #endif

}

void MIDI::sendChannelAftertouch(uint8_t channel, uint8_t pressure)   {

    #if MODE_SERIAL < 1
        hwMIDI.sendAfterTouch(pressure, channel, dinInterface);
        hwMIDI.sendAfterTouch(pressure, channel, usbInterface);
    #endif

}

void MIDI::sendKeyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure)   {

    #if MODE_SERIAL < 1
        hwMIDI.sendPolyPressure(note, pressure, channel, dinInterface);
        hwMIDI.sendPolyPressure(note, pressure, channel, usbInterface);
    #endif

}

void MIDI::sendSysEx(uint8_t *sysExArray, uint8_t arraySize)   {

    #if MODE_SERIAL < 1
        hwMIDI.sendSysEx(arraySize, sysExArray, true, dinInterface);
        hwMIDI.sendSysEx(arraySize, sysExArray, true, usbInterface);
    #endif

}

MIDI midi;
