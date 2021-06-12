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

#include "board/Board.h"
#include "board/Internal.h"
#include "board/common/constants/IO.h"
#include "shared/Types.h"
#include "core/src/general/ADC.h"
#include "core/src/general/Helpers.h"
#include "core/src/general/Atomic.h"
#include "core/src/general/Timing.h"
#include "Pins.h"

//symbolic names
#define PAD_PLATE_X_PLUS_PORT  MUX_PORT_INPUT_0
#define PAD_PLATE_X_PLUS_PIN   MUX_PIN_INPUT_0
#define PAD_PLATE_X_MINUS_PORT MUX_PORT_INPUT_1
#define PAD_PLATE_X_MINUS_PIN  MUX_PIN_INPUT_1
#define PAD_PLATE_Y_PLUS_PORT  MUX_PORT_INPUT_2
#define PAD_PLATE_Y_PLUS_PIN   MUX_PIN_INPUT_2
#define PAD_PLATE_Y_MINUS_PORT MUX_PORT_INPUT_3
#define PAD_PLATE_Y_MINUS_PIN  MUX_PIN_INPUT_3

#ifdef ADC_12_BIT
/// Raw ADC value after which pad is considered pressed.
#define PAD_PRESS_PRESSURE 50
#else
#error Unsupported ADC resolution
#endif

namespace
{
    volatile uint16_t  _padProcessed;
    volatile padData_t _data[NUMBER_OF_PADS];

    /// Array holding ADC channels which should be read for specific coordinate.
    /// Matched with padReadOrder_t enumeration.
    const uint32_t _coordinateAnalogInput[static_cast<uint8_t>(Board::detail::io::padReadOrder_t::AMOUNT)] = {
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN)),
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN)),
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN)),
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN)),
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN)),
        Board::detail::map::adcChannel(CORE_IO_MCU_PIN_DEF(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN)),
    };

    /// Configures pad pins so that pressure value can be read correctly.
    /// Two pressure setups are used. Using only one setup results in pressure being lower
    /// in one pad corner. To overcome this, use two setups where opposite pins are configured.
    /// Once both setups are finished, pressure is average value of two read pressure readings.
    inline void setupPressure0()
    {
        //apply voltage from one X conductor to one Y conductor
        //read voltages at the other X and Y conductors
        CORE_IO_CONFIG({ PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });

        CORE_IO_SET_LOW(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
        CORE_IO_SET_HIGH(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    }

    /// Configures pad pins so that pressure value can be read correctly.
    /// Two pressure setups are used. Using only one setup results in pressure being lower
    /// in one pad corner. To overcome this, use two setups where opposite pins are configured.
    /// Once both setups are finished, pressure is average value of two read pressure readings.
    inline void setupPressure1()
    {
        //apply voltage from one X conductor to one Y conductor
        //read voltages at the other X and Y conductors
        CORE_IO_CONFIG({ PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });

        CORE_IO_SET_LOW(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
        CORE_IO_SET_HIGH(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
    }

    /// Configures pad pins so that X value can be read correctly.
    inline void setupX()
    {
        //apply voltage across the X-plane
        //read the voltage from either Y conductor
        CORE_IO_CONFIG({ PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });

        CORE_IO_SET_LOW(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
        CORE_IO_SET_HIGH(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
    }

    /// Configures pad pins so that Y value can be read correctly.
    inline void setupY()
    {
        //apply voltage across the Y-plane
        //read the voltage from either X conductor
        CORE_IO_CONFIG({ PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN, core::io::pinMode_t::input, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });
        CORE_IO_CONFIG({ PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN, core::io::pinMode_t::outputPP, core::io::pullMode_t::none, core::io::gpioSpeed_t::medium, 0x00 });

        CORE_IO_SET_LOW(PAD_PLATE_X_PLUS_PORT, PAD_PLATE_X_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_X_MINUS_PORT, PAD_PLATE_X_MINUS_PIN);
        CORE_IO_SET_HIGH(PAD_PLATE_Y_PLUS_PORT, PAD_PLATE_Y_PLUS_PIN);
        CORE_IO_SET_LOW(PAD_PLATE_Y_MINUS_PORT, PAD_PLATE_Y_MINUS_PIN);
    }

    /// Configures one of 16 inputs/outputs on 4067 multiplexer.
    inline void setMuxInput(uint8_t muxInput)
    {
        BIT_READ(muxInput, 0) ? CORE_IO_SET_HIGH(MUX_PORT_S0, MUX_PIN_S0) : CORE_IO_SET_LOW(MUX_PORT_S0, MUX_PIN_S0);
        BIT_READ(muxInput, 1) ? CORE_IO_SET_HIGH(MUX_PORT_S1, MUX_PIN_S1) : CORE_IO_SET_LOW(MUX_PORT_S1, MUX_PIN_S1);
        BIT_READ(muxInput, 2) ? CORE_IO_SET_HIGH(MUX_PORT_S2, MUX_PIN_S2) : CORE_IO_SET_LOW(MUX_PORT_S2, MUX_PIN_S2);
        BIT_READ(muxInput, 3) ? CORE_IO_SET_HIGH(MUX_PORT_S3, MUX_PIN_S3) : CORE_IO_SET_LOW(MUX_PORT_S3, MUX_PIN_S3);
    }
}    // namespace

namespace Board
{
    namespace io
    {
        bool padDataAvailable(size_t index, padData_t& data)
        {
            bool available = false;

            ATOMIC_SECTION
            {
                available = BIT_READ(_padProcessed, index);
            }

            if (available)
            {
                ATOMIC_SECTION
                {
                    data.velocity = _data[index].velocity;
                    data.pressure = _data[index].pressure;
                    data.x        = _data[index].x;
                    data.y        = _data[index].y;
                }
            }

            return available;
        }
    }    // namespace io

    namespace detail
    {
        namespace isrHandling
        {
            void adc(uint16_t value)
            {
                using namespace Board::detail::io;

                static bool           firstReading                             = false;
                static uint16_t       pressurePlate1                           = 0;
                static uint32_t       velocityMeasureStartTime[NUMBER_OF_PADS] = {};
                static uint32_t       releaseMeasureStartTime[NUMBER_OF_PADS]  = {};
                static uint8_t        activePad                                = 0;
                static padReadOrder_t padReadOrderIndex                        = padReadOrder_t::readPressure0;
                bool                  padSwitch                                = false;

                //always ignore first reading
                firstReading = !firstReading;

                if (!firstReading)
                {
                    switch (padReadOrderIndex)
                    {
                    case padReadOrder_t::readPressure0:
                    {
                        pressurePlate1    = value;
                        padReadOrderIndex = padReadOrder_t::readPressure1;
                        setupPressure0();
                    }

                    break;

                    case padReadOrder_t::readPressure1:
                    {
                        _data[activePad].pressure = MAX_ADC_VALUE - (value - pressurePlate1);
                        padReadOrderIndex         = padReadOrder_t::readPressure2;
                        setupPressure1();
                    }
                    break;

                    case padReadOrder_t::readPressure2:
                    {
                        pressurePlate1    = value;
                        padReadOrderIndex = padReadOrder_t::readPressure3;
                        setupPressure1();
                    }
                    break;

                    case padReadOrder_t::readPressure3:
                    {
                        uint16_t newPressure = _data[activePad].pressure + (MAX_ADC_VALUE - (value - pressurePlate1));
                        padReadOrderIndex    = padReadOrder_t::readX;
                        setupX();

                        //we now have entire pressure reading
                        //see if the pressure has crossed the threshold for pressure and start measuring velocity if it did
                        //note: normally, this is supposed to be application, not board logic
                        //however, we really need tight timings on detecting velocity and release so we're doing these measurements here instead

                        if (newPressure > PAD_PRESS_PRESSURE)
                        {
                            //reset debounce timeout every time release threshold is passed
                            releaseMeasureStartTime[activePad] = 0;

                            //no point in measuring velocity if the velocity is already measured
                            if (!_data[activePad].velocity)
                            {
                                if (!velocityMeasureStartTime[activePad])
                                {
                                    velocityMeasureStartTime[activePad] = core::timing::currentRunTimeMs();
                                }
                                else
                                {
                                    if ((core::timing::currentRunTimeMs() - velocityMeasureStartTime[activePad]) >= PAD_VELOCITY_TIMEOUT)
                                    {
                                        _data[activePad].velocity = newPressure;
                                    }
                                }
                            }

                            _data[activePad].pressure = newPressure;
                        }
                        else
                        {
                            //no point in debouncing release if pad isn't even pressed
                            if (_data[activePad].velocity)
                            {
                                if (!releaseMeasureStartTime[activePad])
                                {
                                    releaseMeasureStartTime[activePad] = core::timing::currentRunTimeMs();
                                }
                                else
                                {
                                    if ((core::timing::currentRunTimeMs() - releaseMeasureStartTime[activePad]) >= PAD_RELEASE_DEBOUNCE_TIMEOUT)
                                    {
                                        velocityMeasureStartTime[activePad] = 0;
                                        _data[activePad].velocity           = 0;
                                        _data[activePad].pressure           = 0;
                                        _data[activePad].x                  = 0;
                                        _data[activePad].y                  = 0;
                                    }
                                }
                            }
                        }
                    }
                    break;

                    case padReadOrder_t::readX:
                    {
                        _data[activePad].x = value;
                        padReadOrderIndex  = padReadOrder_t::readY;
                        setupY();
                    }
                    break;

                    case padReadOrder_t::readY:
                    {
                        _data[activePad].y = value;
                        padReadOrderIndex  = padReadOrder_t::readPressure0;
                        padSwitch          = true;
                        setupPressure0();
                        BIT_SET(_padProcessed, activePad);
                    }
                    break;

                    default:
                        Board::detail::errorHandler();
                        break;
                    }

                    //switch adc channel
                    core::adc::setChannel(Board::detail::map::adcChannel(_coordinateAnalogInput[static_cast<uint8_t>(padReadOrderIndex)]));

                    if (padSwitch)
                    {
                        if (++activePad == NUMBER_OF_PADS)
                        {
                            //all pads are read
                            activePad = 0;
                        }

                        //set new pad
                        setMuxInput(activePad);
                        BIT_CLEAR(_padProcessed, activePad);
                    }
                }

                core::adc::startConversion();
            }
        }    // namespace isrHandling
    }        // namespace detail
}    // namespace Board
