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
    printf_P(PSTR("%d\n"), lastXCCvalue[pad]);
    printf_P(PSTR("X CC: %d\n"), ccXPad[pad]);
    #else
    if (getPitchBendState(pad, coordinateX))
        midi.sendPitchBend(lastXPitchBendValue[pad], midiChannel[pad]);
    else
        midi.sendControlChange(ccXPad[pad], lastXCCvalue[pad], midiChannel[pad]);
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
    printf_P(PSTR("%d\n"), lastYCCvalue[pad]);
    printf_P(PSTR("Y CC: %d\n"), ccYPad[pad]);
    #else
    if (getPitchBendState(pad, coordinateY))
        midi.sendPitchBend(lastYPitchBendValue[pad], midiChannel[pad]);
    else
        midi.sendControlChange(ccYPad[pad], lastYCCvalue[pad], midiChannel[pad]);
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
        //some special considerations here - don't send note off if same note is active on some other pad
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
                #endif

                #ifdef USE_USB_MIDI
                uint8_t velocity_ = 0;
                midi.sendNoteOff(padNote[pad][i], velocity_, midiChannel[pad]);
                #endif
            }
        }

        //now perform same check for pitch bend if pitch bend is active on current pad
        if ((getMIDISendState(pad, functionXPitchBend) && getPitchBendState(pad, coordinateX)) || (getMIDISendState(pad, functionYPitchBend) && getPitchBendState(pad, coordinateY)))
        {
            sendOff = true;

            for (int i=0; i<NUMBER_OF_PADS; i++)
            {
                //don't check current pad
                if (i == pad)
                continue;

                //don't check released pads
                if (!isPadPressed(i))
                    continue;

                //check both coordinates but don't check the pad if pitch bend isn't active on any coordinate
                if (!(getPitchBendState(i, coordinateX) && getMIDISendState(i, functionXPitchBend)) && !(getPitchBendState(i, coordinateY) && getMIDISendState(i, functionYPitchBend)))
                continue;

                //by this point, we have found pad with active pitch bend on x or y coordinates (or both)
                //if midi channels are the same, don't send pitch bend 0
                if (getMIDIchannel(i) == getMIDIchannel(pad))
                {
                    sendOff = false;
                    break;
                }
            }

            if (sendOff)
            {
                #ifdef DEBUG
                printf_P(PSTR("Sending pitch bend 0 for current pad.\n"));
                #endif

                #ifdef USE_USB_MIDI
                midi.sendPitchBend(0, midiChannel[pad]);
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