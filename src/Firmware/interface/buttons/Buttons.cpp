#include "Buttons.h"

#ifdef NDEBUG
#include "../../midi/MIDI.h"
#endif
#include "../lcd/LCD.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"

note_t Buttons::getTonicFromButton(uint8_t buttonNumber)
{
    return buttonToNoteArray[buttonNumber];
}

void Buttons::setTransportControlType(transportControlType_t type)
{
    transportControlType = type;
}

transportControlType_t Buttons::getTransportControlType()
{
    return transportControlType;
}

Buttons buttons;
