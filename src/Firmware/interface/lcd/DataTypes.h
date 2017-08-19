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