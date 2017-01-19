#pragma once

void handlerInit();

//on/off functions
void handleOnOff(uint8_t id, bool state);

//transport control
void handleTransportControl(uint8_t id, bool state);

//up/down
void handleUpDown(uint8_t id, bool state);

//notes
void handleTonic(uint8_t id, bool state);