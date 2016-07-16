#include "MIDI.h"

#ifdef MIDI_H_

#include "MIDI_parameters.h"
#include "../hardware/midi/MIDI.h"
#include "../interface/pads/Pads.h"

MIDI::MIDI()    {

    //default constructor
    noteOffType = noteOffType_standardNoteOff;

}

void MIDI::init() {

    //enable only midi out
    //listen only on channel 1
    hwMIDI.init(false, true, dinInterface);
    hwMIDI.init(false, true, usbInterface);
    hwMIDI.setInputChannel(DEFAULT_MIDI_CHANNEL);

}

void MIDI::sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)  {

    hwMIDI.sendNoteOn(note, velocity, channel, dinInterface);
    hwMIDI.sendNoteOn(note, velocity, channel, usbInterface);

}

void MIDI::sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)  {

    if (noteOffType == noteOffType_standardNoteOff)    {

        hwMIDI.sendNoteOff(note, velocity, channel, dinInterface);
        hwMIDI.sendNoteOff(note, velocity, channel, usbInterface);

    }   else {

        sendNoteOn(channel, note, velocity);

    }

}

void MIDI::sendControlChange(uint8_t channel, uint8_t ccNumber, uint8_t ccValue) {

    hwMIDI.sendControlChange(ccNumber, ccValue, channel, dinInterface);
    hwMIDI.sendControlChange(ccNumber, ccValue, channel, usbInterface);

}

void MIDI::sendChannelAftertouch(uint8_t channel, uint8_t pressure)   {

    hwMIDI.sendAfterTouch(pressure, channel, dinInterface);
    hwMIDI.sendAfterTouch(pressure, channel, usbInterface);

}

void MIDI::sendKeyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure)   {

    hwMIDI.sendPolyPressure(note, pressure, channel, dinInterface);
    hwMIDI.sendPolyPressure(note, pressure, channel, usbInterface);

}

void MIDI::sendSysEx(uint8_t *sysExArray, uint8_t arraySize)   {

    hwMIDI.sendSysEx(arraySize, sysExArray, true, dinInterface);
    hwMIDI.sendSysEx(arraySize, sysExArray, true, usbInterface);

}

noteOffType_t MIDI::noteOffStatus() {

    return noteOffType;

}

void MIDI::setNoteOffStatus(noteOffType_t type) {

    noteOffType = type;

}

bool MIDI::runningStatusEnabled()   {

    return hwMIDI.runningStatusEnabled();

}

void MIDI::setRunningStatus(bool option)    {

    option ? hwMIDI.enableRunningStatus() : hwMIDI.disableRunningStatus();

}

MIDI midi;
#endif