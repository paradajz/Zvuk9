#include "Board.h"

volatile int8_t      activeColumnInterrupt;
uint8_t              ledState[NUMBER_OF_LEDS];
int16_t              transitionCounter[NUMBER_OF_LEDS];
volatile uint8_t     pwmSteps = DEFAULT_FADE_SPEED;
uint32_t             ledBlinking;
uint16_t             ledBlinkTimer[NUMBER_OF_LEDS];

void Board::initLEDs()  {

    setOutput(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setOutput(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setOutput(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

    setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

}

uint8_t ledStateToValue(ledState_t state)  {

    switch(state)   {

        case ledStateOff:
        return 0;

        case ledStateDim:
        return LED_INTENSITY_HALF;

        case ledStateFull:
        return LED_INTENSITY_FULL;

        case ledStateBlink:
        return LED_INTENSITY_FULL;

        default:
        return 0;

    }

}

ledState_t Board::getLEDstate(uint8_t ledNumber)  {

    if (bitRead(ledBlinking, ledNumber))
    return ledStateBlink;

    switch(ledState[ledNumber]) {

        case LED_INTENSITY_FULL:
        return ledStateFull;

        case LED_INTENSITY_HALF:
        return ledStateDim;

        default:
        return ledStateOff;

    }

}

void Board::setLEDstate(uint8_t ledNumber, ledState_t state) {

    uint8_t value = ledStateToValue(state);

    if (state == ledStateBlink) {

        bitWrite(ledBlinking, ledNumber, 1);
        //make sure led starts blinking immediately
        if ((ledState[ledNumber] == LED_INTENSITY_FULL) || (ledState[ledNumber] == LED_INTENSITY_HALF))
        value = LED_INTENSITY_OFF;
        else value = LED_INTENSITY_FULL;

    } else if (bitRead(ledBlinking, ledNumber))   {

        bitWrite(ledBlinking, ledNumber, 0);
        ledBlinkTimer[ledNumber] = 0;

    }

    ledState[ledNumber] = value;

}

void Board::setPWMsteps(uint8_t steps)    {

    pwmSteps = steps;

}