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

///
/// \brief Default constructor.
///
Database::Database()
{
    
}

///
/// \brief Initializes database.
///
void Database::init()
{
    createLayout();
    DBMS::commitLayout();
}

///
/// \brief Performs factory reset of data in database.
/// @param [in] type Factory reset type. See initType_t enumeration.
///
void Database::factoryReset(initType_t type)
{
    DBMS::initData(type);
    writeCustomValues();
}

///
/// \brief Checks if database has been already initialized by checking DB_BLOCK_ID.
/// \returns True if valid, false otherwise.
///
bool Database::signatureValid()
{
    //check if all bytes up to START_OFFSET address match unique id

    for (int i=0; i<ID_OFFSET; i++)
    {
        if (DBMS::read(DB_BLOCK_ID, 0, i) != UNIQUE_ID)
            return false;
    }

    return true;
}

Database database;
