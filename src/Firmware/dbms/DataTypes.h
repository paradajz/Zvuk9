#pragma once

#include "Config.h"

typedef enum
{
    BIT_PARAMETER,
    BYTE_PARAMETER,
    WORD_PARAMETER,
    DWORD_PARAMETER
} sectionParameterType_t;

///
/// \brief A structure holding information for a single section.
///
typedef struct
{
    uint16_t parameters;
    sectionParameterType_t parameterType;
    bool preserveOnPartialReset;
    uint16_t defaultValue;
} dbSection_t;

///
/// \brief A structure holding information for a single block.
///
typedef struct
{
    uint8_t sections;
    uint16_t blockStartAddress;
    uint16_t sectionAddress[MAX_SECTIONS];
    dbSection_t section[MAX_SECTIONS];
} blockDescriptor;

typedef enum
{
    initPartial,
    initWipe
} initType_t;