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

#include "../Board.h"
#include "HardwareControl.cpp"
#include "board/common/analog/Variables.h"
#include "../../interface/analog/pads/DataTypes.h"

///
/// \ingroup boardAVR
/// @{

///
/// \brief Implementation of core variable used to keep track of run time in milliseconds.
///
volatile uint32_t   rTime_ms;


///
/// \brief Main interrupt service routine.
/// Used to control LED and button matrix and to update current run time.
///
ISR(TIMER3_COMPA_vect)
{
    //1ms
    ledRowsOff();

    if (activeOutColumn == NUMBER_OF_LED_COLUMNS)
        activeOutColumn = 0;

    activateOutputColumn();
    checkLEDs();

    activeOutColumn++;

    //update run time
    rTime_ms++;

    //read input matrix
    if (dIn_count < DIGITAL_IN_BUFFER_SIZE)
    {
        if (++dIn_head == DIGITAL_IN_BUFFER_SIZE)
            dIn_head = 0;

        storeDigitalIn();

        dIn_count++;
    }
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
            setupPressure0();
            break;

            case readPressure1:
            //store second pressure reading from opposite plate
            samples[coordinateZ][activePad] = 1023 - (ADC - pressurePlate1);
            padReadingIndex = readPressure2;
            setupPressure1();
            break;

            case readPressure2:
            pressurePlate1 = ADC;
            padReadingIndex = readPressure3;
            setupPressure1();
            break;

            case readPressure3:
            //store second pressure reading from opposite plate
            samples[coordinateZ][activePad] = samples[coordinateZ][activePad] + (1023 - (ADC - pressurePlate1));
            padReadingIndex = readX;
            setupX();
            break;

            case readX:
            samples[coordinateX][activePad] = ADC;
            //finally, read y
            padReadingIndex = readY;
            setupY();
            break;

            case readY:
            samples[coordinateY][activePad] = ADC;
            //continue with pressure reading
            padReadingIndex = readPressure0;
            padSwitch = true;
            setupPressure0();
            break;
        }

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