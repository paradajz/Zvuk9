#include "../Debug.h"

#if MODE_SERIAL < 1

#ifndef MIDI_H_
#define MIDI_H_

#include <avr/io.h>

class MIDI {

    public:
    MIDI();
    void init();

    void sendNoteOn(uint8_t& channel, uint8_t& note, uint8_t& velocity);
    void sendNoteOff(uint8_t& channel, uint8_t& note, uint8_t& velocity);
    void sendControlChange(uint8_t& channel, uint8_t& ccNumber, uint8_t& value);
    void sendChannelAftertouch(uint8_t& channel, uint8_t& pressure);
    void sendKeyAftertouch(uint8_t& channel, uint8_t& note, uint8_t& pressure);
    void sendSysEx(uint8_t *sysExArray, uint8_t size);

    private:

};

extern MIDI midi;

#endif
#endif