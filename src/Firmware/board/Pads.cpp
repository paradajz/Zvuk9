#include "Board.h"
#include "../interface/pads/DataTypes.h"

const uint8_t       padIDArray[] =
{
    PAD_0,
    PAD_1,
    PAD_2,
    PAD_3,
    PAD_4,
    PAD_5,
    PAD_6,
    PAD_7,
    PAD_8
};

const uint8_t             adcPinReadOrder_board[] =
{
    MUX_COMMON_PIN_0_PIN, //pressure, first reading
    MUX_COMMON_PIN_1_PIN, //pressure, second reading
    MUX_COMMON_PIN_0_PIN, //x coordinate
    MUX_COMMON_PIN_2_PIN  //y coordinate
};

volatile uint16_t   coordinateSamples;
uint8_t             sampleCounter;

//three coordinates
volatile int16_t    extractedSamples[3][NUMBER_OF_PADS];

uint8_t             activePad;
uint8_t             padReadingIndex;

void                (*valueSetup[PAD_READINGS]) (void);

inline void setMuxInput(uint8_t muxInput)
{
    bitRead(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    bitRead(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    bitRead(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    bitRead(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);
}

void setupPressure1()
{
    //To read force (Z-axis), apply a voltage a voltage from one X conductor to one Y conductor, then read voltages at the other X and Y conductors
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);      //X+
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);     //X-
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);      //Y+
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);     //Y-

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);        //X+
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);        //X-
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);        //Y+
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);       //Y-
}

void setupPressure2()
{
    //To read force (Z-axis), apply a voltage a voltage from one X conductor to one Y conductor, then read voltages at the other X and Y conductors
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);     //X+
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);      //X-
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);     //Y+
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);      //Y-

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);        //X+
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);        //X-
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);       //Y+
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);        //Y-
}

void setupX()
{
    //To read X position, apply a voltage across the X-plane, and read the voltage from either Y conductor.
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);
}

void setupY()
{
    //To read Y position, apply a voltage across the Y-plane, and read the voltage from either X conductor.
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);
}

void Board::initPads()
{
    setupPressure1();
    setADCchannel(adcPinReadOrder_board[0]);
    setMuxInput(0);

    //place setup z/x/y in function pointer for simpler access in isr
    valueSetup[readPressure0] = setupPressure1;
    valueSetup[readPressure1] = setupPressure2;
    valueSetup[readX] = setupX;
    valueSetup[readY] = setupY;

    for (int i=0; i<3; i++)
    {
        for (int k=0; k<NUMBER_OF_PADS; k++)
        {
            extractedSamples[i][k] = -1;
        }
    }

    adcInterruptEnable();
}

uint16_t Board::getPadPressure(uint8_t pad)
{
    int16_t returnValue;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue = extractedSamples[coordinateZ][pad];
    }

    if (returnValue == -1)
        return 0;

    return pgm_read_word(&pressure_correction[returnValue]);
}

int16_t Board::getPadX(uint8_t pad)
{
    int16_t returnValue;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue = extractedSamples[coordinateX][pad];
    }

    return returnValue;
}

int16_t Board::getPadY(uint8_t pad)
{
    int16_t returnValue;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue = extractedSamples[coordinateY][pad];
    }

    return returnValue;
}

ISR(ADC_vect)
{
    if (padReadingIndex < readX)
    {
        coordinateSamples += ADC;

        if (padReadingIndex == 1)
        {
            //read pressure from two sensor plates and then get an average reading
            extractedSamples[coordinateZ][activePad] = coordinateSamples >> 1;

            //switch to x/y reading only if pad is pressed
            if (bitRead(padPressed, activePad))
            {
                //start reading x/y coordinates
                padReadingIndex++;
            }
            else
            {
                //switch to another pad
                activePad++;

                if (activePad == NUMBER_OF_PADS)
                    activePad = 0;

                setMuxInput(activePad);
                padReadingIndex = 0;
            }

            //reset samples count
            sampleCounter = 0;
            coordinateSamples = 0;
        }
        else
        {
            //next pressure reading
            padReadingIndex++;
        }
    }
    else if (padReadingIndex == readX)
    {
        //read x
        coordinateSamples += ADC;
        sampleCounter++;

        if (sampleCounter == 2)
        {
            extractedSamples[coordinateX][activePad] = coordinateSamples >> 1;

            sampleCounter = 0;
            coordinateSamples = 0;

            //start sampling y
            padReadingIndex++;
        }
    }
    else if (padReadingIndex == readY)
    {
        //read y
        coordinateSamples += ADC;
        sampleCounter++;

        if (sampleCounter == 2)
        {
            extractedSamples[coordinateY][activePad] = coordinateSamples >> 1;

            sampleCounter = 0;
            coordinateSamples = 0;

            //everything is read, skip to next pad and start over
            padReadingIndex = 0;

            activePad++;

            if (activePad == NUMBER_OF_PADS)
                activePad = 0;

            //set new pad
            setMuxInput(activePad);
        }
    }

    //configure i/o for readout
    (*valueSetup[padReadingIndex])();

    //switch adc channel
    setADCchannel(adcPinReadOrder_board[padReadingIndex]);
}