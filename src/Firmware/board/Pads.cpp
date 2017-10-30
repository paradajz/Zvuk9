/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

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
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);      //X+ first read
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);     //X-
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);     //Y+ in
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);     //Y-

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);        //X+
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);        //X-
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);        //Y+
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);       //Y-
}

void setupPressure2()
{
    //To read force (Z-axis), apply a voltage a voltage from one X conductor to one Y conductor, then read voltages at the other X and Y conductors
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);     //X+
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);      //X- second read
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);     //Y+
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);     //Y- in

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);        //X+
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);        //X-
    setHigh(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);       //Y+
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);       //Y-
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
    bool pressed;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        returnValue_pressure = extractedSamples[coordinateZ][pad];
        returnValue_velocity = velocity[pad];
        velocity[pad] = 0;
        pressed = bitRead(padPressed, pad);
    }

    if (returnValue_velocity)
        return returnValue_velocity;

    if (returnValue_pressure == -1)
        return 0;

    returnValue_pressure = pgm_read_word(&pressure_correction[returnValue_pressure]);

    if (returnValue_pressure > PAD_PRESS_PRESSURE)
    {
        return returnValue_pressure;
    }
    else
    {
        if (pressed)
        {
            if (returnValue_pressure < PAD_RELEASE_PRESSURE)
                return 0;
            else
                return returnValue_pressure;
        }
        else
        {
            return 0;
        }
    }
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

inline void storeMaxPressure()
{
    uint16_t max = coordinateSamples[0];

    if (coordinateSamples[1] > max)
        max = coordinateSamples[1];

    if (coordinateSamples[2] > max)
        max = coordinateSamples[2];

    extractedSamples[coordinateZ][activePad] = (pressurePlate1 + max);
}

ISR(ADC_vect)
{
    if (padReadingIndex < readX)
    {
        coordinateSamples[sampleCounter] = ADC;
        sampleCounter++;

        if (padReadingIndex == readPressure1)
        {
            if (sampleCounter == pressureSamples[activePad])
            {
                //store second pressure reading from opposite plate
                storeMaxPressure();

                //switch to x/y reading only if pad is pressed
                if (bitRead(padPressed, activePad))
                {
                    //start reading x/y coordinates
                    padReadingIndex = readX;
                    velocityReadout[activePad] = false;
                    //we don't need three samples anymore
                    pressureSamples[activePad] = 1;
                    coordinateSamples[0] = 0;
                    coordinateSamples[1] = 0;
                    coordinateSamples[2] = 0;
                }
                else
                {
                    if ((extractedSamples[coordinateZ][activePad] > PAD_PRESS_PRESSURE) && !velocityReadout[activePad])
                    {
                        //first valid touch has been registered
                        //reset current pressure reading and perform reading again based on three samples of which max is chosen
                        extractedSamples[coordinateZ][activePad] = -1;
                        velocityReadout[activePad] = true;
                        //start taking three samples
                        pressureSamples[activePad] = 3;
                    }
                    else if ((extractedSamples[coordinateZ][activePad] > PAD_PRESS_PRESSURE) && velocityReadout[activePad])
                    {
                        //store this value as velocity - very important not to overwrite it!
                        velocity[activePad] = extractedSamples[coordinateZ][activePad];
                    }
                    else
                    {
                        //no pressure reading
                        pressureSamples[activePad] = 1;

                        //switch to another pad
                        activePad++;

                        if (activePad == NUMBER_OF_PADS)
                            activePad = 0;

                        setMuxInput(activePad);
                    }

                    padReadingIndex = readPressure0;
                }

                //reset sample count
                sampleCounter = 0;
            }
        }
        else
        {
            if (sampleCounter == pressureSamples[activePad])
            {
                sampleCounter = 0;

                pressurePlate1 = coordinateSamples[0];

                //next pressure reading
                padReadingIndex = readPressure1;
            }
        }
    }
    //when taking x/y readings, simply take two readings and store average value
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
            padReadingIndex = readY;
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
            padReadingIndex = readPressure0;

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
