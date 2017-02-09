#pragma once

#ifdef BOARD_R1
#define PAD_0                       2
#define PAD_1                       1
#define PAD_2                       0
#define PAD_3                       3
#define PAD_4                       4
#define PAD_5                       5
#define PAD_6                       6
#define PAD_7                       7
#define PAD_8                       8
#else
#define PAD_0                       2
#define PAD_1                       1
#define PAD_2                       0
#define PAD_3                       3
#define PAD_4                       4
#define PAD_5                       5
#define PAD_6                       6
#define PAD_7                       7
#define PAD_8                       8
#endif

const uint8_t padIDArray[] =
{
    PAD_0,
    PAD_1,
    PAD_2,
    PAD_3,
    PAD_4,
    PAD_5,
    PAD_6,
    PAD_7,
    PAD_8
};
