#include "Board.h"

//timer0 - delay/millis/micros/encoders
//timer1 - LEDs (pwm)
//timer2 - LEDs (pwm)
//timer3 - LED matrix

void Board::initTimers()    {

    //clear timer3 info
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    TIMSK3 = 0;
    OCR3A = 0;
    OCR3B = 0;
    OCR3C = 0;

    //turn on CTC mode
    TCCR3B |= (1 << WGM32);

    //set prescaler to 256
    TCCR3B |= (1 << CS32);

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

    //configure timer0
    //used for delay/millis/leds
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;
    OCR0B = 0;

    //turn on CTC mode
    TCCR0A |= (1 << WGM01);

    //set prescaler to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);

    //set compare match register to desired timer count
    OCR0A = 124; //500uS

    //encoder read ISR
    OCR3A = 10;

    //enable CTC interrupt for timer0
    TIMSK0 |= (1 << OCIE0A);

    TIMSK3 |= (1<<OCIE3A);

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

ISR(TIMER0_COMPA_vect)    {

    static uint8_t ledUpdateCounter = 0;

    if (ledUpdateCounter == 2)  {

        ledUpdateCounter = 0;

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

            ledStateSingle = ledState[ledNumber];
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

    ledUpdateCounter++;

    static bool updateMillis = false;

    if (updateMillis)   {

        uint32_t ms;
        ms = rTime_ms;
        ms++;
        //update run time
        rTime_ms = ms;

        for (int i=0; i<NUMBER_OF_LEDS; i++)    {

            if (bitRead(ledBlinking, i))    {

                ledBlinkTimer[i]++;

                if (ledBlinkTimer[i] == LED_BLINK_TIME)    {

                    if (ledState[i] == LED_INTENSITY_FULL)
                        ledState[i] = LED_INTENSITY_OFF;
                    else ledState[i] = LED_INTENSITY_FULL;
                        ledBlinkTimer[i] = 0;

                }

            }

        }

    }   updateMillis = !updateMillis;

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

ISR(TIMER3_COMPA_vect)  {

    static uint8_t encoderCounter = 0;
    updateEncoder(encoderCounter);
    encoderCounter++;
    if (encoderCounter == NUMBER_OF_ENCODERS)
    encoderCounter = 0;

}