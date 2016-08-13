#include "../Debug.h"

#if MODE_SERIAL < 1

#ifndef MIDI_H_
#define MIDI_H_

#include <avr/io.h>
#include "../Types.h"
#include "../eeprom/Configuration.h"

class MIDI {

    public:
    MIDI();
    void init();

    void checkInput();
    void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void sendControlChange(uint8_t channel, uint8_t ccNumber, uint8_t value);
    void sendChannelAftertouch(uint8_t channel, uint8_t pressure);
    void sendKeyAftertouch(uint8_t channel, uint8_t note, uint8_t pressure);
    void sendSysEx(uint8_t *sysExArray, uint8_t size, bool arrayContainsBoundaries);

    bool runningStatusEnabled();
    noteOffType_t getNoteOffStatus();
    void setNoteOffStatus(noteOffType_t type);
    void setRunningStatus(bool option);

    private:
    noteOffType_t       noteOffType;
    uint32_t            lastSysExMessageTime;

};

extern MIDI midi;

#endif
#endif