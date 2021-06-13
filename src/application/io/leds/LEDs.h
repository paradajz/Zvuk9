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
    class LEDs
    {
        public:
        enum class color_t : uint8_t
        {
            off,
            red,
            green,
            yellow,
            blue,
            magenta,
            cyan,
            white,
            AMOUNT
        };

        enum class blinkSpeed_t : uint8_t
        {
            s1000ms,
            s500ms,
            s250ms,
            noBlink
        };

        enum class brightness_t : uint8_t
        {
            bOff,
            b25,
            b50,
            b75,
            b100
        };

        enum class ledState_t : uint32_t
        {
            off,
            on,
            blink
        };

        class HWA
        {
            public:
            HWA() = default;

            virtual void setState(size_t index, brightness_t brightness) = 0;
        };

        LEDs(HWA& hwa, Database& database)
            : _hwa(hwa)
            , _database(database)
        {}

        void       update();
        void       setAllOn();
        void       setAllOff();
        void       setLEDstate(uint8_t ledID, ledState_t state);
        ledState_t getLEDstate(uint8_t ledID);
        void       setNoteLEDstate(MIDI::note_t note, ledState_t state);
        ledState_t getNoteLEDstate(MIDI::note_t note);

        private:
        HWA&      _hwa;
        Database& _database;
    };
}    // namespace IO
