#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#include "handlers/Handlers.h"

extern void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, bool state, uint8_t steps);

Encoders::Encoders()
{
    //default constructor
    for (int i=0; i<CONNECTED_ENCODERS; i++)
        lastStepTime[i] = 0;
}

void Encoders::init()
{
    initHandlers_encoders();
}

void Encoders::update(bool process)
{
    int8_t steps;

    uint32_t currentTime = rTimeMs();

    for (int i=0; i<CONNECTED_ENCODERS; i++)
    {
        steps = board.getEncoderState(i);

        if (steps == 0)
            continue;

        bool direction = steps > 0;

        uint32_t timeDifference = currentTime - lastStepTime[i];

        if (timeDifference < SPEED_TIMEOUT)
            steps = ENCODER_SPEED_2;

        lastStepTime[i] = currentTime;

        if (pads.getEditModeState())
        {
            display.displayPadEditChangeParametersError();
            return;
        }

        //allow only program and scale encoder while in menu
        //no message on display? maybe TO-DO
        if (menu.menuDisplayed())
        {
            if (!((encoderMap[i] == PROGRAM_ENCODER) || (encoderMap[i] == PRESET_ENCODER)))
                return;
        }

        if (encoderHandler[encoderMap[i]] != NULL)
        {
            //cancel preset button modifier with any other encoder
            if ((encoderMap[i] != PRESET_ENCODER) && buttons.getModifierState())
                buttons.setModifierState(false);

            (*encoderHandler[encoderMap[i]])(encoderMap[i], direction, abs(steps));
        }
    }
}

void Encoders::flush()
{
    update(false);
}

Encoders encoders;
