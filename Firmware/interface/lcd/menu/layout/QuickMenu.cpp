#include "../Menu.h"
#ifdef MENU_H_
#include "../items/QuickMenu.h"

#include "../../../../version/Firmware.h"
#include "../../../../version/Hardware.h"
#include "../../../../eeprom/Configuration.h"
#include "../../../../init/Init.h"

#include "../functions/MenuFunctions.h"

void Menu::createQuickMenuLayout()   {

    if (pads.isUserScale(pads.getActivePreset()))
    {
        menuItem[quickMenuItem_us_octaveShift].stringPointer = shiftMode_octave_string;
        menuItem[quickMenuItem_us_octaveShift].level = 1;
        menuItem[quickMenuItem_us_octaveShift].function = changeOctaveButtons;
        menuItem[quickMenuItem_us_octaveShift].argument.argument1 = (uint8_t)shiftMode_octave;

        menuItem[quickMenuItem_us_midiChannelChange].stringPointer = shiftMode_midiChannel_string;
        menuItem[quickMenuItem_us_midiChannelChange].level = 2;
        menuItem[quickMenuItem_us_midiChannelChange].function = changeOctaveButtons;
        menuItem[quickMenuItem_us_midiChannelChange].argument.argument1 = (uint8_t)shiftMode_channel;

        menuItem[quickMenuItem_us_padEditMode].stringPointer = padEditMode_string;
        menuItem[quickMenuItem_us_padEditMode].level = 3;
        menuItem[quickMenuItem_us_padEditMode].function = padEditMode;

        menuItem[quickMenuItem_us_userMenu].stringPointer = menuType_user;
        menuItem[quickMenuItem_us_userMenu].level = 4;
        menuItem[quickMenuItem_us_userMenu].function = NULL;
    }

    else
    {
        menuItem[quickMenuItem_ps_octaveShift].stringPointer = shiftMode_octave_string;
        menuItem[quickMenuItem_ps_octaveShift].level = 1;
        menuItem[quickMenuItem_ps_octaveShift].function = changeOctaveButtons;
        menuItem[quickMenuItem_ps_octaveShift].argument.argument1 = (uint8_t)shiftMode_octave;

        menuItem[quickMenuItem_ps_midiChannelChange].stringPointer = shiftMode_midiChannel_string;
        menuItem[quickMenuItem_ps_midiChannelChange].level = 2;
        menuItem[quickMenuItem_ps_midiChannelChange].function = changeOctaveButtons;
        menuItem[quickMenuItem_ps_midiChannelChange].argument.argument1 = (uint8_t)shiftMode_channel;

        menuItem[quickMenuItem_ps_noteShift].stringPointer = shiftMode_note_string;
        menuItem[quickMenuItem_ps_noteShift].level = 3;
        menuItem[quickMenuItem_ps_noteShift].function = changeOctaveButtons;
        menuItem[quickMenuItem_ps_noteShift].argument.argument1 = (uint8_t)shiftMode_octave;

        menuItem[quickMenuItem_ps_userMenu].stringPointer = menuType_user;
        menuItem[quickMenuItem_ps_userMenu].level = 4;
        menuItem[quickMenuItem_ps_userMenu].function = NULL;
    }

}
#endif