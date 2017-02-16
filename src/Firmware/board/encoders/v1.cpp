#include "../Board.h"

#ifdef BOARD_R1
volatile uint8_t    encoderBuffer[MAX_NUMBER_OF_ENCODERS];

volatile uint8_t *encoderPort1Array[] =
{
    &ENCODER_00_PIN_0_PORT,
    &ENCODER_01_PIN_0_PORT,
    &ENCODER_02_PIN_0_PORT,
    &ENCODER_03_PIN_0_PORT,
    &ENCODER_04_PIN_0_PORT,
    &ENCODER_05_PIN_0_PORT,
    &ENCODER_06_PIN_0_PORT,
    &ENCODER_07_PIN_0_PORT,
    &ENCODER_08_PIN_0_PORT,
    &ENCODER_09_PIN_0_PORT
};

volatile uint8_t *encoderPort2Array[] =
{
    &ENCODER_00_PIN_1_PORT,
    &ENCODER_01_PIN_1_PORT,
    &ENCODER_02_PIN_1_PORT,
    &ENCODER_03_PIN_1_PORT,
    &ENCODER_04_PIN_1_PORT,
    &ENCODER_05_PIN_1_PORT,
    &ENCODER_06_PIN_1_PORT,
    &ENCODER_07_PIN_1_PORT,
    &ENCODER_08_PIN_1_PORT,
    &ENCODER_09_PIN_1_PORT
};

int8_t Board::getEncoderState(uint8_t encoderNumber)
{
    uint8_t tempBuffer = encoderBuffer[encoderNumber];
    return readEncoder(encoderNumber, tempBuffer);
}
#endif