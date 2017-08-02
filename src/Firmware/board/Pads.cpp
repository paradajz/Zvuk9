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
    MUX_COMMON_PIN_2_PIN, //pressure, first reading
    MUX_COMMON_PIN_0_PIN, //pressure, second reading
    MUX_COMMON_PIN_2_PIN, //x coordinate
    MUX_COMMON_PIN_0_PIN  //y coordinate
};

volatile uint16_t   coordinateSamples[3];
volatile uint16_t   pressureReading[2];

uint8_t             sampleCounter;

//uint16_t firstPressureIgnored;
volatile uint16_t   velocityStored;

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

void setupPressure()
{
    //pressure is read from x+/y+
    //set 0/5V across x-/y-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);
}

void setupY()
{
    //x is read from y+
    //set 0/5V across x+/x-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN); //read this
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN); //output!
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN); //read this
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);
}

void setupX()
{
    //y is read from x+
    //set 0/5V across y+/y-
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN); //read this
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN); //read this
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);
}

void Board::initPads()
{
    setupPressure();
    setADCchannel(adcPinReadOrder_board[0]);
    setMuxInput(0);

    //place setup z/x/y in function pointer for simpler access in isr
    valueSetup[readPressure0] = setupPressure;
    valueSetup[readPressure1] = setupPressure;
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
        bitWrite(velocityStored, activePad, 0);
    }

    return returnValue;
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

inline void storeMedianValue(padCoordinate_t coordinate)
{
    if ((coordinateSamples[0] <= coordinateSamples[1]) && (coordinateSamples[0] <= coordinateSamples[2]))
    {
        extractedSamples[coordinate][activePad] = (coordinateSamples[1] <= coordinateSamples[2]) ? coordinateSamples[1] : coordinateSamples[2];
    }
    else if ((coordinateSamples[1] <= coordinateSamples[0]) && (coordinateSamples[1] <= coordinateSamples[2]))
    {
        extractedSamples[coordinate][activePad] = (coordinateSamples[0] <= coordinateSamples[2]) ? coordinateSamples[0] : coordinateSamples[2];
    }
    else
    {
        extractedSamples[coordinate][activePad] = (coordinateSamples[0] <= coordinateSamples[1]) ? coordinateSamples[0] : coordinateSamples[1];
    }
}

ISR(ADC_vect)
{
    if (padReadingIndex < readX)
    {
        pressureReading[padReadingIndex] = ADC;

        if (padReadingIndex == readPressure1)
        {
            //store pressure sample
            if (sampleCounter >= 1)
            {
                //ignore first reading
                coordinateSamples[sampleCounter-1] = (1023 - (pressureReading[readPressure1] - pressureReading[readPressure0]));
            }

            sampleCounter++;

            if (sampleCounter == 4)
            {
                //make sure *not* to overwrite velocity value
                if (!bitRead(padPressed, activePad) && !bitRead(velocityStored, activePad))
                {
                    //store median value
                    storeMedianValue(coordinateZ);

                    if (extractedSamples[coordinateZ][activePad] > 0)
                        bitWrite(velocityStored, activePad, 1);
                }
                else if (bitRead(padPressed, activePad))
                {
                    storeMedianValue(coordinateZ);
                }

                //reset pressure count
                sampleCounter = 0;

                //start reading x/y coordinates
                padReadingIndex++;
            }
            else
            {
                //just reset read index back to zero, don't switch pad yet
                padReadingIndex = 0;
            }
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
        coordinateSamples[sampleCounter] = ADC;
        sampleCounter++;

        if (sampleCounter == 3)
        {
            //store median value
            storeMedianValue(coordinateX);

            //reset pressure count
            sampleCounter = 0;

            //start sampling y
            padReadingIndex++;
        }
    }
    else if (padReadingIndex == readY)
    {
        //read y
        coordinateSamples[sampleCounter] = ADC;
        sampleCounter++;

        if (sampleCounter == 3)
        {
            //store median value
            storeMedianValue(coordinateY);

            //reset pressure count
            sampleCounter = 0;

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