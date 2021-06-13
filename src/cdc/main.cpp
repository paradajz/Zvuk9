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

#include <cstdio>
#include <cstring>
#include "board/Board.h"
#include "shared/logger/Logger.h"
#include "shared/Types.h"

Logger logger(Logger::lineEnding_t::lf);

namespace
{
    padData_t padData;
}

class LoggingInterface : public Logger::StreamWriter
{
    public:
    LoggingInterface() = default;

    bool write(const char* text) override
    {
        for (size_t i = 0; i < strlen(text); i++)
        {
            while (!Board::USB::writeCDC((char*)&text[i], 1))
                ;
        }

        return true;
    }
};

int main()
{
    Board::init();

    while (1)
    {
        for (int i = 0; i < NUMBER_OF_PADS; i++)
        {
            if (Board::io::padDataAvailable(i, padData))
            {
                LOG_INFO("Pad %d velocity/pressure/x/y: %d %d %d %d", i, padData.velocity, padData.pressure, padData.x, padData.y);
            }
        }

        LOG_INFO("");
    }

    return 0;
}