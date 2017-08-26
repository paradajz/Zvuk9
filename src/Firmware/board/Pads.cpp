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

const uint8_t       adcPinReadOrder_board[] =
{
    MUX_COMMON_PIN_0_PIN, //pressure, first reading
    MUX_COMMON_PIN_1_PIN, //pressure, second reading
    MUX_COMMON_PIN_0_PIN, //x coordinate
    MUX_COMMON_PIN_2_PIN  //y coordinate
};

volatile uint16_t   coordinateSamples[3];
uint8_t             sampleCounter;
int16_t             pressurePlate1;
int16_t             velocityReadout[NUMBER_OF_PADS];
volatile int16_t    velocity[NUMBER_OF_PADS];

uint8_t             pressureSamples[NUMBER_OF_PADS] =
{
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
};

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
    int16_t returnValue_pressure;
    int16_t returnValue_velocity;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue_pressure = extractedSamples[coordinateZ][pad];
        returnValue_velocity = velocity[pad];
        velocity[pad] = 0;
    }

    if (returnValue_velocity)
        return returnValue_velocity;

    if (returnValue_pressure == -1)
        return 0;

    returnValue_pressure = pgm_read_word(&pressure_correction[returnValue_pressure]);

    return returnValue_pressure > MIN_PAD_PRESSURE ? returnValue_pressure : 0;
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

inline void storeMaxPressure(bool firstPressure)
{
    //int16_t value;

    //if ((coordinateSamples[0] <= coordinateSamples[1]) && (coordinateSamples[0] <= coordinateSamples[2]))
    //{
        //value = (coordinateSamples[1] <= coordinateSamples[2]) ? coordinateSamples[1] : coordinateSamples[2];
    //}
    //else if ((coordinateSamples[1] <= coordinateSamples[0]) && (coordinateSamples[1] <= coordinateSamples[2]))
    //{
        //value = (coordinateSamples[0] <= coordinateSamples[2]) ? coordinateSamples[0] : coordinateSamples[2];
    //}
    //else
    //{
        //value = (coordinateSamples[0] <= coordinateSamples[1]) ? coordinateSamples[0] : coordinateSamples[1];
    //}

    uint16_t max = coordinateSamples[0];

    if (coordinateSamples[1] > max)
        max = coordinateSamples[1];

    if (coordinateSamples[2] > max)
        max = coordinateSamples[2];

    //special checks here due to two needed median values for two sensor plates
    if (firstPressure)
    {
        pressurePlate1 = max;
    }
    else
    {
        extractedSamples[coordinateZ][activePad] = (pressurePlate1 + max) >> 1;
    }
}

ISR(ADC_vect)
{
    if (padReadingIndex < readX)
    {
        coordinateSamples[sampleCounter] = ADC;
        sampleCounter++;

        if (padReadingIndex == 1)
        {
            if (sampleCounter == pressureSamples[activePad])
            {
                storeMaxPressure(false);

                //switch to x/y reading only if pad is pressed
                if (bitRead(padPressed, activePad))
                {
                    //start reading x/y coordinates
                    padReadingIndex++;
                    velocityReadout[activePad] = false;
                    pressureSamples[activePad] = 1;
                    coordinateSamples[0] = 0;
                    coordinateSamples[1] = 0;
                    coordinateSamples[2] = 0;
                }
                else
                {
                    //first valid touch has been registered
                    if ((extractedSamples[coordinateZ][activePad] > MIN_PAD_PRESSURE) && !velocityReadout[activePad])
                    {
                        extractedSamples[coordinateZ][activePad] = -1;
                        velocityReadout[activePad] = true;
                        //start taking three samples
                        pressureSamples[activePad] = 3;
                    }
                    //store this value as velocity - very important not to overwrite it!
                    else if ((extractedSamples[coordinateZ][activePad] > MIN_PAD_PRESSURE) && velocityReadout[activePad])
                    {
                        velocity[activePad] = extractedSamples[coordinateZ][activePad];
                    }

                    //switch to another pad
                    activePad++;

                    if (activePad == NUMBER_OF_PADS)
                        activePad = 0;

                    setMuxInput(activePad);
                    padReadingIndex = 0;
                }

                //reset samples count
                sampleCounter = 0;
            }
        }
        else
        {
            if (sampleCounter == pressureSamples[activePad])
            {
                sampleCounter = 0;

                storeMaxPressure(true);

                //next pressure reading
                padReadingIndex++;
            }
        }
    }
    else if (padReadingIndex == readX)
    {
        //read x
        coordinateSamples[0] += ADC;
        sampleCounter++;

        if (sampleCounter == 2)
        {
            extractedSamples[coordinateX][activePad] = coordinateSamples[0] >> 1;

            sampleCounter = 0;
            coordinateSamples[0] = 0;

            //start sampling y
            padReadingIndex++;
        }
    }
    else if (padReadingIndex == readY)
    {
        //read y
        coordinateSamples[0] += ADC;
        sampleCounter++;

        if (sampleCounter == 2)
        {
            extractedSamples[coordinateY][activePad] = coordinateSamples[0] >> 1;

            sampleCounter = 0;
            coordinateSamples[0] = 0;
            coordinateSamples[1] = 0;
            coordinateSamples[2] = 0;

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