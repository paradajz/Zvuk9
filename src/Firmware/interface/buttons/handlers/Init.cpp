#include "Handlers.h"

#include "../../../board/Board.h"

void (*buttonHandler[MAX_NUMBER_OF_BUTTONS]) (bool state);

void handlerInit()
{
    for (int i=0; i<MAX_NUMBER_OF_BUTTONS; i++)
        buttonHandler[i] = NULL;

    buttonHandler[BUTTON_TRANSPORT_PLAY] = handlePlay;
    buttonHandler[BUTTON_TRANSPORT_STOP] = handleStop;
    buttonHandler[BUTTON_TRANSPORT_RECORD] = handleRecord;
}