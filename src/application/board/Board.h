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

#pragma once

#include "interface/digital/output/leds/Variables.h"
#include "common/DataTypes.h"
#include "dbms/src/DataTypes.h"

///
/// \defgroup board Board

///
/// \ingroup board
/// @{

class Board
{
    public:
    ///
    /// \brief Default constructor.
    ///
    Board();

    ///
    /// \brief Perfoms initialization of MCU and all board peripherals.
    ///
    void init();

    ///
    /// \brief Checks if firmware has been updated.
    /// Firmware file has written CRC in last two flash addresses. Application stores last read CRC in EEPROM.
    /// If EEPROM and flash CRC differ, firmware has been updated.
    /// \returns True if firmware has been updated, false otherwise.
    ///
    static bool checkNewRevision();

    ///
    /// \brief Performs software MCU reboot.
    ///
    void reboot();

    ///
    /// \brief Checks if pad data is available.
    /// Pad data is read in ISR and stored into samples array.
    /// Once all coordinates are read, data is considered available.
    /// \returns True if data is available, false otherwise.
    ///
    bool padDataAvailable();

    ///
    /// \brief Returns Z coordinate (pressure) reading for requested pad.
    /// @param [in] pad Pad for which reading is returned.
    /// \returns Z coordinate (pressure) value for requested pad.
    ///
    int16_t getPadPressure(uint8_t pad);

    ///
    /// \brief Returns X coordinate reading for requested pad.
    /// @param [in] pad Pad for which reading is returned.
    /// \returns X coordinate value for requested pad.
    ///
    int16_t getPadX(uint8_t pad);

    ///
    /// \brief Returns Y coordinate reading for requested pad.
    /// @param [in] pad Pad for which reading is returned.
    /// \returns Y coordinate value for requested pad.
    ///
    int16_t getPadY(uint8_t pad);

    ///
    /// \brief Checks if data from button matrix is available.
    /// Matrix data is read in ISR and stored into digitalInBuffer array.
    /// Once all columns are read, data is considered available.
    /// \returns True if data is available, false otherwise.
    ///
    static bool digitalInputDataAvailable();

    ///
    /// \brief Returns last read button state for requested button index.
    /// @param [in] buttonIndex Index of button which should be read.
    /// \returns True if button is pressed, false otherwise.
    ///
    static bool getButtonState(uint8_t buttonIndex);

    ///
    /// \brief Checks if requested encoder ID is enabled.
    /// @param [in] encoderNumber Encoder which is being checked.
    /// \returns True if encoder is enabled, false otherwise.
    ///
    bool encoderEnabled(uint8_t encoderNumber);

    ///
    /// \brief Checks state of requested encoder.
    /// @param [in] encoderID       Encoder which is being checked.
    /// \returns 0 if encoder hasn't been moved, 1 if it's moving in positive and -1 if it's
    /// moving in negative direction.
    ///
    static int8_t getEncoderState(uint8_t encoderID);

    ///
    /// \brief Used to read contents of memory provided by specific board.
    /// @param [in] address Memory address from which to read from.
    /// @param [in] type    Type of parameter which is being read. Defined in DBMS module.
    /// @param [in] value   Pointer to variable in which read value is being stored.
    /// \returns            True on success, false otherwise.
    ///
    static bool memoryRead(uint32_t address, sectionParameterType_t type, int32_t &value);

    ///
    /// \brief Used to write value to memory provided by specific board.
    /// @param [in] address Memory address in which new value is being written.
    /// @param [in] value   Value to write.
    /// @param [in] type    Type of parameter which is being written. Defined in DBMS module.
    /// \returns            True on success, false otherwise.
    ///
    static bool memoryWrite(uint32_t address, int32_t value, sectionParameterType_t type);

    private:
    ///
    /// \brief Initializes all pins to correct states.
    ///
    static void initPins();
    ///
    /// \brief Initializes USB peripheral and configures it as MIDI device.
    ///
    static void initUSB_MIDI();

    ///
    /// \brief Initializes UART peripheral used to send and receive MIDI data.
    ///
    static void initUART_MIDI();

    ///
    /// \brief Initializes main and PWM timers.
    ///
    static void initTimers();

    ///
    /// \brief Initializes pads and ADC peripheral.
    ///
    static void initPads();

    ///
    /// \brief Checks state of requested encoder.
    /// Internal function.
    /// @param [in] encoderID       Encoder which is being checked.
    /// @param [in] pairState       A and B signal readings from encoder placed into bits 0 and 1.
    /// \returns 0 if encoder hasn't been moved, 1 if it's moving in positive and -1 if it's
    /// moving in negative direction.
    ///
    static int8_t readEncoder(uint8_t encoderID, uint8_t pairState);
};

///
/// \brief External definition of Board class instance.
///
extern Board board;

/// @}