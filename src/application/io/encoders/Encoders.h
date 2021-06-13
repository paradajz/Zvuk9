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

#include "database/Database.h"
#include "midi/src/MIDI.h"

namespace IO
{
    class Encoders
    {
        public:
        enum class position_t : uint8_t
        {
            stopped,
            ccw,
            cw,
        };

        enum class acceleration_t : uint8_t
        {
            disabled,
            slow,
            medium,
            fast,
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
            virtual bool isFiltered(size_t                    index,
                                    IO::Encoders::position_t  position,
                                    IO::Encoders::position_t& filteredPosition,
                                    uint32_t                  sampleTakenTime) = 0;

            virtual void     reset(size_t index)            = 0;
            virtual uint32_t lastMovementTime(size_t index) = 0;
        };

        Encoders(HWA&      hwa,
                 Filter&   filter,
                 uint32_t  timeDiffTimeout,
                 Database& database,
                 MIDI&     midi)
            : _hwa(hwa)
            , _filter(filter)
            , TIME_DIFF_READOUT(timeDiffTimeout)
            , _database(database)
            , _midi(midi)
        {}

        void init();
        void update(bool process = true);
        void setMIDIchannelPresetEncMode(bool state);
        bool getMIDIchannelPresetEncMode();

        private:
        HWA&    _hwa;
        Filter& _filter;

        /// Time difference betweeen multiple encoder readouts in milliseconds.
        const uint32_t TIME_DIFF_READOUT;

        Database& _database;
        MIDI&     _midi;
    };
}    // namespace IO
