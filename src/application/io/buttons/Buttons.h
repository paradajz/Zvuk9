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

#pragma once

#include "midi/src/MIDI.h"

namespace IO
{
    class Buttons
    {
        public:
        enum class transportControlMode_t : uint32_t
        {
            MMC,      /// Uses standard MIDI transport control messages.
            CC,       /// Uses MIDI-mappable predefined CC messages for transport controls
            MMC_CC    /// Uses both standard MIDI transport control messages and CC messages.
        };

        enum class button_t : uint32_t
        {
            NOTE_C_SHARP,
            NOTE_D_SHARP,
            NOTE_F_SHARP,
            NOTE_G_SHARP,
            NOTE_A_SHARP,
            NOTE_C,
            NOTE_D,
            NOTE_E,
            NOTE_F,
            NOTE_G,
            NOTE_A,
            NOTE_B,
            ON_OFF_SPLIT,
            ON_OFF_X,
            ON_OFF_Y,
            ON_OFF_AFTERTOUCH,
            ON_OFF_NOTES,
            OCTAVE_DOWN,
            OCTAVE_UP,
            TRANSPORT_PLAY,
            TRANSPORT_STOP,
            TRANSPORT_RECORD,
            PROGRAM_ENC,
            PRESET_ENC,
            AMOUNT
        };

        class HWA
        {
            public:
            //should return true if the value has been refreshed, false otherwise
            virtual bool state(size_t index, uint8_t& numberOfReadings, uint32_t& states) = 0;
        };

        class Filter
        {
            public:
            virtual bool isFiltered(size_t index, uint8_t& numberOfReadings, uint32_t& states) = 0;
        };

        Buttons(HWA&    hwa,
                Filter& filter)
            : _hwa(hwa)
            , _filter(filter)
        {}

        void                   init();
        void                   update();
        void                   setButtonEnableState(int8_t buttonID, bool state);
        bool                   getButtonEnableState(uint8_t buttonID);
        void                   setTransportControlMode(transportControlMode_t type);
        transportControlMode_t getTransportControlMode();
        MIDI::note_t           getNoteFromButton(uint8_t buttonNumber);
        bool                   getButtonState(uint8_t buttonID);
        void                   setButtonState(uint8_t buttonID, bool state);

        private:
        bool buttonDebounced(uint8_t buttonID, bool buttonState);
        void handleTransportControlEvent(uint8_t buttonNumber, bool state);
        void handleTonicEvent(MIDI::note_t note, bool state);
        void handleOctaveEvent(bool direction, bool state);
        void processButton(uint8_t button, uint8_t state);
        void initHandlers();
        void handleOnOff(uint8_t id, bool state);
        void handleTransportControl(uint8_t id, bool state);
        void handleUpDown(uint8_t id, bool state);
        void handleTonic(uint8_t id, bool state);
        void handleProgramEncButton(uint8_t id, bool state);
        void handlePresetEncButton(uint8_t id, bool state);

        /// External definition of function pointers used as handlers for buttons.
        void (*buttonHandler[MAX_NUMBER_OF_BUTTONS])(uint8_t data, bool state);

        /// Array holding information about whether specific button is enabled or not.
        /// Each button has 1 bit in order to reserve space.
        uint8_t buttonEnabled[MAX_NUMBER_OF_BUTTONS / 8 + 1];

        /// Flag holding information about whether buttons are handled or not.
        /// If set to false, pressing buttons will result in no action being taken.
        bool processingEnabled;

        HWA&    _hwa;
        Filter& _filter;

        /// Holds currently active transport control mode.
        transportControlMode_t transportControlMode;

        /// Array holding mapping of notes to specific buttons.
        uint8_t buttonToNoteArray[MAX_NUMBER_OF_BUTTONS];

        /// Array holding current state of buttons.
        /// Each button has 1 bit in order to reserve space.
        uint8_t buttonPressed[MAX_NUMBER_OF_BUTTONS / 8 + 1];

        /// Array holding debounce count for all buttons to avoid incorrect state detection.
        uint8_t buttonDebounceCounter[MAX_NUMBER_OF_BUTTONS];
    };
}    // namespace IO