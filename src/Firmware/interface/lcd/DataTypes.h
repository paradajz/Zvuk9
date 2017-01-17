#pragma once

typedef enum
{
    noMessage,
    showMessage,
    messageDisplayed,
    clearMessage
} messageStatus_t;

typedef enum
{
    userMenu,
    serviceMenu,
    noMenu
} menuType_t;

typedef enum
{
    text,
    message
} lcdTextType;
