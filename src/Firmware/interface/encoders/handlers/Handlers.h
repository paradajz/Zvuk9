#pragma once

void initHandlers_encoders();

void handleProgram(uint8_t id, int8_t steps);
void handleScale(uint8_t id, int8_t steps);

void handleCC(uint8_t id, int8_t steps);
void handleLimit(uint8_t id, int8_t steps);
void handleCurve(uint8_t id, int8_t steps);