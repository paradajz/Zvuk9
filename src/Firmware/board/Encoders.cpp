#include "Board.h"

uint16_t            encoderData[NUMBER_OF_ENCODERS];
volatile uint8_t    encoderBuffer[NUMBER_OF_ENCODERS];

#ifdef BOARD_R1
volatile uint8_t *encoderPort1Array[] =
{
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

volatile uint8_t *encoderPort2Array[] =
{
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
#endif

#ifdef BOARD_R1
void Board::initEncoders()
{
    for (int i=0; i<NUMBER_OF_ENCODERS; i++)
    {
        setInput(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setInput(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        setHigh(*(encoderPort1Array[i]), encoderPinIndex1Array[i]);
        setHigh(*(encoderPort2Array[i]), encoderPinIndex2Array[i]);

        encoderData[i] |= ((uint16_t)0 << 8);
        encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);   //set number of pulses to 8
    }
}
#endif

inline int8_t readEncoder(uint8_t encoderID, uint8_t pairState)
{
    //generate new data
    uint8_t newPairData = 0;
    newPairData |= (((encoderData[encoderID] << 2) & 0x000F) | (uint16_t)pairState);

    //remove old data
    encoderData[encoderID] &= ENCODER_CLEAR_TEMP_STATE_MASK;

    //shift in new data
    encoderData[encoderID] |= (uint16_t)newPairData;

    int8_t encRead = encoderLookUpTable[newPairData];

    if (!encRead)
        return 0;

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

    if (lastEncoderDirection != newEncoderDirection)
        return 0;

    if (currentPulses % PULSES_PER_STEP)
        return 0;

    //clear current pulses
    encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;

    //set default pulse count
    encoderData[encoderID] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);

    if (newEncoderDirection)
        return 1;
    else
        return -1;
}

int8_t Board::getEncoderState(uint8_t encoderNumber)
{
    uint8_t tempBuffer = encoderBuffer[encoderNumber];
    return readEncoder(encoderNumber, tempBuffer);
}