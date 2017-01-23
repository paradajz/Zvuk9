#include "Handlers.h"

#include "../../../board/Board.h"

void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (uint8_t data, bool state);

void initHandlers_buttons()
{
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
        buttonHandler[i] = NULL;

    //transport controls
    buttonHandler[BUTTON_TRANSPORT_PLAY] = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_STOP] = handleTransportControl;
    buttonHandler[BUTTON_TRANSPORT_RECORD] = handleTransportControl;

    //on/ff
    buttonHandler[BUTTON_ON_OFF_NOTES] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_AFTERTOUCH] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_X] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_Y] = handleOnOff;
    buttonHandler[BUTTON_ON_OFF_SPLIT] = handleOnOff;

    //up/down
    buttonHandler[BUTTON_OCTAVE_DOWN] = handleUpDown;
    buttonHandler[BUTTON_OCTAVE_UP] = handleUpDown;

    //notes
    buttonHandler[BUTTON_NOTE_C_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_D_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_F_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_G_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_A_SHARP] = handleTonic;
    buttonHandler[BUTTON_NOTE_C] = handleTonic;
    buttonHandler[BUTTON_NOTE_D] = handleTonic;
    buttonHandler[BUTTON_NOTE_E] = handleTonic;
    buttonHandler[BUTTON_NOTE_F] = handleTonic;
    buttonHandler[BUTTON_NOTE_G] = handleTonic;
    buttonHandler[BUTTON_NOTE_A] = handleTonic;
    buttonHandler[BUTTON_NOTE_B] = handleTonic;
}
