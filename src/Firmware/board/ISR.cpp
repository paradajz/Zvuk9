#include "Board.h"

//timer0 - delay/millis/micros/encoders
//timer1 - LEDs (pwm)
//timer2 - LEDs (pwm)
//timer3 - LED matrix

//ISR runs at 1.5ms
#define LED_BLINK_TIME_SCALED   (LED_BLINK_TIME / (float)1.5 / NUMBER_OF_LED_COLUMNS)

void Board::initTimers()    {

    //configure timer3
    //used for led matrix
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;
    TIMSK3 = 0;
    OCR3A = 0;
    OCR3B = 0;
    OCR3C = 0;

    //set CTC mode
    TCCR3B |= (1 << WGM32);

    //set prescaler to 64
    TCCR3B |= (1 << CS31) | (1 << CS30);

    //set compare match register to desired timer count
    OCR3A = 374; //1.5ms

    //enable CTC interrupt for timer3
    TIMSK3 |= (1<<OCIE3A);

    //configure timer1/2 for LED matrix PWM
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIMSK1 = 0;
    TCNT1 = 0;
    OCR1A = 0;
    OCR1B = 0;
    OCR1C = 0;

    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    TCNT2 = 0;
    OCR2A = 0;
    OCR2B = 0;

    //phase correct PWM, top 0xFF
    TCCR1A |= (1<<WGM10);
    TCCR2A |= (1<<WGM20);

    //prescaler 1
    TCCR1B |= (1<<CS10);
    TCCR2B |= (1<<CS20);

    //configure timer0, used to read encoders and for millis
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;

    //set CTC mode
    TCCR0A |= (1<<WGM01);

    //set prescaler 64
    TCCR0B |= (1<<CS00) | (1<<CS01);

    //set compare match register to desired timer count
    OCR0A = 124; //500us

    //enable CTC interrupt for timer0
    TIMSK0 |= (1 << OCIE0A);

}

inline void activateColumn(uint8_t column) {

    bitRead(column, 0) ? setHigh(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX) : setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    bitRead(column, 1) ? setHigh(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX) : setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    bitRead(column, 2) ? setHigh(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX) : setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

}

inline void ledRowOn(uint8_t rowNumber, uint8_t value)  {

    switch (rowNumber)  {

        case 0:
        OCR2A = value;
        TCCR2A |= (1<<COM2A1);
        break;

        case 1:
        OCR1A = value;
        TCCR1A |= (1<<COM1A1);
        break;

        case 2:
        OCR1B = value;
        TCCR1A |= (1<<COM1B1);
        break;

    }

}

inline void ledRowsOff() {

    //turn off pwm
    TCCR2A &= ~(1<<COM2A1);
    TCCR1A &= ~(1<<COM1A1);
    TCCR1A &= ~(1<<COM1B1);

    setLow(LED_ROW_1_PORT, LED_ROW_3_PIN_INDEX);
    setLow(LED_ROW_2_PORT, LED_ROW_3_PIN_INDEX);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

}

inline uint8_t ledStateToValue(uint8_t state)  {

    if (bitRead(state, LED_HALF_INTENSITY_BIT))
        return LED_INTENSITY_HALF;

    if (bitRead(state, LED_FULL_INTENSITY_BIT))
        return LED_INTENSITY_FULL;

    return 0;

}

ISR(TIMER3_COMPA_vect)  {

    //LEDs
    ledRowsOff();
    if (activeColumnInterrupt == NUMBER_OF_LED_COLUMNS)
    activeColumnInterrupt = 0;
    activateColumn(activeColumnInterrupt);

    uint8_t ledNumber;
    uint8_t ledStateSingle;
    uint8_t currentStepValue;
    bool stepDirection;
    bool stepUpdate;

    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)  {

        ledNumber = activeColumnInterrupt+i*NUMBER_OF_LED_COLUMNS;
        ledStateSingle = ledStateToValue(ledState[ledNumber]);

        if (bitRead(ledState[ledNumber], LED_BLIK_ENABLED_BIT)) {

            ledBlinkTimer[ledNumber]++;

            if (ledBlinkTimer[ledNumber] >= LED_BLINK_TIME_SCALED)  {

                bitWrite(ledState[ledNumber], LED_BLINK_STATE_BIT, !bitRead(ledState[ledNumber], LED_BLINK_STATE_BIT));
                ledBlinkTimer[ledNumber] = 0;

            }

            bitRead(ledState[ledNumber], LED_BLINK_STATE_BIT) ? ledStateSingle = LED_INTENSITY_FULL : ledStateSingle = LED_INTENSITY_OFF;

        }

        currentStepValue = transitionCounter[ledNumber];
        stepUpdate = currentStepValue != ledStateSingle;

        if (currentStepValue)
            ledRowOn(i, currentStepValue);

        if (stepUpdate) {

            stepDirection = currentStepValue < ledStateSingle;

            switch(stepDirection)   {

                case true:
                transitionCounter[ledNumber] += pwmSteps;
                if (transitionCounter[ledNumber] > ledStateSingle)
                transitionCounter[ledNumber] = ledStateSingle;
                break;

                case false:
                transitionCounter[ledNumber] -= pwmSteps;
                if (transitionCounter[ledNumber] < ledStateSingle)
                transitionCounter[ledNumber] = ledStateSingle;
                break;

            }

        }

    }

    activeColumnInterrupt++;

}

inline void updateEncoder(uint8_t encoderID)    {

    uint8_t p1val = readPin(*encoderPort1Array[encoderID], encoderPinIndex1Array[encoderID]);
    uint8_t p2val = readPin(*encoderPort2Array[encoderID], encoderPinIndex2Array[encoderID]);

    uint8_t pairState = p1val;
    pairState <<= 1;
    pairState |= p2val;

    //add new data
    uint8_t newPairData = 0;
    newPairData |= (((encoderData[encoderID] << 2) & 0x000F) | (uint16_t)pairState);
    encoderData[encoderID] &= ENCODER_CLEAR_TEMP_STATE_MASK;

    //shift in new data
    encoderData[encoderID] |= (uint16_t)newPairData;

    int8_t encRead = encoderLookUpTable[newPairData];

    if (!encRead) return;

    bool newEncoderDirection = encRead > 0;
    //get current number of pulses from encoderData
    int8_t currentPulses = (encoderData[encoderID] >> 4) & 0x000F;
    currentPulses += encRead;
    //clear current pulses
    encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;
    //shift in new pulse count
    encoderData[encoderID] |= (uint16_t)(currentPulses << 4);
    //get last encoder direction
    bool lastEncoderDirection = bitRead(encoderData[encoderID], ENCODER_DIRECTION_BIT);
    //write new encoder direction
    bitWrite(encoderData[encoderID], ENCODER_DIRECTION_BIT, newEncoderDirection);

    if (lastEncoderDirection != newEncoderDirection) return;
    if (currentPulses % PULSES_PER_STEP) return;

    //clear current pulses
    encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;

    //set default pulse count
    encoderData[encoderID] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);

    //clear current moving status
    if (newEncoderDirection) encoderMoving[encoderID]++;
    else encoderMoving[encoderID]--;

}

ISR(TIMER0_COMPA_vect)  {

    static uint8_t encoderCounter = 0;
    updateEncoder(encoderCounter);
    encoderCounter++;
    if (encoderCounter == NUMBER_OF_ENCODERS)
    encoderCounter = 0;

    static bool updateMillis = true;

    updateMillis = !updateMillis;

    if (updateMillis)   {

        uint32_t ms;
        ms = rTime_ms;
        ms++;
        //update run time
        rTime_ms = ms;

    }

}