#include "Board.h"

volatile int8_t      encoderMoving[NUMBER_OF_ENCODERS];
uint16_t             encoderData[NUMBER_OF_ENCODERS];
volatile uint32_t    encoderBuffer[NUMBER_OF_ENCODERS];

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

void Board::initEncoders()  {

    for (int i=0; i<NUMBER_OF_ENCODERS; i++)    {

        setInput(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setInput(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        setHigh(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setHigh(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        encoderData[i] |= ((uint16_t)0 << 8);
        encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);   //set number of pulses to 8

    }

}

inline void updateEncoder(uint8_t encoderID)    {

    //add new data
    uint8_t newPairData = 0;
    //newPairData |= (((encoderData[encoderID] << 2) & 0x000F) | (uint16_t)pairState);
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

int8_t Board::getEncoderState(uint8_t encoderNumber)  {

    int8_t returnValue;
    returnValue = encoderMoving[encoderNumber];
    encoderMoving[encoderNumber] = 0;
    return returnValue;

}