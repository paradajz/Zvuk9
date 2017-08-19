#pragma once

#include "../dbms/src/DBMS.h"
#include "blocks/Blocks.h"
#include "Config.h"

///
/// \addtogroup eeprom
/// @{
///
class Database : public DBMS
{
    public:
    Database();
    void init();
    void factoryReset(initType_t type);
    bool signatureValid();

    private:
    void createLayout();
    void writeCustomValues();
    void initProgramSettings();
    void initScales();
    void initGlobalSettings();
};

extern Database database;
/// @}
