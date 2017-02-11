#include "Handlers.h"

#include "../../../board/Board.h"

void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, bool state, uint8_t steps);

void initHandlers_encoders()
{
    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
        encoderHandler[i] = NULL;

    encoderHandler[PROGRAM_ENCODER] = handleProgram;
    encoderHandler[PRESET_ENCODER] = handleScale;

    encoderHandler[X_CC_ENCODER] = handleCC;
    encoderHandler[Y_CC_ENCODER] = handleCC;

    encoderHandler[X_MAX_ENCODER] = handleLimit;
    encoderHandler[X_MIN_ENCODER] = handleLimit;
    encoderHandler[Y_MAX_ENCODER] = handleLimit;
    encoderHandler[Y_MIN_ENCODER] = handleLimit;

    encoderHandler[X_CURVE_ENCODER] = handleCurve;
    encoderHandler[Y_CURVE_ENCODER] = handleCurve;
}