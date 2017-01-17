#pragma once

void handlerInit();

//on/off functions
void handleNotes(bool state);
void handleAftertouch(bool state);
void handleX(bool state);
void handleY(bool state);
void handleSplit(bool state);

//transport control
void handlePlay(bool state);
void handleStop(bool state);
void handleRecord(bool state);

//up/down
void handleUp(bool state);
void handleDown(bool state);

//notes
void handleCsharp(bool state);
void handleDsharp(bool state);
void handleFsharp(bool state);
void handleGsharp(bool state);
void handleAsharp(bool state);
void handleC(bool state);
void handleD(bool state);
void handleE(bool state);
void handleF(bool state);
void handleG(bool state);
void handleA(bool state);
void handleB(bool state);
