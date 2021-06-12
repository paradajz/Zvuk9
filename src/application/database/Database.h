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

#include "dbms/src/LESSDB.h"

class Database : public LESSDB
{
    public:
    class Handlers
    {
        public:
        Handlers()                                = default;
        virtual void presetChange(uint8_t preset) = 0;
        virtual void factoryResetStart()          = 0;
        virtual void factoryResetDone()           = 0;
        virtual void initialized()                = 0;
    };

    Database(LESSDB::StorageAccess& storageAccess)
        : LESSDB(storageAccess)
    {}

    enum class block_t : uint8_t
    {
        global,
        program,
        scale,
        padCalibration,
        AMOUNT
    };

    class Section
    {
        public:
        Section() {}

        enum class global_t : uint32_t
        {
            midi,
            velocitySensitivity,
            AMOUNT
        };

        enum class program_t : uint32_t
        {
            lastActiveProgram,
            lastActiveScale,
            globalSettings,
            localSettings,
            AMOUNT
        };

        enum class scale_t : uint32_t
        {
            predefined,
            user,
            AMOUNT
        };

        enum class padCalibration_t : uint32_t
        {
            pressureUpper,
            xLower,
            xUpper,
            yLower,
            yUpper,
            AMOUNT
        };
    };

    enum class globalProgramSettings_t : uint32_t
    {
        GLOBAL_PROGRAM_SETTING_SPLIT_STATE_ID,
        GLOBAL_PROGRAM_SETTING_X_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID,
        GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
        GLOBAL_PROGRAM_SETTING_CC_X_ID,
        GLOBAL_PROGRAM_SETTING_CC_Y_ID,
        GLOBAL_PROGRAM_SETTING_X_MIN_ID,
        GLOBAL_PROGRAM_SETTING_X_MAX_ID,
        GLOBAL_PROGRAM_SETTING_Y_MIN_ID,
        GLOBAL_PROGRAM_SETTING_Y_MAX_ID,
        GLOBAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
        GLOBAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
        AMOUNT
    };

    enum class localProgramSettings_t : uint32_t
    {
        LOCAL_PROGRAM_SETTING_X_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_Y_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_PITCH_BEND_X_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_PITCH_BEND_Y_ENABLE_ID,
        LOCAL_PROGRAM_SETTING_MIDI_CHANNEL_ID,
        LOCAL_PROGRAM_SETTING_CC_X_ID,
        LOCAL_PROGRAM_SETTING_CC_Y_ID,
        LOCAL_PROGRAM_SETTING_X_MIN_ID,
        LOCAL_PROGRAM_SETTING_X_MAX_ID,
        LOCAL_PROGRAM_SETTING_Y_MIN_ID,
        LOCAL_PROGRAM_SETTING_Y_MAX_ID,
        LOCAL_PROGRAM_SETTING_X_CURVE_GAIN_ID,
        LOCAL_PROGRAM_SETTING_Y_CURVE_GAIN_ID,
        AMOUNT
    };

    enum class predefinedScaleParameters_t : uint32_t
    {
        PREDEFINED_SCALE_OCTAVE_ID,
        PREDEFINED_SCALE_TONIC_ID,
        PREDEFINED_SCALE_SHIFT_ID,
        AMOUNT
    };

    enum class midiSettings_t : uint32_t
    {
        MIDI_SETTING_AFTERTOUCH_TYPE_ID,
        MIDI_SETTING_RUNNING_STATUS_ID,
        MIDI_SETTING_NOTE_OFF_TYPE_ID,
        MIDI_SETTING_TRANSPORT_CC_ID,
        MIDI_SETTING_PITCH_BEND_TYPE_ID,
        MIDI_SETTING_RESERVED_1_ID,
        MIDI_SETTING_RESERVED_2_ID,
        MIDI_SETTING_RESERVED_3_ID,
        AMOUNT
    };

    enum class velocitySettings_t : uint32_t
    {
        VELOCITY_SETTING_SENSITIVITY_ID,
        VELOCITY_SETTING_CURVE_ID,
        AMOUNT
    };

    static constexpr uint8_t NUMBER_OF_PROGRAMS       = 15;
    static constexpr uint8_t NUMBER_OF_USER_SCALES    = 10;
    static constexpr uint8_t NOTES_PER_PAD_USER_SCALE = 7;
    static constexpr uint8_t BLANK_NOTE               = 128;

    using LESSDB::read;
    using LESSDB::update;

    template<typename T>
    int32_t read(T section, size_t index)
    {
        block_t blockIndex = block(section);
        return LESSDB::read(static_cast<uint8_t>(blockIndex), static_cast<uint8_t>(section), index);
    }

    template<typename T>
    bool read(T section, size_t index, int32_t& value)
    {
        block_t blockIndex = block(section);
        return LESSDB::read(static_cast<uint8_t>(blockIndex), static_cast<uint8_t>(section), index, value);
    }

    template<typename T>
    bool update(T section, size_t index, int32_t value)
    {
        block_t blockIndex = block(section);
        return LESSDB::update(static_cast<uint8_t>(blockIndex), static_cast<uint8_t>(section), index, value);
    }

    bool init();
    bool factoryReset(LESSDB::factoryResetType_t type);
    bool isInitialized();
    void registerHandlers(Handlers& handlers);

    private:
    block_t block(Section::global_t section)
    {
        return block_t::global;
    }

    block_t block(Section::program_t section)
    {
        return block_t::program;
    }

    block_t block(Section::scale_t section)
    {
        return block_t::scale;
    }

    block_t block(Section::padCalibration_t section)
    {
        return block_t::padCalibration;
    }

    bool     isSignatureValid();
    uint16_t getDbUID();
    bool     setDbUID(uint16_t uid);
    void     writeDefaults();
    void     writeGlobalSettings();
    void     writeProgramSettings();
    void     writeScales();

    Handlers* _handlers = nullptr;

    /// Address at which user data starts (after system block).
    /// Used to set correct offset in database for user layout.
    uint32_t _userDataStartAddress;

    bool _initialized = false;
};