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

#include <stddef.h>
#include <inttypes.h>
#include "midi/src/MIDI.h"
#include "shared/Types.h"

namespace Board
{
#ifdef UID_BITS
    /// Structure holding unique ID for MCU.
    typedef struct
    {
        uint8_t uid[UID_BITS / 8];
    } uniqueID_t;

    /// Retrieves MCU-specific unique ID number.
    /// param [in]: uid Reference to structure in which retrieved UID will be stored.
    void uniqueID(uniqueID_t& uid);
#endif

    /// Perfoms initialization of MCU and all board peripherals.
    void init();

    /// Performs software MCU reboot.
    void reboot();

    namespace io
    {
        enum class rgbIndex_t : uint8_t
        {
            r,
            g,
            b
        };

        enum class ledBrightness_t : uint8_t
        {
            bOff = 0,
            b25  = 1,
            b50  = 2,
            b75  = 3,
            b100 = 4
        };

        enum class encoderIndex_t : uint8_t
        {
            a,
            b
        };

        /// Structure containing digital input readings for a given input.
        /// Count represents total amount of readings stored in readings variable.
        /// Readings variable contains up to last 32 readings where LSB bit is the
        /// newest reading, and MSB bit is the last.
        typedef struct
        {
            uint8_t  count;
            uint32_t readings;
        } dInReadings_t;

        /// Returns last read digital input states for requested digital input index.
        /// param [in]:     digitalInIndex  Index of digital input which should be read.
        /// param [in,out]: dInReadings     Reference to variable in which new digital input readings are stored.
        /// returns: True if there are new readings for specified digital input index.
        bool digitalInState(size_t digitalInIndex, dInReadings_t& dInReadings);

        /// Calculates encoder index based on provided button index.
        /// param [in]: buttonID   Button index from which encoder is being calculated.
        /// returns: Calculated encoder index.
        size_t encoderIndex(size_t buttonID);

        /// Used to calculate index of A or B signal of encoder.
        /// param [in]: encoderID       Encoder which is being checked.
        /// param [in]: index   A or B signal (enumerated type, see encoderIndex_t).
        /// returns: Calculated index of A or B signal of encoder.
        size_t encoderSignalIndex(size_t encoderID, encoderIndex_t index);

        /// Used to turn LED connected to the board on or off.
        /// param [in]: ledID           LED for which to change state.
        /// param [in]: brightnessLevel See ledBrightness_t enum.
        void writeLEDstate(size_t ledID, ledBrightness_t ledBrightness);

        /// brief Checks for current analog value for specified analog index.
        /// @param[in] analogID     Analog index for which ADC value is being checked.
        /// param [in,out]:         Reference to variable in which new ADC reading is stored.
        /// returns: True if there is a new reading for specified analog index.
        bool analogValue(size_t analogID, uint16_t& value);

        /// Checks if pad data is available for specified pad.
        /// Once all coordinates are read, data is considered available.
        /// \returns True if data is available, false otherwise.
        bool padDataAvailable(size_t index, padData_t& data);
    }    // namespace io

    namespace USB
    {
        /// Checks if USB has been enumerated on host machine.
        bool isUSBconnected();

        /// Used to read MIDI data from USB interface.
        /// param [in]: USBMIDIpacket   Pointer to structure in which MIDI data is stored.
        /// returns: True if data is available, false otherwise.
        bool readMIDI(MIDI::USBMIDIpacket_t& USBMIDIpacket);

        /// Used to write MIDI data to USB interface.
        /// param [in]: USBMIDIpacket   Pointer to structure holding MIDI data to write.
        /// returns: True if data is available, false otherwise.
        bool writeMIDI(MIDI::USBMIDIpacket_t& USBMIDIpacket);
    }    // namespace USB

    namespace UART
    {
        /// Initializes UART peripheral.
        /// param [in]: channel     UART channel on MCU.
        /// param [in]: baudRate    UART speed (baudRate).
        bool init(uint8_t channel, uint32_t baudRate);

        /// Deinitializes specified UART channel.
        /// param [in]: channel UART channel on MCU.
        bool deInit(uint8_t channel);

        /// Checks if specified UART channel is initialized.
        /// param [in]: channel UART channel on MCU.
        bool isInitialized(uint8_t channel);

        /// Used to read MIDI data from RX UART buffer.
        /// param [in]:     channel     UART channel on MCU.
        /// param [in,out]: data        Pointer to variable in which read data is being stored.
        /// returns: False if buffer is empty, true otherwise.
        bool read(uint8_t channel, uint8_t& data);

        /// Used to write MIDI data to UART TX buffer.
        /// param [in]: channel     UART channel on MCU.
        /// param [in]: data        Byte of data to write.
        /// returns: True on success. Since this function waits until
        /// outgoig buffer is full, result will always be success (1).
        bool write(uint8_t channel, uint8_t data);

        /// Used to enable or disable UART loopback functionality.
        /// Used to pass incoming UART data to TX channel immediately.
        /// param [in]: channel UART channel on MCU.
        /// param [in]: state   New state of loopback functionality (true/enabled, false/disabled).
        void setLoopbackState(uint8_t channel, bool state);

        /// Checks if all data on specified UART channel has been sent.
        /// param [in]: channel UART channel on MCU.
        /// returns: True if there is no more data to transmit, false otherwise.
        bool isTxEmpty(uint8_t channel);
    }    // namespace UART

    namespace NVM
    {
        //NVM: non-volatile memory

        enum class parameterType_t : uint8_t
        {
            byte,
            word,
            dword
        };

        /// Initializes and prepares non-volatile storage on board.
        bool init();

        /// Returns total available bytes to store in non-volatile memory.
        uint32_t size();

        /// Used to wipe non-volatile memory on specified range.
        bool clear();

        /// Returns amount of actual memory it takes to store provided parameter type.
        size_t paramUsage(parameterType_t type);

        /// Used to read contents of memory provided by specific board,
        /// param [in]: address Memory address from which to read from.
        /// param [in]: value   Pointer to variable in which read value is being stored.
        /// param [in]: type    Type of parameter which is being read.
        /// returns: True on success, false otherwise.
        bool read(uint32_t address, int32_t& value, parameterType_t type);

        /// Used to write value to memory provided by specific board.
        /// param [in]: address Memory address in which new value is being written.
        /// param [in]: value   Value to write.
        /// param [in]: type    Type of parameter which is being written.
        /// returns: True on success, false otherwise.
        bool write(uint32_t address, int32_t value, parameterType_t type);
    }    // namespace NVM
};       // namespace Board