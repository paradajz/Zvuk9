#include "Board.h"

volatile int8_t     activeColumnInterrupt;
uint8_t             ledState[NUMBER_OF_LEDS];
int16_t             transitionCounter[NUMBER_OF_LEDS];
volatile uint8_t    pwmSteps = DEFAULT_FADE_SPEED;
bool                blinkEnabled;
volatile uint8_t    ledValue[NUMBER_OF_LEDS];

uint8_t ledStateToValue(uint8_t state)  {

    if (bitRead(state, LED_HALF_INTENSITY_BIT))
        return LED_INTENSITY_HALF;

    if (bitRead(state, LED_FULL_INTENSITY_BIT))
        return LED_INTENSITY_FULL;

    return 0;

}

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

ledState_t Board::getLEDstate(uint8_t ledNumber)  {

    if (bitRead(ledState[ledNumber], LED_HALF_INTENSITY_BIT))
        return ledStateDim;

    if (bitRead(ledState[ledNumber], LED_FULL_INTENSITY_BIT))
    return ledStateFull;

    return ledStateOff;

}

void Board::setLEDstate(uint8_t ledNumber, ledState_t state, bool blink) {

    switch(state)   {

        case ledStateOff:
        ledState[ledNumber] = 0;
        break;

        case ledStateDim:
        //set dim intensity bit
        bitSet(ledState[ledNumber], LED_HALF_INTENSITY_BIT);
        //clear full intensity bit
        bitClear(ledState[ledNumber], LED_FULL_INTENSITY_BIT);
        //clear blinking bits, just in case
        bitClear(ledState[ledNumber], LED_BLINK_ENABLED_BIT);
        bitClear(ledState[ledNumber], LED_BLINK_STATE_BIT);
        break;

        case ledStateFull:
        //set full intensity bit
        bitSet(ledState[ledNumber], LED_FULL_INTENSITY_BIT);
        //clear dim intensity bit
        bitClear(ledState[ledNumber], LED_HALF_INTENSITY_BIT);
        //clear blinking bits, just in case
        bitClear(ledState[ledNumber], LED_BLINK_ENABLED_BIT);
        bitClear(ledState[ledNumber], LED_BLINK_STATE_BIT);
        break;

        default:
        return;

    }

    if (blink) {

        //enable blinking
        bitSet(ledState[ledNumber], LED_BLINK_ENABLED_BIT);
        //make sure led starts blinking immediately
        bitSet(ledState[ledNumber], LED_BLINK_STATE_BIT);

    } else {

        if (bitRead(ledState[ledNumber], LED_BLINK_ENABLED_BIT))   {

            //disable blinking bits
            bitClear(ledState[ledNumber], LED_BLINK_ENABLED_BIT);
            bitClear(ledState[ledNumber], LED_BLINK_STATE_BIT);

        }

    }

    ledValue[ledNumber] = ledStateToValue(ledState[ledNumber]);

}

void Board::setPWMsteps(uint8_t steps)    {

    pwmSteps = steps;

}

bool Board::ledBlinking(uint8_t ledNumber)  {

    return (bitRead(ledState[ledNumber], LED_BLINK_ENABLED_BIT));

}