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
