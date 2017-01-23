#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#include "handlers/Handlers.h"

extern void (*encoderHandler[NUMBER_OF_ENCODERS]) (uint8_t id, bool state, uint8_t steps);

Encoders::Encoders()
{
    //default constructor
    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
        lastStepTime[i] = 0;
}

void Encoders::init()
{
    initHandlers_encoders();
}

void Encoders::update(bool process)
{
    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
    {
        int8_t encoderSteps = board.getEncoderState(i);

        if (encoderSteps == 0)
            continue;

        uint32_t timeDifference = rTimeMs() - lastStepTime[i];
        uint8_t steps = ENCODER_SPEED_1;

        if (timeDifference > SPEED_TIMEOUT)
            steps = ENCODER_SPEED_1;
        else
            steps = ENCODER_SPEED_2;

        lastStepTime[i] = rTimeMs();

        if (pads.editModeActive())
        {
            display.displayPadEditChangeParametersError();
            return;
        }

        //allow only program and scale encoder while in menu
        //no message on display? maybe TO-DO
        if (menu.menuDisplayed())
        {
            if (!((i == PROGRAM_ENCODER) || (i == PRESET_ENCODER)))
                return;
        }

        if (encoderHandler[i] != NULL)
            (*encoderHandler[i])(i, (encoderSteps > 0), abs(steps));
    }
}

void Encoders::flush()
{
    update(false);
}

Encoders encoders;
