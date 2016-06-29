#ifndef QUICKMENU_H_
#define QUICKMENU_H_

typedef enum {

    quickMenuItem_us_octaveShift,
    quickMenuItem_us_midiChannelChange,
    quickMenuItem_us_padEditMode,
    quickMenuItem_us_userMenu,

    QUICK_MENU_ITEMS_USER_SCALE

} quickMenuItems_userScale_t;

typedef enum {

    quickMenuItem_ps_octaveShift,
    quickMenuItem_ps_midiChannelChange,
    quickMenuItem_ps_noteShift,
    quickMenuItem_ps_userMenu,

    QUICK_MENU_ITEMS_PREDEFINED_SCALE

} quickMenuItems_predefinedScale_t;

#endif