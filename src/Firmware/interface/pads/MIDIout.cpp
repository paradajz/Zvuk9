#include "Pads.h"
#include "../leds/LEDs.h"
#include "../lcd/LCD.h"
#include "../../database/Database.h"
#ifdef NDEBUG
#include "../../midi/src/MIDI.h"
#endif

///
/// \ingroup interfacePads
/// @{

///
/// \brief Sends MIDI CC message on X coordinate for requested pad.
/// @param [in] pad     Pad for which MIDI CC value on X coordinate is being sent.
///
void Pads::sendX(int8_t pad)
{
    assert(PAD_CHECK(pad));

    #ifdef DEBUG
    printf_P(PSTR("X for pad %d: "), pad);
    printf_P(PSTR("%d\n"), lastXMIDIvalue[pad]);
    printf_P(PSTR("X CC: %d\n"), ccXPad[pad]);
    #else
    midi.sendControlChange(ccXPad[pad], lastXMIDIvalue[pad], midiChannel[pad]);
    #endif
}

///
/// \brief Sends MIDI CC message on Y coordinate for requested pad.
/// @param [in] pad     Pad for which MIDI CC value on Y coordinate is being sent.
///
void Pads::sendY(int8_t pad)
{
    assert(PAD_CHECK(pad));

    #ifdef DEBUG
    printf_P(PSTR("Y for pad %d: "), pad);
    printf_P(PSTR("%d\n"), lastYMIDIvalue[pad]);
    printf_P(PSTR("Y CC: %d\n"), ccYPad[pad]);
    #else
    midi.sendControlChange(ccYPad[pad], lastYMIDIvalue[pad], midiChannel[pad]);
    #endif
}

///
/// \brief Sends MIDI notes for requested pad.
/// @param [in] pad         Pad for which MIDI notes are being sent.
/// @param [in] velocity    MIDI velocity value for sent pad notes.
/// @param [in] state       State of MIDI notes (true/on, false/off).
///
void Pads::sendNotes(int8_t pad, uint8_t velocity, bool state)
{
    assert(PAD_CHECK(pad));

    bool sendOff;

    switch(state)
    {
        case true:
        //note on
        #ifdef DEBUG
        printf_P(PSTR("Pad %d pressed. Notes:\n"), pad);
        #endif

        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] == BLANK_NOTE)
                continue;

            #ifdef DEBUG
            printf_P(PSTR("%d\n"), padNote[pad][i]);
            #else
            midi.sendNoteOn(padNote[pad][i], velocity, midiChannel[pad]);
            #endif
        }

        #ifdef DEBUG
        printf_P(PSTR("Velocity: %d\n"), velocity);
        #endif
        break;

        case false:
        //note off
        #ifdef DEBUG
        printf_P(PSTR("Pad %d released. Notes: \n"), pad);
        #endif
        //some special considerations here
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            sendOff = true;

            if (padNote[pad][i] == BLANK_NOTE)
                continue;

            for (int j=0; j<NUMBER_OF_PADS; j++)
            {
                //don't check current pad
                if (j == pad)
                    continue;

                //don't check released pads
                if (!isPadPressed(j))
                    continue;

                //don't check pad if noteSend is disabled
                if (!bitRead(noteSendEnabled, j))
                    continue;

                //only send note off if the same note isn't active on some other pad already
                if (padNote[j][i] == padNote[pad][i])
                {
                    //extra check here - check if midi channels differ
                    if (midiChannel[j] == midiChannel[pad])
                    {
                        sendOff = false;
                        break; //no need to check further
                    }
                }
            }

            if (sendOff)
            {
                #ifdef DEBUG
                printf_P(PSTR("%d\n"), padNote[pad][i]);
                #else
                uint8_t velocity_ = 0;
                midi.sendNoteOff(padNote[pad][i], velocity_, midiChannel[pad]);
                #endif
            }
        }
        break;
    }

    updateNoteLEDs(pad, state);
}

///
/// \brief Sends MIDI aftertouch for requested pad.
/// @param [in] pad     Pad for which MIDI aftertouch is being sent.
///
void Pads::sendAftertouch(int8_t pad)
{
    assert(PAD_CHECK(pad));

    #ifdef NDEBUG
    uint8_t aftertouchValue = bitRead(lastMIDInoteState, pad) ? lastAftertouchValue[pad] : 0;
    #endif

    switch(aftertouchType)
    {
        case aftertouchPoly:
        #ifdef DEBUG
        printf_P(PSTR("Sending key aftertouch, pad %d: %d\n"), pad, lastAftertouchValue[pad]);
        #else
        for (int i=0; i<NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] != BLANK_NOTE)
                midi.sendPolyPressure(padNote[pad][i], aftertouchValue, midiChannel[pad]);
        }
        #endif
        break;

        case aftertouchChannel:
        #ifdef DEBUG
        printf_P(PSTR("Sending channel aftertouch: %d\n"), maxAftertouchValue);
        #else
        midi.sendAfterTouch(maxAftertouchValue, midiChannel[pad]);
        #endif
        break;
    }

    if (!bitRead(lastMIDInoteState, pad))
        bitWrite(aftertouchActivated, pad, false);
}

/// @}