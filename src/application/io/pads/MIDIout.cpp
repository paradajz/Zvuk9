/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#include <assert.h>
#include "Pads.h"
#include "core/src/general/BitManipulation.h"

/// \ingroup interfacePads
/// @{

/// Sends MIDI CC or Pitch Bend message on X coordinate for requested pad.
/// @param [in] pad     Pad for which MIDI CC value on X coordinate is being sent.
void Pads::sendX(int8_t pad)
{
    assert(PAD_CHECK(pad));

    if (getPitchBendState(pad, coordinateX))
    {
        midi.sendPitchBend(lastXPitchBendValue[pad], midiChannel[pad]);
#ifdef DEBUG
        printf_P(PSTR("X for pad %d: %d\n"), pad, lastXPitchBendValue[pad]);
#endif
    }
    else
    {
        midi.sendControlChange(ccXPad[pad], lastXCCvalue[pad], midiChannel[pad]);
#ifdef DEBUG
        printf_P(PSTR("X for pad %d: %d, CC %d\n"), pad, lastXCCvalue[pad], ccXPad[pad]);
#endif
    }
}

/// Sends MIDI CC or Pitch Bend message on Y coordinate for requested pad.
/// @param [in] pad     Pad for which MIDI CC value on Y coordinate is being sent.
void Pads::sendY(int8_t pad)
{
    assert(PAD_CHECK(pad));

    if (getPitchBendState(pad, coordinateY))
    {
        midi.sendPitchBend(lastYPitchBendValue[pad], midiChannel[pad]);
#ifdef DEBUG
        printf_P(PSTR("Y for pad %d: %d\n"), pad, lastYPitchBendValue[pad]);
#endif
    }
    else
    {
        midi.sendControlChange(ccYPad[pad], lastYCCvalue[pad], midiChannel[pad]);
#ifdef DEBUG
        printf_P(PSTR("Y for pad %d: %d, CC %d\n"), pad, lastYCCvalue[pad], ccYPad[pad]);
#endif
    }
}

/// Sends MIDI notes (or Pitch Bend 0 on release) for requested pad.
/// @param [in] pad         Pad for which MIDI notes or PB0 are being sent.
/// @param [in] velocity    MIDI velocity value for sent pad notes.
/// @param [in] state       State of MIDI notes (true/on, false/off+PB0).
void Pads::sendNotes(int8_t pad, uint8_t velocity, bool state)
{
    assert(PAD_CHECK(pad));

    bool sendOff;

    switch (state)
    {
    case true:
        //note on
        if (!getMIDISendState(pad, functionOnOffNotes))
            return;
#ifdef DEBUG
        printf_P(PSTR("Pad %d pressed. Notes:\n"), pad);
#endif

        for (int i = 0; i < NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] == BLANK_NOTE)
                continue;

#ifdef DEBUG
            printf_P(PSTR("%d\n"), padNote[pad][i]);
#endif

            midi.sendNoteOn(padNote[pad][i], velocity, midiChannel[pad]);
        }

#ifdef DEBUG
        printf_P(PSTR("Velocity: %d\n"), velocity);
#endif
        break;

    case false:
        //note off
        if (getMIDISendState(pad, functionOnOffNotes))
        {
#ifdef DEBUG
            printf_P(PSTR("Pad %d released. Notes: \n"), pad);
#endif
            //some special considerations here - don't send note off if same note is active on some other pad
            for (int i = 0; i < NOTES_PER_PAD; i++)
            {
                sendOff = true;

                if (padNote[pad][i] == BLANK_NOTE)
                    continue;

                for (int j = 0; j < NUMBER_OF_PADS; j++)
                {
                    //don't check current pad
                    if (j == pad)
                        continue;

                    //don't check released pads
                    if (!isPadPressed(j))
                        continue;

                    //don't check pad if noteSend is disabled
                    if (!BIT_READ(noteSendEnabled, j))
                        continue;

                    //only send note off if the same note isn't active on some other pad already
                    if (padNote[j][i] == padNote[pad][i])
                    {
                        //extra check here - check if midi channels differ
                        if (midiChannel[j] == midiChannel[pad])
                        {
                            sendOff = false;
                            break;    //no need to check further
                        }
                    }
                }

                if (sendOff)
                {
#ifdef DEBUG
                    printf_P(PSTR("%d\n"), padNote[pad][i]);
#endif

                    uint8_t velocity_ = 0;
                    midi.sendNoteOff(padNote[pad][i], velocity_, midiChannel[pad]);
                }
            }
        }

        //now perform same check for pitch bend if pitch bend is active on current pad
        if (getMIDISendState(pad, functionXPitchBend) || getMIDISendState(pad, functionYPitchBend))
        {
            sendOff = true;

            for (int i = 0; i < NUMBER_OF_PADS; i++)
            {
                //don't check current pad
                if (i == pad)
                    continue;

                //don't check released pads
                if (!isPadPressed(i))
                    continue;

                //check both coordinates but don't check the pad if pitch bend isn't active on any coordinate
                if (!(getMIDISendState(i, functionXPitchBend) || getMIDISendState(i, functionYPitchBend)))
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

                midi.sendPitchBend(0, midiChannel[pad]);
            }
        }
        break;
    }

    updateNoteLEDs(pad, state);
}

/// Sends MIDI aftertouch for requested pad.
/// @param [in] pad     Pad for which MIDI aftertouch is being sent.
void Pads::sendAftertouch(int8_t pad)
{
    assert(PAD_CHECK(pad));

    uint8_t aftertouchValue = BIT_READ(lastMIDInoteState, pad) ? lastAftertouchValue[pad] : 0;

    switch (aftertouchType)
    {
    case aftertouchPoly:
#ifdef DEBUG
        printf_P(PSTR("Sending key aftertouch, pad %d: %d\n"), pad, lastAftertouchValue[pad]);
#endif

        for (int i = 0; i < NOTES_PER_PAD; i++)
        {
            if (padNote[pad][i] != BLANK_NOTE)
                midi.sendAfterTouch(padNote[pad][i], aftertouchValue, midiChannel[pad]);
        }
        break;

    case aftertouchChannel:
#ifdef DEBUG
        printf_P(PSTR("Sending channel aftertouch: %d\n"), maxAftertouchValue);
#endif

        midi.sendAfterTouch(maxAftertouchValue, midiChannel[pad]);
        break;
    }

    if (!BIT_READ(lastMIDInoteState, pad))
        BIT_WRITE(aftertouchActivated, pad, false);
}

/// @}
