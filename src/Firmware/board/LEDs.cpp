#include "Board.h"

volatile int8_t     activeLEDcolumn;

volatile bool       blinkEnabled,
                    blinkState;

volatile uint8_t    pwmSteps,
                    ledState[MAX_NUMBER_OF_LEDS];

volatile uint16_t   ledBlinkTime;

volatile int8_t     transitionCounter[MAX_NUMBER_OF_LEDS];
volatile uint32_t   blinkTimerCounter;

void Board::initLEDs()
{
    setOutput(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setOutput(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setOutput(LED_ROW_3_PORT, LED_ROW_3_PIN);

    #ifdef LED_INVERT
    setHigh(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setHigh(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setHigh(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #else
    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN);
    #endif

    setOutput(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setOutput(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setOutput(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);

    setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN);
    setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN);
    setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN);
}
