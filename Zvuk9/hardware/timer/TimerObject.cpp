#include "TimerObject.h"
#include "interface/leds/LEDsettings.h"
#include "interface/encoders/EncoderSettings.h"
#include <util/delay.h>

#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3

#define ENCODER_READ_PER_INTERRUPT  (NUMBER_OF_ENCODERS/2)

#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

//LEDs
volatile int8_t             activeColumnInterrupt = 0;
ledIntensity                ledState[NUMBER_OF_LEDS] = { ledIntensityOff };
volatile int16_t            transitionCounter[NUMBER_OF_LEDS] = { 0 };
volatile uint8_t            pwmSteps = DEFAULT_FADE_SPEED;

//encoders
volatile encoderPosition    encoderMoving[NUMBER_OF_ENCODERS];
volatile uint8_t*           pin1_register[NUMBER_OF_ENCODERS];
volatile uint8_t*           pin2_register[NUMBER_OF_ENCODERS];
uint8_t                     pin1_bitmask[NUMBER_OF_ENCODERS];
uint8_t                     pin2_bitmask[NUMBER_OF_ENCODERS];
uint8_t                     state[NUMBER_OF_ENCODERS];
int32_t                     position[NUMBER_OF_ENCODERS];

//timer1 testing
volatile uint32_t rTime_ms;

const uint8_t encoderPin1Array[] = {

    ENCODER_PAIR_00_PIN_0,
    ENCODER_PAIR_01_PIN_0,
    ENCODER_PAIR_02_PIN_0,
    ENCODER_PAIR_03_PIN_0,
    ENCODER_PAIR_04_PIN_0,
    ENCODER_PAIR_05_PIN_0,
    ENCODER_PAIR_06_PIN_0,
    ENCODER_PAIR_07_PIN_0,
    ENCODER_PAIR_08_PIN_0,
    ENCODER_PAIR_09_PIN_0

};

const uint8_t encoderPin2Array[] = {

    ENCODER_PAIR_00_PIN_1,
    ENCODER_PAIR_01_PIN_1,
    ENCODER_PAIR_02_PIN_1,
    ENCODER_PAIR_03_PIN_1,
    ENCODER_PAIR_04_PIN_1,
    ENCODER_PAIR_05_PIN_1,
    ENCODER_PAIR_06_PIN_1,
    ENCODER_PAIR_07_PIN_1,
    ENCODER_PAIR_08_PIN_1,
    ENCODER_PAIR_09_PIN_1

};

inline void updateEncoder(uint8_t id)    {

    uint8_t p1val = DIRECT_PIN_READ(pin1_register[id], pin1_bitmask[id]);
    uint8_t p2val = DIRECT_PIN_READ(pin2_register[id], pin2_bitmask[id]);

    uint8_t newState = state[id] & 3;
    int32_t lastPosition = position[id];

    if (p1val) newState |= 4;
    if (p2val) newState |= 8;

    state[id] = (newState >> 2);

    switch (newState) {

        case 1: case 7: case 8: case 14:
        position[id]++;
        break;

        case 2: case 4: case 11: case 13:
        position[id]--;
        break;

        case 3: case 12:
        position[id] += 2;
        break;

        case 6: case 9:
        position[id] -= 2;
        break;

    }

    if (position[id] != lastPosition)    {

        bool encoderDirection = position[id] > lastPosition;

        if ((position[id] % PULSES_PER_STEP) == 0)   {

            if (encoderDirection) encoderMoving[id] = encMoveRight;
            else encoderMoving[id] = encMoveLeft;

        }

    }

}

inline void ledRowsOff() {

    digitalWrite(LED_ROW_1_PIN, LOW);
    digitalWrite(LED_ROW_2_PIN, LOW);
    digitalWrite(LED_ROW_3_PIN, LOW);

}

inline void activateColumn(uint8_t column) {

    digitalWrite(DECODER_OUT_1, bitRead(column, 0));
    digitalWrite(DECODER_OUT_2, bitRead(column, 1));
    digitalWrite(DECODER_OUT_3, bitRead(column, 2));

}

inline void ledRowOn(uint8_t rowNumber, uint8_t value)  {

    switch (rowNumber)  {

        case 0:
        analogWrite(LED_ROW_1_PIN, value);
        break;

        case 1:
        analogWrite(LED_ROW_2_PIN, value);
        break;

        case 2:
        analogWrite(LED_ROW_3_PIN, value);
        break;

    }

}

//timer0 - delay/millis/micros/encoders
//timer1 - LEDs (pwm)
//timer2 - LEDs (pwm)
//timer3 - LED matrix

ISR(TIMER0_COMPA_vect)    {

    static bool updateMillis = false;

    if (updateMillis)   {

        uint32_t ms;
        ms = rTime_ms;
        ms++;
        //update run time
        rTime_ms = ms;

    }   updateMillis = !updateMillis;

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
        updateEncoder(i);

}

ISR(TIMER3_COMPA_vect)  {

    //LEDs
    ledRowsOff();
    activateColumn(activeColumnInterrupt);

    for (int i=0; i<NUMBER_OF_LED_ROWS; i++)  {

        uint8_t ledNumber = activeColumnInterrupt+i*NUMBER_OF_LED_COLUMNS;
        ledIntensity ledStateSingle = ledState[ledNumber];

        if (transitionCounter[ledNumber]) ledRowOn(i, transitionCounter[ledNumber]);
        if (transitionCounter[ledNumber] < ledStateSingle)
        transitionCounter[ledNumber] += pwmSteps;
        if (transitionCounter[ledNumber] > 255) transitionCounter[ledNumber] = 255;

        if ((ledStateSingle == 0) || (ledStateSingle == ledIntensityDim))
        transitionCounter[ledNumber] -= pwmSteps;

        if ((ledStateSingle == ledIntensityDim) && (transitionCounter[ledNumber] < ledIntensityDim))
        transitionCounter[ledNumber] = ledIntensityDim;
        if (transitionCounter[ledNumber] < 0) transitionCounter[ledNumber] = 0;

    }

    activeColumnInterrupt++;
    if (activeColumnInterrupt == NUMBER_OF_LED_COLUMNS) activeColumnInterrupt = 0;

}

ledIntensity TimerObject::getLEDstate(uint8_t ledNumber)  {

    return ledState[ledNumber];

}

void TimerObject::setLEDstate(uint8_t ledNumber, ledIntensity intensity) {

    cli();
    ledState[ledNumber] = intensity;
    sei();

}

TimerObject::TimerObject()  {

    //default constructor

}

void TimerObject::init() {

    cli();

    //configure timer3
    //used for led control
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;

    //turn on CTC mode
    TCCR3B |= (1 << WGM32);

    //set prescaler to 256
    TCCR3B |= (1 << CS32);

    //set compare match register to desired timer count
    OCR3A = 94; //around 1500us

    //set PWM frequency to 31kHz
    TCCR1B = (TCCR1B & 0b11111000) | 0x01;
    TCCR2B = (TCCR2B & 0b11111000) | 0x01;

    //configure timer0
    //used for delay/millis/encoders
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;

    //turn on CTC mode
    TCCR0A |= (1 << WGM01);

    //set prescaler to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);

    //set compare match register to desired timer count
    OCR0A = 124; //500uS


    //enable CTC interrupt for timer3
    TIMSK3 |= (1 << OCIE3A);

    //enable CTC interrupt for timer0
    TIMSK0 |= (1 << OCIE0A);

    sei();

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
        setupEncoder(i, encoderPin1Array[i], encoderPin2Array[i]);

};

encoderPosition TimerObject::getEncoderState(uint8_t encoderNumber)  {

    encoderPosition returnValue;
    returnValue = encoderMoving[encoderNumber];
    encoderMoving[encoderNumber] = encStopped;
    return returnValue;

}

void TimerObject::setPWMsteps(uint8_t steps)    {

    cli();
    pwmSteps = steps;
    //for (int i=0; i<NUMBER_OF_LEDS; i++)
        //transitionCounter[i] = 0;
    sei();

}

void TimerObject::setupEncoder(uint8_t id, uint8_t pin1, uint8_t pin2) {

    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);

    pin1_register[id] = PIN_TO_BASEREG(pin1);
    pin1_bitmask[id] = PIN_TO_BITMASK(pin1);

    pin2_register[id] = PIN_TO_BASEREG(pin2);
    pin2_bitmask[id] = PIN_TO_BITMASK(pin2);

    position[id] = 0;

    //allow time for a passive R-C filter to charge
    //through the pullup resistors, before reading
    //the initial state
    _delay_ms(2);

    uint8_t s = 0;

    if (DIRECT_PIN_READ(pin1_register[id], pin1_bitmask[id])) s |= 1;
    if (DIRECT_PIN_READ(pin2_register[id], pin2_bitmask[id])) s |= 2;

    state[id] = s;

}

uint32_t newMillis()    {

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

void newDelay(uint32_t delayTime)    {

    for (uint32_t i=0; i<delayTime; i++)
    _delay_ms(1);

}

TimerObject timers;