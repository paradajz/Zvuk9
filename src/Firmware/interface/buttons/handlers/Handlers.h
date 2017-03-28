#pragma once

void initHandlers_buttons();

//on/off functions
void handleOnOff(uint8_t id, bool state);

//transport control
void handleTransportControl(uint8_t id, bool state);

//up/down
void handleUpDown(uint8_t id, bool state);

//notes
void handleTonic(uint8_t id, bool state);

void handleProgramEncButton(uint8_t id, bool state);
void handlePresetEncButton(uint8_t id, bool state);