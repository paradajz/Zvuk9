#pragma once

typedef enum
{
    noMenu,
    userMenu,
    serviceMenu,
} menuType_t;

typedef enum
{
    lcdtext_still,
    lcdText_temp
} lcdTextType_t;

typedef enum
{
    globalSetting,
    singlePadSetting,
    allPadsSetting
} settingType_t;

typedef enum
{
    scroll_ltr,
    scroll_rtl
} scrollDirection_t;

typedef struct
{
    bool enabled;
    uint8_t size;
    uint8_t startIndex;
    int8_t currentIndex;
    scrollDirection_t direction;
} scrollEvent_t;