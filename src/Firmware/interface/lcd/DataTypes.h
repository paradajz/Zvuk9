#pragma once

///
/// \brief List of all possible menu types.
///
typedef enum
{
    noMenu,
    userMenu,
    serviceMenu,
} menuType_t;

///
/// \brief List of all possible text types on display.
///
typedef enum
{
    lcdtext_still,
    lcdText_temp
} lcdTextType_t;

///
/// \brief List of possible setting types which are being displayed when changing various parameters.
///
typedef enum
{
    globalSetting,
    singlePadSetting,
    allPadsSetting
} settingType_t;

///
/// \brief List of all possible text scrolling directions.
///
typedef enum
{
    scroll_ltr,
    scroll_rtl
} scrollDirection_t;

///
/// \brief Structure holding data for scrolling event on display for single row.
///
typedef struct
{
    uint8_t size;
    uint8_t startIndex;
    int8_t currentIndex;
    scrollDirection_t direction;
} scrollEvent_t;