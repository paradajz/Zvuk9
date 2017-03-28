#include "../Board.h"

uint8_t             encoderState[CONNECTED_ENCODERS];
int8_t              encPulses_x4[CONNECTED_ENCODERS];
volatile int8_t     encPulses[CONNECTED_ENCODERS];

void Board::initEncoders()
{
    for (int i=0; i<MAX_NUMBER_OF_ENCODERS; i++)
    {
        //encoderData[i] |= ((uint16_t)0 << 8);
        //set number of pulses to 8
        //encoderData[i] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);
    }

    #ifdef BOARD_R2
    //encoderInverted[PROGRAM_ENCODER] = true;
    //encoderInverted[X_MAX_ENCODER] = true;
    //encoderInverted[Y_MAX_ENCODER] = true;
    //encoderInverted[X_CURVE_ENCODER] = true;
    //encoderInverted[Y_CURVE_ENCODER] = true;
    #endif
}

int8_t Board::readEncoder(uint8_t encoderID, uint8_t pairState)
{
    //generate new data
    //uint8_t newPairData = 0;
    //newPairData |= (((encoderData[encoderID] << 2) & 0x000F) | (uint16_t)pairState);
//
    ////remove old data
    //encoderData[encoderID] &= ENCODER_CLEAR_TEMP_STATE_MASK;
//
    ////shift in new data
    //encoderData[encoderID] |= (uint16_t)newPairData;
//
    //int8_t encRead = encoderLookUpTable[newPairData];
//
    //if (!encRead)
        //return 0;
//
    //bool newEncoderDirection = encRead > 0;
    ////get current number of pulses from encoderData
    //int8_t currentPulses = (encoderData[encoderID] >> 4) & 0x000F;
    //currentPulses += encRead;
    ////clear current pulses
    //encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;
    ////shift in new pulse count
    //encoderData[encoderID] |= (uint16_t)(currentPulses << 4);
    ////get last encoder direction
    //bool lastEncoderDirection = bitRead(encoderData[encoderID], ENCODER_DIRECTION_BIT);
    ////write new encoder direction
    //bitWrite(encoderData[encoderID], ENCODER_DIRECTION_BIT, newEncoderDirection);
//
    //if (lastEncoderDirection != newEncoderDirection)
        //return 0;
//
    //if (currentPulses % PULSES_PER_STEP)
        //return 0;
//
    ////clear current pulses
    //encoderData[encoderID] &= ENCODER_CLEAR_PULSES_MASK;
//
    ////set default pulse count
    //encoderData[encoderID] |= ((uint16_t)ENCODER_DEFAULT_PULSE_COUNT_STATE << 4);
//
    //if (newEncoderDirection)
        //return 1;
    //else
        return -1;
}