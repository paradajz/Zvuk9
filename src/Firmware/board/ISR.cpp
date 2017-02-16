#include "Board.h"

volatile uint32_t rTime_ms;

const uint8_t ledRowPinArray[] =
{
    LED_ROW_1_PIN,
    LED_ROW_2_PIN,
    LED_ROW_3_PIN
};

volatile uint8_t *ledRowPortArray[] =
{
    &LED_ROW_1_PORT,
    &LED_ROW_2_PORT,
    &LED_ROW_3_PORT
};

void Board::initTimers()
{
    //configure timer3
    //used for led matrix, millis and encoder readout
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
    OCR3A = 249; //1ms

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

    //clear timer0 conf
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK0 = 0;
    OCR0A = 0;
}

inline void checkLEDs()
{
    if (blinkEnabled)
    {
        if (!blinkTimerCounter)
        {
            //change blinkBit state and write it into ledState variable if LED is in blink state
            for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
            {
                if (bitRead(ledState[i], LED_BLINK_ON_BIT))
                {
                    if (blinkState)
                        bitSet(ledState[i], LED_BLINK_STATE_BIT);
                    else
                        bitClear(ledState[i], LED_BLINK_STATE_BIT);
                }
            }

            blinkState = !blinkState;
        }
    }

    //if there is an active LED in current column, turn on LED row
    //do fancy transitions here
    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)
    {
        uint8_t ledNumber = activeLEDcolumn+i*NUMBER_OF_LED_COLUMNS;
        uint8_t ledStateSingle = bitRead(ledState[ledNumber], LED_ACTIVE_BIT) && (bitRead(ledState[ledNumber], LED_BLINK_ON_BIT) == bitRead(ledState[ledNumber], LED_BLINK_STATE_BIT));

        if (ledStateSingle)
        {
            if (bitRead(ledState[ledNumber], LED_INTENSITY_BIT))
                ledStateSingle = LED_FULL_INTENSITY;
            else
                ledStateSingle = LED_HALF_INTENSITY;
        }

        //don't bother with pwm if it's disabled
        if (!pwmSteps && ledStateSingle)
        {
            ledRowOn(i, ledTransitionScale[ledStateSingle]);
        }
        else
        {
            if (ledTransitionScale[transitionCounter[ledNumber]])
                ledRowOn(i, ledTransitionScale[transitionCounter[ledNumber]]);

            if (transitionCounter[ledNumber] != ledStateSingle)
            {
                if(transitionCounter[ledNumber] < ledStateSingle)
                {
                    //fade up
                    transitionCounter[ledNumber] += pwmSteps;

                    if (transitionCounter[ledNumber] > ledStateSingle)
                        transitionCounter[ledNumber] = ledStateSingle;
                }
                else
                {
                    //fade down
                    transitionCounter[ledNumber] -= pwmSteps;

                    if (transitionCounter[ledNumber] < 0)
                        transitionCounter[ledNumber] = 0;
                }
            }
        }
    }
}

inline void ledRowOn(uint8_t rowNumber, uint8_t intensity)
{
    if (intensity == 255)
    {
        //max value, don't use pwm
        #ifdef LED_INVERT
        setLow(*(ledRowPortArray[rowNumber]), ledRowPinArray[rowNumber]);
        #else
        setHigh(*(ledRowPortArray[rowNumber]), ledRowPinArray[rowNumber]);
        #endif
        return;
    }

    #ifdef LED_INVERT
    intensity = 255 - intensity;
    #endif

    switch (rowNumber)
    {
        //turn off pwm if intensity is max
        case 0:
        OCR2A = intensity;
        TCCR2A |= (1<<COM2A1);
        break;

        case 1:
        OCR1A  = intensity;
        TCCR1A |= (1<<COM1A1);
        break;

        case 2:
        OCR1B = intensity;
        TCCR1A |= (1<<COM1B1);
        break;

        default:
        break;
    }
}

inline void ledRowsOff()
{
    //turn off pwm
    TCCR2A &= ~(1<<COM2A1);
    TCCR1A &= ~(1<<COM1A1);
    TCCR1A &= ~(1<<COM1B1);

    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)
    {
        #ifdef LED_INVERT
        setHigh(*(ledRowPortArray[i]), ledRowPinArray[i]);
        #else
        setLow(*(ledRowPortArray[i]), ledRowPinArray[i]);
        #endif
    }
}

inline void activateOutputColumn()
{
    //clear current decoder state
    DECODER_OUT_PORT &= DECODER_OUT_CLEAR_MASK;
    //activate new column
    DECODER_OUT_PORT |= decoderOutOrderArray[activeLEDcolumn];

    _NOP();
}

#ifdef BOARD_R2
inline void activateInputColumn(uint8_t column)
{
    //clear current decoder state
    DECODER_IN_PORT &= DECODER_IN_CLEAR_MASK;
    //activate new column
    DECODER_IN_PORT |= decoderInOrderArray[column];

    _NOP();
}

inline void storeDigitalIn(uint8_t column, uint8_t bufferIndex)
{
    uint8_t data = 0;

    //make room for new data
    inputBuffer[bufferIndex] <<= 8;

    //pulse latch pin
    pulseLowToHigh(INPUT_SHIFT_REG_LATCH_PORT, INPUT_SHIFT_REG_LATCH_PIN);

    for (int i=0; i<8; i++)
    {
        data <<= 1;
        data |= readPin(INPUT_SHIFT_REG_IN_PORT, INPUT_SHIFT_REG_IN_PIN);
        //pulse clock pin
        pulseHightToLow(INPUT_SHIFT_REG_CLOCK_PORT, INPUT_SHIFT_REG_CLOCK_PIN);
    }

    inputBuffer[bufferIndex] |= (uint64_t)data;
}
#endif

ISR(TIMER3_COMPA_vect)
{
    ledRowsOff();

    if (activeLEDcolumn == NUMBER_OF_LED_COLUMNS)
        activeLEDcolumn = 0;

    activateOutputColumn();
    checkLEDs();
    activeLEDcolumn++;
    blinkTimerCounter++;

    if (blinkTimerCounter >= ledBlinkTime)
        blinkTimerCounter = 0;

    uint32_t ms;
    ms = rTime_ms;
    ms++;
    //update run time
    rTime_ms = ms;

    #ifdef BOARD_R1
    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        bitWrite(encoderBuffer[i], 1, readPin(*encoderPort1Array[i], encoderPinIndex1Array[i]));
        bitWrite(encoderBuffer[i], 0, readPin(*encoderPort2Array[i], encoderPinIndex2Array[i]));
    }
    #elif defined (BOARD_R2)
    //read input matrix
    uint8_t bufferIndex = digital_buffer_head + 1;

    if (bufferIndex >= DIGITAL_BUFFER_SIZE)
        bufferIndex = 0;

    if (digital_buffer_tail == bufferIndex)
        return; //buffer full, exit

    inputBuffer[bufferIndex] = 0;
    digital_buffer_head = bufferIndex;

    for (int i=0; i<NUMBER_OF_BUTTON_COLUMNS; i++)
    {
        activateInputColumn(i);
        storeDigitalIn(i, bufferIndex);
    }
    #endif
}