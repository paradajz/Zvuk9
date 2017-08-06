#include "Encoders.h"
#include "../pads/Pads.h"
#include "../buttons/Buttons.h"
#include "../lcd/menu/Menu.h"
#include "../../database/Database.h"
#include "handlers/Handlers.h"

extern void (*encoderHandler[MAX_NUMBER_OF_ENCODERS]) (uint8_t id, int8_t steps);

///
/// \brief Default constructor.
///
Encoders::Encoders()
{
    
}

///
/// \brief Initializes Encoders object.
///
void Encoders::init()
{
    initHandlers_encoders();
}

///
/// \brief Continuously checks state of all encoders.
/// @param [in] If set to true, data from encoders won't be processed. Set to true by default.
///
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
            continue;

        if (lastStepTime[i] < SPEED_TIMEOUT)
        {
            encoderSpeed[i] += ENCODER_SPEED_CHANGE;
            steps = steps > 0 ? encoderSpeed[i] : -encoderSpeed[i];
        }
        else
        {
            encoderSpeed[i] = 0;
        }

        //allow only program and scale encoder while in menu
        //no message on display? maybe TO-DO
        if (menu.isMenuDisplayed())
        {
            if (!((i == PROGRAM_ENCODER) || (i == X_MAX_ENCODER) || (i == X_MIN_ENCODER) || (i == Y_MAX_ENCODER) || (i == Y_MIN_ENCODER)))
                continue;
        }

        if (encoderHandler[i] != NULL)
        {
            //cancel preset button modifier with any other encoder
            if ((i != PRESET_ENCODER) && buttons.getModifierState())
                buttons.setModifierState(false);

            (*encoderHandler[i])(i, steps);
            lastStepTime[i] = 0;
        }
    }
}

///
/// \brief Flushes all data from encoders.
///
void Encoders::flush()
{
    update(false);
}

Encoders encoders;
