#include "Pads.h"

void Pads::setHandleLCDxyCallback(void (*fptr)(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable))    {

    sendLCDxyCallback = fptr;

}

void Pads::setHandlePadPressCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY))   {

    sendPadPressCallback = fptr;

}

void Pads::setHandlePadReleaseCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes))   {

    sendPadReleaseCallback = fptr;

}

void Pads::setHandleLCDAfterTouchCallback(void (*fptr)(uint8_t pressure))   {

    sendLCDAfterTouchCallback = fptr;

}

void Pads::setHandleLEDstateCallback(void (*fptr)(uint8_t ledNumber, ledIntensity_t state))   {

    sendLEDstateCallback = fptr;

}

void Pads::setHandleClearPadDataCallback(void (*fptr)(uint8_t pad))    {

    sendClearPadDataCallback = fptr;

}

void Pads::setHandlePadEditModeCallback(void (*fptr)(uint8_t pad))  {

    sendPadEditModeCallback = fptr;

}