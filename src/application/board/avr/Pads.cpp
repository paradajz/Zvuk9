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
#include "../../interface/analog/pads/DataTypes.h"
#include "constants/Pads.h"

///
/// \ingroup boardAVR
/// @{

///
/// \brief Array holding pad indexes for easier access.
///
const uint8_t padIDArray[NUMBER_OF_PADS] =
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

///
/// \brief Array holding pins which should be read for specific coordinate.
/// Matched with padReadOrder_t enumeration.
///
const uint8_t coordinateAnalogInput[PAD_READINGS] =
{
    PAD_PLATE_X_PLUS_PIN,
    PAD_PLATE_Y_PLUS_PIN,
    PAD_PLATE_Y_MINUS_PIN,
    PAD_PLATE_X_MINUS_PIN,
    PAD_PLATE_Y_PLUS_PIN,
    PAD_PLATE_X_PLUS_PIN
};

///
/// \brief Variable holding temporary pressure reading from first pad plate.
/// Pressure is read using two readings (X and Y plate). Difference between two
/// readings is proportional to resistance (pressure) between plates.
///
uint16_t             pressurePlate1;

///
/// Array holding pointers to functions used to setup pins for
/// specified pad coordinates. Used for easier access in ISR.
///
void                (*coordinateSetup[PAD_READINGS])(void);

///
/// \brief Array holding ADC samples for all three coordinates for all pads.
///
volatile int16_t    samples[3][NUMBER_OF_PADS];

///
/// \brief Holds currently active pad (pad which is being processed).
///
uint8_t             activePad;

///
/// \brief Holds currently active coordinate reading for active pad.
///
uint8_t             padReadingIndex;


///
/// \brief Configures one of 16 inputs/outputs on 4067 multiplexer.
///
inline void setMuxInput(uint8_t muxInput)
{
    BIT_READ(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    BIT_READ(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    BIT_READ(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    BIT_READ(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);
}

///
/// \brief Configures pad pins so that pressure value can be read correctly.
// Two pressure setups are used. Using only one setup results in pressure being lower
/// in one pad corner. To overcome this, use two setups where opposite pins are configured.
/// Once both setups are finished, pressure is average value of two read pressure readings.
/// @{

void setupPressure0()
{
    //apply voltage from one X conductor to one Y conductor
    //read voltages at the other X and Y conductors
    setInput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setInput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);

    setLow(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setHigh(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
}

void setupPressure1()
{
    //apply voltage from one X conductor to one Y conductor
    //read voltages at the other X and Y conductors
    setInput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setInput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);

    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setHigh(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
}

/// @}

///
/// \brief Configures pad pins so that X value can be read correctly.
///
void setupX()
{
    //apply voltage across the X-plane
    //read the voltage from either Y conductor
    setInput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setInput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setOutput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setOutput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);

    setLow(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    setHigh(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
}

///
/// \brief Configures pad pins so that Y value can be read correctly.
///
void setupY()
{
    //apply voltage across the Y-plane
    //read the voltage from either X conductor
    setInput(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setInput(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setOutput(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setOutput(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);

    setLow(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    setLow(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    setHigh(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
    setLow(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
}

///
/// \brief ADC ISR used to read values from pads.
///
ISR(ADC_vect)
{
    //always ignore first reading
    static bool ignoreFirst = true;
    //pad should be switched if all coordinates are read
    bool padSwitch = false;

    if (!ignoreFirst)
    {
        switch(padReadingIndex)
        {
            case readPressure0:
            pressurePlate1 = ADC;
            padReadingIndex = readPressure1;
            break;

            case readPressure1:
            //store second pressure reading from opposite plate
            samples[coordinateZ][activePad] = 1023 - (ADC - pressurePlate1);
            padReadingIndex = readPressure2;
            break;

            case readPressure2:
            pressurePlate1 = ADC;
            padReadingIndex = readPressure3;
            break;

            case readPressure3:
            //store second pressure reading from opposite plate
            samples[coordinateZ][activePad] = samples[coordinateZ][activePad] + (1023 - (ADC - pressurePlate1));
            padReadingIndex = readX;
            break;

            case readX:
            samples[coordinateX][activePad] = ADC;
            //finally, read y
            padReadingIndex = readY;
            break;

            case readY:
            samples[coordinateY][activePad] = ADC;
            //continue with pressure reading
            padReadingIndex = readPressure0;
            padSwitch = true;
            break;
        }

        //configure i/o for readout
        (*coordinateSetup[padReadingIndex])();

        //switch adc channel
        setADCchannel(coordinateAnalogInput[padReadingIndex]);

        if (padSwitch)
        {
            activePad++;

            if (activePad == NUMBER_OF_PADS)
            {
                //all pads are read
                setMuxInput(padIDArray[0]);
                return;
            }

            //set new pad
            setMuxInput(padIDArray[activePad]);
        }
    }

    ignoreFirst = !ignoreFirst;

    startADCconversion();
}

/// @}

///
/// \brief Initializes pad variables and ADC peripheral.
///
void Board::initPads()
{
    //initialize ADC peripheral first
    adcConf adcConfiguration;

    adcConfiguration.prescaler = ADC_PRESCALER_64;
    adcConfiguration.vref = ADC_VREF_AVCC;

    setUpADC(adcConfiguration);

    //few dummy reads to init ADC properly
    for (int i=0; i<5; i++)
        getADCvalue();

    setupPressure0();
    //start by reading first pressure value
    padReadingIndex = readPressure0;
    setADCchannel(coordinateAnalogInput[padReadingIndex]);
    //select first pad
    activePad = 0;
    setMuxInput(padIDArray[activePad]);

    //place setup z/x/y in function pointer for simpler access in isr
    coordinateSetup[readPressure0] = setupPressure0;
    coordinateSetup[readPressure1] = setupPressure0;
    coordinateSetup[readPressure2] = setupPressure1;
    coordinateSetup[readPressure3] = setupPressure1;
    coordinateSetup[readX] = setupX;
    coordinateSetup[readY] = setupY;

    for (int i=0; i<3; i++)
    {
        for (int k=0; k<NUMBER_OF_PADS; k++)
        {
            samples[i][k] = -1;
        }
    }

    adcInterruptEnable();
}

///
/// \brief Checks if pad data is available.
/// Pad data is read in ISR and stored into samples array.
/// Once all coordinates are read, data is considered available.
/// At this point, activePad variable is set to invalid value
/// to stop further data reading from ISR until continuePadReadout
/// function is called.
/// \returns True if data is available, false otherwise.
///
bool Board::padDataAvailable()
{
    return (activePad == NUMBER_OF_PADS);
}

///
/// \brief Resets active pad index and starts data acquisition from pads again.
///
void Board::continuePadReadout()
{
    activePad = 0;
    startADCconversion();
}

///
/// \brief Returns Z coordinate (pressure) reading for requested pad.
/// @param [in] pad Pad for which reading is returned.
/// \returns Z coordinate (pressure) value for requested pad.
///
int16_t Board::getPadPressure(uint8_t pad)
{
    if (samples[coordinateZ][pad] != -1)
    {
        if (samples[coordinateZ][pad] >= PRESSURE_VALUES)
            samples[coordinateZ][pad] = PRESSURE_VALUES-1;

        uint16_t cVal = samples[coordinateZ][pad];//pgm_read_word(&pressure_correction[samples[coordinateZ][pad]]);

        //if pad is already pressed, return zero value only if it's smaller
        //than PAD_RELEASE_PRESSURE
        if (cVal < PAD_PRESS_PRESSURE)
        {
            if (BIT_READ(padPressed, pad))
            {
                if (cVal < PAD_RELEASE_PRESSURE)
                    cVal = 0;
            }
            else
            {
                cVal = 0;
            }
        }
        else
        {
            cVal -= PAD_PRESS_PRESSURE;
        }

        #ifdef DEBUG
        printf("pad %d pressure: %d raw: %d\n", pad, cVal, samples[coordinateZ][pad]);
        #endif

        return cVal;
    }
    else
    {
        return -1;
    }
}

///
/// \brief Returns X coordinate reading for requested pad.
/// @param [in] pad Pad for which reading is returned.
/// \returns X coordinate value for requested pad.
///
int16_t Board::getPadX(uint8_t pad)
{
    if (samples[coordinateX][pad] != -1)
        return 1023 - samples[coordinateX][pad];
    else
        return -1;
}

///
/// \brief Returns Y coordinate reading for requested pad.
/// @param [in] pad Pad for which reading is returned.
/// \returns Y coordinate value for requested pad.
///
int16_t Board::getPadY(uint8_t pad)
{
    if (samples[coordinateY][pad] != -1)
        return samples[coordinateY][pad];
    else
        return -1;
}