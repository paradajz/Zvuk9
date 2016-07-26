#include "TimerObject.h"
#include "../../interface/leds/LEDsettings.h"
#include "../../interface/encoders/EncoderSettings.h"
#include "../../vserial/Serial.h"
#include "../../Debug.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3

//LEDs
volatile int8_t                     activeColumnInterrupt = 0;
uint8_t                             ledState[NUMBER_OF_LEDS];
int16_t                             transitionCounter[NUMBER_OF_LEDS];
volatile uint8_t                    pwmSteps = DEFAULT_FADE_SPEED;
uint32_t                            ledBlinking = 0;
uint16_t                            ledBlinkTimer[NUMBER_OF_LEDS];

//encoders
volatile int8_t                     encoderMoving[NUMBER_OF_ENCODERS];
uint16_t                            encoderData[NUMBER_OF_ENCODERS];

volatile uint32_t rTime_ms;

const uint8_t encoderPinIndex1Array[] = {

    ENCODER_PAIR_00_PIN_0_INDEX,
    ENCODER_PAIR_01_PIN_0_INDEX,
    ENCODER_PAIR_02_PIN_0_INDEX,
    ENCODER_PAIR_03_PIN_0_INDEX,
    ENCODER_PAIR_04_PIN_0_INDEX,
    ENCODER_PAIR_05_PIN_0_INDEX,
    ENCODER_PAIR_06_PIN_0_INDEX,
    ENCODER_PAIR_07_PIN_0_INDEX,
    ENCODER_PAIR_08_PIN_0_INDEX,
    ENCODER_PAIR_09_PIN_0_INDEX

};

const uint8_t encoderPinIndex2Array[] = {

    ENCODER_PAIR_00_PIN_1_INDEX,
    ENCODER_PAIR_01_PIN_1_INDEX,
    ENCODER_PAIR_02_PIN_1_INDEX,
    ENCODER_PAIR_03_PIN_1_INDEX,
    ENCODER_PAIR_04_PIN_1_INDEX,
    ENCODER_PAIR_05_PIN_1_INDEX,
    ENCODER_PAIR_06_PIN_1_INDEX,
    ENCODER_PAIR_07_PIN_1_INDEX,
    ENCODER_PAIR_08_PIN_1_INDEX,
    ENCODER_PAIR_09_PIN_1_INDEX

};

volatile uint8_t *encoderPin1Array[] = {

    &ENCODER_PAIR_00_PIN_0_PIN,
    &ENCODER_PAIR_01_PIN_0_PIN,
    &ENCODER_PAIR_02_PIN_0_PIN,
    &ENCODER_PAIR_03_PIN_0_PIN,
    &ENCODER_PAIR_04_PIN_0_PIN,
    &ENCODER_PAIR_05_PIN_0_PIN,
    &ENCODER_PAIR_06_PIN_0_PIN,
    &ENCODER_PAIR_07_PIN_0_PIN,
    &ENCODER_PAIR_08_PIN_0_PIN,
    &ENCODER_PAIR_09_PIN_0_PIN

};

volatile uint8_t *encoderPin2Array[] = {

    &ENCODER_PAIR_00_PIN_1_PIN,
    &ENCODER_PAIR_01_PIN_1_PIN,
    &ENCODER_PAIR_02_PIN_1_PIN,
    &ENCODER_PAIR_03_PIN_1_PIN,
    &ENCODER_PAIR_04_PIN_1_PIN,
    &ENCODER_PAIR_05_PIN_1_PIN,
    &ENCODER_PAIR_06_PIN_1_PIN,
    &ENCODER_PAIR_07_PIN_1_PIN,
    &ENCODER_PAIR_08_PIN_1_PIN,
    &ENCODER_PAIR_09_PIN_1_PIN

};

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

inline void updateEncoder(uint8_t encoderID)    {

    uint8_t p1val = (*encoderPin1Array[encoderID] >> encoderPinIndex1Array[encoderID]) & 0x01;
    uint8_t p2val = (*encoderPin2Array[encoderID] >> encoderPinIndex2Array[encoderID]) & 0x01;

    //uint8_t p1val = 0;
    //uint8_t p2val = 0;

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

inline void ledRowsOff() {

    //turn off pwm
    TCCR2A &= ~(1<<COM2A1);
    TCCR1A &= ~(1<<COM1A1);
    TCCR1A &= ~(1<<COM1B1);

    setLowMacro(LED_ROW_1_PORT, LED_ROW_3_PIN_INDEX);
    setLowMacro(LED_ROW_2_PORT, LED_ROW_3_PIN_INDEX);
    setLowMacro(LED_ROW_3_PORT, LED_ROW_3_PIN_INDEX);

}

inline void activateColumn(uint8_t column) {

    bitRead(column, 0) ? setHighMacro(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX) : setLowMacro(DECODER_OUT_1_PORT, DECODER_OUT_1_PIN_INDEX);
    bitRead(column, 1) ? setHighMacro(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX) : setLowMacro(DECODER_OUT_2_PORT, DECODER_OUT_2_PIN_INDEX);
    bitRead(column, 2) ? setHighMacro(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX) : setLowMacro(DECODER_OUT_3_PORT, DECODER_OUT_3_PIN_INDEX);

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

//timer0 - delay/millis/micros/encoders
//timer1 - LEDs (pwm)
//timer2 - LEDs (pwm)
//timer3 - LED matrix

ISR(TIMER0_COMPA_vect)    {

    #ifdef MODULE_LEDS

        //static uint8_t ledUpdateCounter = 0;

        //if (ledUpdateCounter == 2)  {

                //ledUpdateCounter = 0;

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

        //}

        //ledUpdateCounter++;

    #endif

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

ISR(TIMER3_COMPA_vect)  {

    #ifdef MODULE_ENCODERS
        static uint8_t encoderCounter = 0;
        updateEncoder(encoderCounter);
        encoderCounter++;
        if (encoderCounter == NUMBER_OF_ENCODERS)
        encoderCounter = 0;
    #endif

}

ledState_t TimerObject::getLEDstate(uint8_t ledNumber)  {

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

void TimerObject::setLEDstate(uint8_t ledNumber, ledState_t state) {

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

TimerObject::TimerObject()  {

    //default constructor

}

void TimerObject::init() {

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

    OCR3A = 10;

    //enable CTC interrupt for timer0
    TIMSK0 |= (1 << OCIE0A);

    TIMSK3 |= (1<<OCIE3A);

    #ifdef MODULE_ENCODERS

        volatile uint8_t *encoderPort1Array[] = {

            &ENCODER_PAIR_00_PIN_0_PORT,
            &ENCODER_PAIR_01_PIN_0_PORT,
            &ENCODER_PAIR_02_PIN_0_PORT,
            &ENCODER_PAIR_03_PIN_0_PORT,
            &ENCODER_PAIR_04_PIN_0_PORT,
            &ENCODER_PAIR_05_PIN_0_PORT,
            &ENCODER_PAIR_06_PIN_0_PORT,
            &ENCODER_PAIR_07_PIN_0_PORT,
            &ENCODER_PAIR_08_PIN_0_PORT,
            &ENCODER_PAIR_09_PIN_0_PORT

        };

        volatile uint8_t *encoderPort2Array[] = {

            &ENCODER_PAIR_00_PIN_1_PORT,
            &ENCODER_PAIR_01_PIN_1_PORT,
            &ENCODER_PAIR_02_PIN_1_PORT,
            &ENCODER_PAIR_03_PIN_1_PORT,
            &ENCODER_PAIR_04_PIN_1_PORT,
            &ENCODER_PAIR_05_PIN_1_PORT,
            &ENCODER_PAIR_06_PIN_1_PORT,
            &ENCODER_PAIR_07_PIN_1_PORT,
            &ENCODER_PAIR_08_PIN_1_PORT,
            &ENCODER_PAIR_09_PIN_1_PORT

        };

        volatile uint8_t *encoderDDR1Array[] = {

            &ENCODER_PAIR_00_PIN_0_DDR,
            &ENCODER_PAIR_01_PIN_0_DDR,
            &ENCODER_PAIR_02_PIN_0_DDR,
            &ENCODER_PAIR_03_PIN_0_DDR,
            &ENCODER_PAIR_04_PIN_0_DDR,
            &ENCODER_PAIR_05_PIN_0_DDR,
            &ENCODER_PAIR_06_PIN_0_DDR,
            &ENCODER_PAIR_07_PIN_0_DDR,
            &ENCODER_PAIR_08_PIN_0_DDR,
            &ENCODER_PAIR_09_PIN_0_DDR

        };

        volatile uint8_t *encoderDDR2Array[] = {

            &ENCODER_PAIR_00_PIN_1_DDR,
            &ENCODER_PAIR_01_PIN_1_DDR,
            &ENCODER_PAIR_02_PIN_1_DDR,
            &ENCODER_PAIR_03_PIN_1_DDR,
            &ENCODER_PAIR_04_PIN_1_DDR,
            &ENCODER_PAIR_05_PIN_1_DDR,
            &ENCODER_PAIR_06_PIN_1_DDR,
            &ENCODER_PAIR_07_PIN_1_DDR,
            &ENCODER_PAIR_08_PIN_1_DDR,
            &ENCODER_PAIR_09_PIN_1_DDR

        };


        for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

            setInputMacro(*(encoderDDR1Array[i]), encoderPinIndex1Array[i]);
            setInputMacro(*(encoderDDR2Array[i]), encoderPinIndex2Array[i]);

            setHighMacro(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
            setHighMacro(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

            encoderData[i] |= ((uint16_t)0 << 8);
            encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);   //set number of pulses to 8

        }
    #endif

};

int8_t TimerObject::getEncoderState(uint8_t encoderNumber)  {

    int8_t returnValue;
    returnValue = encoderMoving[encoderNumber];
    encoderMoving[encoderNumber] = 0;
    return returnValue;

}

void TimerObject::setPWMsteps(uint8_t steps)    {

    pwmSteps = steps;

}

uint32_t rTimeMillis()    {

    uint32_t _rTime_mS;

    //save interrupt flag
    uint8_t interruptFlag = SREG;

    //disable interrupts
    cli();

    _rTime_mS = rTime_ms;

    //restore interruptFlag
    SREG = interruptFlag;

    return _rTime_mS;

}

void wait(uint32_t delayTime)    {

    for (uint32_t i=0; i<delayTime; i++)
    _delay_ms(1);

}

TimerObject timers;
