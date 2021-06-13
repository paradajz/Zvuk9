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

#include "Database.h"
#include "Layout.h"

/// Helper macro for easier entry and exit from system block.
/// Important: ::init must called before trying to use this macro.
#define SYSTEM_BLOCK_ENTER(code)                                                                           \
    {                                                                                                      \
        LESSDB::setLayout(dbLayout, static_cast<uint8_t>(block_t::AMOUNT) + 1, 0);                         \
        code                                                                                               \
            LESSDB::setLayout(&dbLayout[1], static_cast<uint8_t>(block_t::AMOUNT), _userDataStartAddress); \
    }

/// Initializes database.
bool Database::init()
{
    if (!LESSDB::init())
        return false;

    //set only system block for now
    if (!LESSDB::setLayout(dbLayout, 1, 0))
    {
        return false;
    }
    else
    {
        _userDataStartAddress = LESSDB::nextParameterAddress();
    }

    bool returnValue = true;

    if (!isSignatureValid())
    {
        returnValue = factoryReset(LESSDB::factoryResetType_t::full);
    }

    if (returnValue)
    {
        _initialized = true;

        if (_handlers != nullptr)
            _handlers->initialized();
    }

    return returnValue;
}

bool Database::isInitialized()
{
    return _initialized;
}

bool Database::factoryReset(LESSDB::factoryResetType_t type)
{
    if (_handlers != nullptr)
        _handlers->factoryResetStart();

    if (!clear())
        return false;

    if (_initializeData)
    {
        //init system block first
        SYSTEM_BLOCK_ENTER(
            if (!initData(type)) return false;);

        //now the rest
        if (!initData(type))
            return false;

        writeDefaults();

        if (!setDbUID(getDbUID()))
            return false;
    }

    if (_handlers != nullptr)
        _handlers->factoryResetDone();

    return true;
}

/// Checks if database has been already initialized by checking DB_BLOCK_ID.
/// returns: True if valid, false otherwise.
bool Database::isSignatureValid()
{
    uint16_t signature;

    SYSTEM_BLOCK_ENTER(
        signature = read(0,
                         static_cast<uint8_t>(SectionPrivate::system_t::uid),
                         0);)

    return getDbUID() == signature;
}

/// Calculates unique database ID.
/// UID is calculated by appending number of parameters and their types for all
/// sections and all blocks.
uint16_t Database::getDbUID()
{
    /// Magic value with which calculated signature is XORed.
    const uint16_t uidBase = 0x1701;

    uint16_t signature = 0;

    //get unique database signature based on its blocks/sections
    for (int i = 0; i < static_cast<uint8_t>(block_t::AMOUNT) + 1; i++)
    {
        for (int j = 0; j < dbLayout[i].numberOfSections; j++)
        {
            signature += static_cast<uint16_t>(dbLayout[i].section[j].numberOfParameters);
            signature += static_cast<uint16_t>(dbLayout[i].section[j].parameterType);
        }
    }

    return signature ^ uidBase;
}

/// Updates unique database UID.
/// UID is written to first two database locations.
/// param [in]: uid Database UID to set.
bool Database::setDbUID(uint16_t uid)
{
    bool returnValue;

    SYSTEM_BLOCK_ENTER(
        returnValue = update(0, static_cast<uint8_t>(SectionPrivate::system_t::uid), 0, uid);)

    return returnValue;
}

void Database::registerHandlers(Handlers& handlers)
{
    _handlers = &handlers;
}