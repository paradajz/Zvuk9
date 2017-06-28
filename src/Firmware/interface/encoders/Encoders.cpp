#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#include "handlers/Handlers.h"

extern void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, bool state, uint8_t steps);

Encoders::Encoders()
{
    
}

void Encoders::init()
{
    initHandlers_encoders();
}

void Encoders::update(bool process)
{
    int8_t steps;

    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        if (lastStepTime[i] < 255)
            lastStepTime[i]++;

        if (!board.encoderEnabled(i))
            continue;

        steps = board.getEncoderState(i);

        if (!process)
            continue;

        if (steps == 0)
            continue;;

        bool direction = steps > 0;

        if (lastStepTime[i] < SPEED_TIMEOUT)
            steps = ENCODER_SPEED_2;

        lastStepTime[i] = 0;

        if (pads.getEditModeState())
        {
            display.displayPadEditChangeParametersError();
            continue;
        }

        //allow only program and scale encoder while in menu
        //no message on display? maybe TO-DO
        if (menu.menuDisplayed())
        {
            if (!((i == PROGRAM_ENCODER) || (i == X_MAX_ENCODER) || (i == X_MIN_ENCODER) || (i == Y_MAX_ENCODER) || (i == Y_MIN_ENCODER)))
                continue;
        }

        if (encoderHandler[i] != NULL)
        {
            //cancel preset button modifier with any other encoder
            if ((i != PRESET_ENCODER) && buttons.getModifierState())
                buttons.setModifierState(false);

            (*encoderHandler[i])(i, direction, abs(steps));
        }
    }
}

void Encoders::flush()
{
    update(false);
}

Encoders encoders;
