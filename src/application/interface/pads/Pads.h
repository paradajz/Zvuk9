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

#include "curves/Curves.h"
#include "../../board/Board.h"
#include "Config.h"
#include "../../database/Database.h"
#include "Sanity.h"

///
/// \brief Pad updating and processing.
/// \defgroup interfacePads Pads
/// \ingroup interface
/// @{

class Pads
{
    public:
    Pads();

    void init();
    void update();

    uint8_t getLastTouchedPad();
    bool isPadPressed(int8_t pad);
    uint8_t getNumberOfPressedPads();
    uint8_t getProgram();
    uint8_t getScale();
    bool isUserScale(int8_t scale);
    bool isPredefinedScale(int8_t scale);
    bool getEditModeState();
    note_t getTonic();
    uint8_t getOctave(bool padEditMode = false);
    bool getSplitState();
    bool getMIDISendState(int8_t pad, function_t type);
    aftertouchType_t getAftertouchType();
    uint8_t getPadNote(int8_t pad, int8_t noteIndex);
    bool isNoteAssigned(note_t note);
    note_t getTonicFromNote(int8_t note);
    uint8_t getOctaveFromNote(int8_t note);
    int8_t getScaleShiftLevel();
    uint8_t getCCvalue(int8_t pad, padCoordinate_t type);
    uint8_t getCClimit(int8_t pad, padCoordinate_t type, limitType_t limitType);
    curve_t getCCcurve(int8_t pad, padCoordinate_t curve);
    velocitySensitivity_t getVelocitySensitivity();
    curve_t getVelocityCurve();
    uint8_t getMIDIchannel(int8_t pad);
    bool isCalibrationEnabled();
    padCoordinate_t getCalibrationMode();
    uint16_t getCalibrationLimit(int8_t pad, padCoordinate_t coordinate, limitType_t limitType);
    int8_t getPredefinedScaleNotes(int8_t scale);
    note_t getScaleNote(int8_t scale, int8_t index);
    uint8_t getScaledPressure(int8_t pad, uint16_t pressure, pressureType_t type);
    uint16_t getScaledXY(int8_t pad, uint16_t xyValue, padCoordinate_t type, valueScaleType_t scaleType);
    pitchBendType_t getPitchBendType();
    bool getPitchBendState(int8_t pad, padCoordinate_t coordinate);

    changeResult_t setProgram(int8_t program);
    changeResult_t setScale(int8_t scale);
    changeResult_t setTonic(note_t note, bool internalChange = false);
    changeResult_t setEditModeState(bool state, int8_t pad = 0);
    changeResult_t setSplitState(bool state);
    changeResult_t setMIDISendState(function_t type, bool state);
    changeResult_t setAftertouchType(aftertouchType_t type);
    changeResult_t setMIDIchannel(int8_t pad, int8_t channel);
    changeResult_t setVelocitySensitivity(velocitySensitivity_t type);
    changeResult_t setVelocityCurve(curve_t curve);
    changeResult_t setCCcurve(padCoordinate_t coordinate, int16_t curve);
    changeResult_t setCCvalue(padCoordinate_t coordinate, int16_t cc);
    changeResult_t setCClimit(padCoordinate_t coordinate, limitType_t limitType, int16_t value);
    void setCalibrationMode(bool state, padCoordinate_t type = coordinateX);
    changeResult_t calibratePressure(int8_t pad, int16_t limit, bool updateMIDIvalue = false);
    changeResult_t calibrateXY(int8_t pad, padCoordinate_t type, limitType_t limitType, int16_t limit, bool updateMIDIvalue = false);
    changeResult_t setPadNote(int8_t pad, note_t note);
    changeResult_t setOctave(int8_t shift, bool padEditMode = false);
    changeResult_t setScaleShiftLevel(int8_t shiftLevel, bool internalChange = false);
    changeResult_t setPitchBendType(pitchBendType_t type);
    changeResult_t setPitchBendState(bool state, padCoordinate_t coordinate);

    private:
    void getConfiguration();
    void getProgramParameters();
    void getScaleParameters();
    void getPadLimits();
    void getXLimits();
    void getYLimits();
    void getPressureLimits();
    void getAftertouchLimits();
    void getPadParameters();
    bool isAftertouchActivated(int8_t pad);
    dbSection_padCalibration_t getPressureZone(int8_t pad);

    void setPadPressState(int8_t pad, bool state);
    void updateNoteLEDs(int8_t pad, bool state);
    void updateLastPressedPad(int8_t pad, bool state);

    void checkMIDIdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    bool checkVelocity(int8_t pad, uint16_t value);
    bool checkNoteBuffer();
    bool checkAftertouch(int8_t pad, bool velocityAvailable, uint16_t value);
    bool checkX(int8_t pad);
    bool checkY(int8_t pad);
    void checkLCDdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);

    void sendNotes(int8_t pad, uint8_t velocity, bool state);
    void sendAftertouch(int8_t pad);
    void sendX(int8_t pad);
    void sendY(int8_t pad);

    changeResult_t updateFunctionLEDs(int8_t pad);
    void storeNotes(int8_t pad);
    void resetScale();

    ///
    /// \brief Arrays holding last MIDI values for X (CC), Y (CC) and Z (velocity and aftertouch) coordinates for each pad.
    /// @{

    uint8_t                 lastXCCvalue[NUMBER_OF_PADS],
                            lastYCCvalue[NUMBER_OF_PADS],
                            lastAftertouchValue[NUMBER_OF_PADS],
                            lastVelocityValue[NUMBER_OF_PADS];

    /// @}

    int16_t                 lastRawXValue[NUMBER_OF_PADS],
                            lastRawYValue[NUMBER_OF_PADS];

    ///
    /// \brief Initial X and Y positions once pad has been pressed for all pads.
    /// Used for pitch bend type 2.
    ///
    uint16_t                initialXposition[NUMBER_OF_PADS],
                            initialYposition[NUMBER_OF_PADS];

    ///
    /// \brief Arrays holding pitch bend X/Y value for all pads.
    /// @{

    uint16_t                lastXPitchBendValue[NUMBER_OF_PADS],
                            lastYPitchBendValue[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Variable holding last MIDI note state for all pads (true if note on was last event, false if note off).
    /// \warning Variable type assumes there can be no more than 16 pads since each bit holds value for single pad.
    ///
    uint16_t                lastMIDInoteState;

    ///
    /// \brief Array holding initial pressure value for all pads (raw value 0-1023).
    /// Used to detect whether pressure is increasing or decreasing.
    ///
    //int16_t                 initialPressureValue[NUMBER_OF_PADS];

    ///
    /// \brief Array holding CC controller number for every pad on X and Y coordinates.
    /// @{

    uint8_t                 ccXPad[NUMBER_OF_PADS],
                            ccYPad[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Arrays holding minimum and maximum values for CC messages for every pad on X and Y coordinates.
    /// @{

    uint8_t                 ccXminPad[NUMBER_OF_PADS],
                            ccXmaxPad[NUMBER_OF_PADS],
                            ccYminPad[NUMBER_OF_PADS],
                            ccYmaxPad[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Array holding curve numbers for every pad on X and Y coordinates.
    /// @{

    uint8_t                 padCurveX[NUMBER_OF_PADS],
                            padCurveY[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Variables holding send states for MIDI notes, CC messages on X and Y coordinates and aftertouch.
    /// \warning Variable type assumes there can be no more than 16 pads since each bit holds value for single pad.
    /// @{

    uint16_t                xSendEnabled,
                            ySendEnabled,
                            noteSendEnabled,
                            aftertouchSendEnabled;

    /// @}

    ///
    /// \brief Array holding upper limits (raw values, 0-1023) for pressure (used to scale velocity to MIDI value) on all pads.
    ///
    uint16_t                padPressureLimitUpper[NUMBER_OF_PADS];

    ///
    /// \brief Arrays holding lower and upper limits (raw values, 0-1023) for pressure (used to scale aftertouch to MIDI value) for all pads.
    /// @{

    uint16_t                padAftertouchLimitLower[NUMBER_OF_PADS],
                            padAftertouchLimitUpper[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Arrays holding lower and upper limits (raw values, 0-1023) for X and Y coordinates (used to scale X and Y values to MIDI values) for all pads.
    /// @{

    uint16_t                padXLimitLower[NUMBER_OF_PADS],
                            padXLimitUpper[NUMBER_OF_PADS],
                            padYLimitLower[NUMBER_OF_PADS],
                            padYLimitUpper[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Array holding MIDI notes for every pad.
    /// Each pad can have several notes. See value of NOTES_PER_PAD.
    ///
    uint8_t                 padNote[NUMBER_OF_PADS][NOTES_PER_PAD];

    ///
    /// \brief Array holding value of MIDI channel for every pad.
    ///
    uint8_t                 midiChannel[NUMBER_OF_PADS];

    ///
    /// \brief Holds active aftertouch type.
    ///
    aftertouchType_t        aftertouchType;

    ///
    /// \brief Holds maximum value of aftertouch among all pads.
    /// Used while channel aftertouch is active aftertouch type.
    ///
    uint8_t                 maxAftertouchValue;

    ///
    /// \brief Holds state of split mode.
    /// If set to true, split mode is enabled (disabled otherwise).
    ///
    bool                    splitEnabled;

    ///
    /// \brief Holds active scale.
    ///
    int8_t                  activeScale;

    ///
    /// \brief Holds active program.
    ///
    int8_t                  activeProgram;

    ///
    /// \brief Holds state of pad edit mode (true if pad edit mode is active, false otherwise).
    ///
    bool                    editModeState;

    ///
    /// \brief Holds active octave used in pad edit mode.
    ///
    int8_t                  activePadEditOctave;

    ///
    /// \brief Variable holding information whether aftertouch was activated on certain pad.
    /// \warning Variable type assumes there can be no more than 16 pads since each bit holds value for single pad.
    ///
    uint16_t                aftertouchActivated;

    ///
    /// \brief Holds current sensitivity level for velocity.
    ///
    velocitySensitivity_t   velocitySensitivity;

    ///
    /// \brief Holds current curve for velocity.
    ///
    curve_t                 velocityCurve;

    ///
    /// \brief Arrays holding last time in milliseconds values on X or Y coordinate have been changed for each pad.
    /// Used to detect value bounce.
    /// @{

    uint8_t                 xSendTimer[NUMBER_OF_PADS],
                            ySendTimer[NUMBER_OF_PADS];

    /// @}

    ///
    /// \brief Holds last time when either X or Y coordinates have changed value.
    ///
    uint32_t                lastXYchangeTime;

    ///
    /// \brief Array holding last time in milliseconds aftertouch values have been changed for each pad.
    /// Used to detect value bounce.
    ///
    uint8_t                 lastAftertouchUpdateTime[NUMBER_OF_PADS];

    ///
    /// \brief Variable used to detect whether new pressure reading is smaller than previous one for every pad.
    /// Once reduction has been detected, X and Y readings are ignored to avoid bouncing.
    /// \warning Variable type assumes there can be no more than 16 pads since each bit holds value for single pad.
    ///
    //uint16_t                pressureReduction;

    ///
    /// \brief Variables used to record pad press history.
    /// This implementation is similar to circular buffer and it's used to record order in which pads
    /// have been pressed and released.
    /// @{

    uint8_t                 padPressHistory_buffer[NUMBER_OF_PADS];
    uint8_t                 padPressHistory_counter;

    /// @}

    ///
    /// \brief Variables used to implement note buffer.
    /// Note buffer contains time and pad index and it's used to delay sending
    /// of MIDI notes after X and Y coordinates after defined amount of time.
    /// @{

    uint8_t                 pad_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t                 note_buffer_head;
    uint8_t                 note_buffer_tail;

    /// @}

    ///
    /// \brief Holds current state of calibration mode (true if enabled, false otherwise).
    ///
    bool                    calibrationEnabled;

    ///
    /// \brief Holds type of active calibration (coordinate which is being calibrated).
    /// Enumerated type (see padCoordinate_t enumeration).
    ///
    padCoordinate_t         activeCalibration;

    ///
    /// \brief Variables used for measuring time in seconds after which last read pressure value is considered calibrated value in calibration mode.
    /// @{

    uint8_t                 pressureCalibrationTime;
    uint32_t                pressureCalibrationLastChange;

    /// @}

    ///
    /// \brief Array holding last time index pad has been pressed.
    ///
    uint32_t                lastPadPressTime[NUMBER_OF_PADS];

    ///
    /// \brif Holds currently active pitch bend type.
    ///
    pitchBendType_t         pitchBendType;

    /// \brief Variables holding pitch bend enable state for all pads on X and Y coordinates
    /// If pitch bend is enabled for certain pad, pitch bend MIDI message is sent instead of CC messages.
    /// \warning Variable type assumes there can be no more than 16 pads since each bit holds value for single pad.
    /// @{

    uint16_t                pitchBendEnabledX,
                            pitchBendEnabledY;

    /// @}

};

extern Pads pads;

/// @}
