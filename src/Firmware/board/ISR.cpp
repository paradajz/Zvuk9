#include "Board.h"

//timer0 - delay/millis/micros/encoders
//timer1 - LEDs (pwm)
//timer2 - LEDs (pwm)
//timer3 - LED matrix

void Board::initTimers()    {

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

ISR(TIMER3_COMPA_vect)  {

    //turn off pwm
    TCCR2A &= ~(1<<COM2A1);
    TCCR1A &= ~(1<<COM1A1);
    TCCR1A &= ~(1<<COM1B1);

    setLow(LED_ROW_1_PORT, LED_ROW_1_PIN_INDEX);
    setLow(LED_ROW_2_PORT, LED_ROW_2_PIN_INDEX);
    setLow(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

    if (activeColumnInterrupt == NUMBER_OF_LED_COLUMNS)
        activeColumnInterrupt = 0;

    bitRead(activeColumnInterrupt, 0) ? setHigh(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX) : setLow(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    bitRead(activeColumnInterrupt, 1) ? setHigh(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX) : setLow(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    bitRead(activeColumnInterrupt, 2) ? setHigh(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX) : setLow(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

    uint8_t ledNumber;

    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)  {

        ledNumber = activeColumnInterrupt+i*NUMBER_OF_LED_COLUMNS;

        if (transitionCounter[ledNumber])   {

            switch (i)  {

                case 0:
                OCR2A = transitionCounter[ledNumber];
                TCCR2A |= (1<<COM2A1);
                break;

                case 1:
                OCR1A = transitionCounter[ledNumber];
                TCCR1A |= (1<<COM1A1);
                break;

                case 2:
                OCR1B = transitionCounter[ledNumber];
                TCCR1A |= (1<<COM1B1);
                break;

            }

        }

        if (transitionCounter[ledNumber] != ledValue[ledNumber]) {

            if (transitionCounter[ledNumber] < ledValue[ledNumber]) {

                transitionCounter[ledNumber] += pwmSteps;
                if (transitionCounter[ledNumber] > ledValue[ledNumber])
                    transitionCounter[ledNumber] = ledValue[ledNumber];

            }   else {

                transitionCounter[ledNumber] -= pwmSteps;
                if (transitionCounter[ledNumber] < ledValue[ledNumber])
                    transitionCounter[ledNumber] = ledValue[ledNumber];

            }

        }

    }

    activeColumnInterrupt++;

    uint32_t ms;
    ms = rTime_ms;
    ms++;
    //update run time
    rTime_ms = ms;

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        bitWrite(encoderBuffer[i], 1, readPin(*encoderPort1Array[i], encoderPinIndex1Array[i]));
        bitWrite(encoderBuffer[i], 0, readPin(*encoderPort2Array[i], encoderPinIndex2Array[i]));

    }

}