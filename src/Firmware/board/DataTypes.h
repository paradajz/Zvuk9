#pragma once

typedef enum
{
    rebootApp,
    rebootBtldr
} rebootType_t;

typedef enum
{
    readPressure0,
    readPressure1,
    readX,
    readY,
    PAD_READINGS
} padReadOrder;