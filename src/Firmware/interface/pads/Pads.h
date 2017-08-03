#pragma once

#include "curves/Curves.h"
#include "Calibration.h"
#include "../../board/Board.h"
#include "Config.h"
#include "../../database/Database.h"
#include "Sanity.h"

#define constrain(input, low, high) ((input)<(low)?(low):((input)>(high)?(high):(input)))

class Pads
{
    public:
    Pads();

    void init();
    void update();

    //getters
    uint8_t getLastTouchedPad();
    bool isPadPressed(int8_t pad);
    uint8_t getNumberOfPressedPads();
    uint8_t getProgram();
    uint8_t getScale();
    bool isUserScale(int8_t scale);
    bool isPredefinedScale(int8_t scale);
    bool getEditModeState();
    note_t getTonic();
    uint8_t getOctave();
    bool getSplitState();
    bool getMIDISendState(int8_t pad, function_t type);
    aftertouchType_t getAftertouchType();
    uint8_t getPadNote(int8_t pad, int8_t noteIndex);
    bool isNoteAssigned(note_t note);
    note_t getTonicFromNote(int8_t note);
    uint8_t getOctaveFromNote(int8_t note);
    int8_t getScaleShiftLevel();
    uint8_t getCC(int8_t pad, padCoordinate_t type);
    uint8_t getCClimit(int8_t pad, padCoordinate_t type, limitType_t limitType);
    curve_t getCCcurve(int8_t pad, padCoordinate_t curve);
    velocitySensitivity_t getVelocitySensitivity();
    curve_t getVelocityCurve();
    uint8_t getMIDIchannel(int8_t pad);
    bool isCalibrationEnabled();
    padCoordinate_t getCalibrationMode();
    uint16_t getCalibrationLimit(int8_t pad, padCoordinate_t coordinate, limitType_t limitType);
    int8_t getPredefinedScaleNotes(scale_t scale);
    note_t getScaleNote(scale_t scale, int8_t index);
    uint8_t getScaledPressure(int8_t pad, uint16_t pressure, padCalibrationSection pressureZone, pressureType_t type);
    uint16_t getScaledXY(int8_t pad, uint16_t xyValue, padCoordinate_t type, bool midiScale);

    //setters
    bool setProgram(int8_t program);
    bool setScale(int8_t scale);
    changeResult_t setTonic(note_t note, bool internalChange = false);
    bool setOctave(int8_t octave);
    bool setEditModeState(bool state, int8_t pad = 0);
    bool setSplitState(bool state);
    bool setMIDISendState(function_t type, bool state);
    bool setAftertouchType(aftertouchType_t type);
    bool setMIDIchannel(int8_t pad, uint8_t channel);
    bool setVelocitySensitivity(velocitySensitivity_t type);
    bool setVelocityCurve(curve_t curve);
    bool setCCcurve(padCoordinate_t coordinate, curve_t curve);
    void setCalibrationMode(bool state, padCoordinate_t type = coordinateX);
    bool calibratePressure(int8_t pad, uint8_t pressureZone, int16_t limit);
    bool calibrateXY(int8_t pad, padCoordinate_t type, limitType_t limitType, int16_t limit);
    changeResult_t addNote(int8_t pad, note_t note);
    changeResult_t shiftOctave(bool direction);
    changeResult_t shiftNote(bool direction, bool internalChange = false);
    changeResult_t changeCCvalue(bool direction, padCoordinate_t type, int8_t steps);
    changeResult_t changeCClimitValue(bool direction, padCoordinate_t coordinate, limitType_t limitType, int8_t steps);

    private:
    //getters
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
    padCalibrationSection getActivePressureZone(int8_t pad);

    //setters
    void setPadPressState(int8_t pad, bool padState);

    //checking
    void checkMIDIdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    bool checkVelocity(int8_t pad);
    bool checkNoteBuffer();
    bool checkAftertouch(int8_t pad, bool velocityAvailable);
    bool checkX(int8_t pad);
    bool checkY(int8_t pad);
    void checkLCDdata(int8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    void checkRemainingOctaveShift();
    void checkRemainingNoteShift();

    //MIDI send
    void sendNotes(int8_t pad, uint8_t velocity, bool state);
    void sendAftertouch(int8_t pad);
    void sendX(int8_t pad);
    void sendY(int8_t pad);

    //misc
    void updateLastPressedPad(int8_t pad, bool state);
    void updateFunctionLEDs(int8_t pad);
    void updateNoteLEDs(int8_t pad, bool state);
    void storeNotes(int8_t pad);

    //last midi values
    uint8_t                 lastXMIDIvalue[NUMBER_OF_PADS],
                            lastYMIDIvalue[NUMBER_OF_PADS];

    uint8_t                 lastVelocityValue[NUMBER_OF_PADS],
                            lastAftertouchValue[NUMBER_OF_PADS];

    uint16_t                lastMIDInoteState;

    //last raw pressure value
    //needed to get correct aftertouch value
    int16_t                 lastPressureValue[NUMBER_OF_PADS];

    //parameters from eeprom
    uint8_t                 ccXPad[NUMBER_OF_PADS],
                            ccYPad[NUMBER_OF_PADS],
                            ccXminPad[NUMBER_OF_PADS],
                            ccXmaxPad[NUMBER_OF_PADS],
                            ccYminPad[NUMBER_OF_PADS],
                            ccYmaxPad[NUMBER_OF_PADS];

    uint8_t                 padCurveX[NUMBER_OF_PADS],
                            padCurveY[NUMBER_OF_PADS];

    //store info for all pads in single uint16_t - we know there are only 9 pads
    uint16_t                xSendEnabled,
                            ySendEnabled,
                            noteSendEnabled,
                            aftertouchSendEnabled;

    uint16_t                padPressureLimitUpper[NUMBER_OF_PADS][PRESSURE_CALIBRATION_ZONES],
                            padXLimitLower[NUMBER_OF_PADS],
                            padXLimitUpper[NUMBER_OF_PADS],
                            padYLimitLower[NUMBER_OF_PADS],
                            padYLimitUpper[NUMBER_OF_PADS],
                            padAftertouchLimitLower[NUMBER_OF_PADS][PRESSURE_CALIBRATION_ZONES],
                            padAftertouchLimitUpper[NUMBER_OF_PADS][PRESSURE_CALIBRATION_ZONES];

    uint8_t                 padNote[NUMBER_OF_PADS][NOTES_PER_PAD];

    uint8_t                 midiChannel[NUMBER_OF_PADS];
    aftertouchType_t        aftertouchType;
    uint8_t                 maxAftertouchValue;

    bool                    splitEnabled;

    int8_t                  activeOctave,
                            noteShiftLevel;

    int8_t                  activeScale,
                            activeProgram;

    //debouncing
    uint16_t                aftertouchActivated;

    uint8_t                 xSendTimer[NUMBER_OF_PADS],
                            ySendTimer[NUMBER_OF_PADS],
                            lastAftertouchUpdateTime[NUMBER_OF_PADS];

    uint16_t                pressureReduction;

    //used to shift octave once all pads are released
    int8_t                  octaveShiftAmount[NUMBER_OF_PADS];
    uint16_t                octaveShiftPadBuffer;

    //used to shift scale once all pads are released
    int8_t                  noteShiftAmount[NUMBER_OF_PADS];
    uint16_t                scaleShiftPadBuffer;

    //pad press history buffer
    uint8_t                 padPressHistory_buffer[NUMBER_OF_PADS];
    uint8_t                 padPressHistory_counter;

    //note buffer
    uint8_t                 pad_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t                 velocity_buffer[PAD_NOTE_BUFFER_SIZE];
    uint32_t                pad_note_timer_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t                 note_buffer_head;
    uint8_t                 note_buffer_tail;

    //calibration
    bool                    calibrationEnabled;
    uint8_t                 pressureCalibrationTime;
    uint32_t                pressureCalibrationLastChange;
    padCoordinate_t         activeCalibration;

    //pressure info
    velocitySensitivity_t   velocitySensitivity;
    curve_t                 velocityCurve;

    bool                    editModeActivated;
};

extern Pads pads;
