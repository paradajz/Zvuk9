#pragma once

typedef enum
{
    transportStop,
    transportPlay,
    transportRecordOn,
    transportRecordOff
} transportControl_t;

typedef enum
{
    transportMMC,
    transportCC,
    transportMMC_CC
} transportControlType_t;
