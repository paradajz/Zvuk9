#pragma once

#include "curves/Curves.h"
#include "scales/Scales.h"
#include "Calibration.h"
#include "../../board/Board.h"
#include "Config.h"

#define constrain(input, low, high) ((input)<(low)?(low):((input)>(high)?(high):(input)))

class Pads
{
    public:
    Pads();
    void init();
    void update();

    //program/scale
    //getters
    uint8_t getActiveProgram();
    uint8_t getActiveScale();
    //setters
    bool setActiveProgram(int8_t program);
    bool setActiveScale(int8_t scale);

    //pad edit mode
    bool getEditModeState();
    void setEditModeState(bool state, uint8_t pad = 0);
    void displayActivePadNotes(uint8_t pad);
    changeOutput_t assignPadNote(uint8_t pad, note_t note);
    void changeActiveOctave(bool direction);

    //functions on/off
    //getters
    bool getMIDISendState(onOff_t type, uint8_t padNumber);
    aftertouchType_t getAftertouchType();
    bool getSplitState();
    //setters
    void setMIDISendState(onOff_t type, bool state);
    void setAftertouchType(aftertouchType_t type);
    void setSplitState(bool state);

    //notes
    //getters
    uint8_t getActiveOctave();
    uint8_t getPadNote(uint8_t pad, uint8_t noteIndex);
    note_t getActiveTonic();
    bool noteActive(note_t note);
    note_t getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);
    //setters
    changeOutput_t shiftOctave(bool direction);
    changeOutput_t shiftNote(bool direction, bool internalChange = false);
    changeOutput_t setTonic(note_t note, bool internalChange = false);
    int8_t getNoteShiftLevel();

    //CC
    //getters
    uint8_t getCCvalue(padCoordinate_t type, uint8_t padNumber);
    uint8_t getCClimitValue(padCoordinate_t type, ccLimitType_t limitType, uint8_t padNumber);
    uint8_t getCCcurve(padCoordinate_t curve, uint8_t padNumber);
    //setters
    changeOutput_t changeCCvalue(bool direction, padCoordinate_t type, int8_t steps);
    changeOutput_t changeCClimitValue(bool direction, padCoordinate_t coordinate, ccLimitType_t limitType, int8_t steps);
    changeOutput_t setCCcurve(bool direction, padCoordinate_t coordinate, int8_t steps=1);

    //midi channel
    uint8_t getMIDIchannel(uint8_t pad);
    bool setMIDIchannel(uint8_t pad, uint8_t channel);

    //pad info
    uint8_t getLastTouchedPad();
    bool isPadPressed(uint8_t padNumber);
    bool allPadsReleased();

    //scale info
    bool isUserScale(uint8_t scale);
    bool isPredefinedScale(uint8_t scale);

    //pressure info
    pressureSensitivity_t getPressureSensitivity();
    void setPressureSensitivity(pressureSensitivity_t type);
    curve_t getPressureCurve();
    void setPressureCurve(curve_t curve);

    uint16_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);

    //calibration
    bool calibrate(padCoordinate_t type, calibrationDirection direction, uint8_t pad, uint16_t limit);
    void setCalibrationMode(bool state, padCoordinate_t type = coordinateX);
    void resetCalibration();

    private:

    //init
    void setupSysEx();

    //EEPROM config read
    void getConfiguration();
    void getProgramParameters();
    void getScaleParameters();
    void getPadLimits();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAftertouchLimits();
    void getPadParameters();

    //midi scaling
    uint8_t scalePressure(uint8_t pad, int16_t pressure, pressureType_t type);
    uint8_t scaleXY(uint8_t pad, int16_t xyValue, padCoordinate_t type);

    //data sampling/debouncing
    bool pressureStable(uint8_t padNumber, bool pressDetected);

    //data availability checks
    bool checkAftertouch(uint8_t pad, bool velocityAvailable);
    bool checkVelocity(uint8_t pad, uint16_t value);
    bool checkX(uint8_t pad, int16_t value);
    bool checkY(uint8_t pad, int16_t value);

    //pad press updating/info
    void setPadPressState(uint8_t padNumber, bool padState);
    void updateLastPressedPad(uint8_t pad, bool state);

    //lcd/led handling on midi event
    void handleNoteLEDs(uint8_t pad, bool state);
    void handleNoteLCD(uint8_t pad, uint8_t velocity, bool state);
    void handleXYlcd(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xEnabled, bool yEnabled);

    //scale
    scaleType_t getScaleType(int8_t scale);
    void generateScale(scale_t scale);

    //features on/off
    void setMIDISendState(onOff_t type, uint8_t padNumber, bool state);
    void setFunctionLEDs(uint8_t padNumber);

    //MIDI send
    void checkMIDIdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    bool checkNoteBuffer();
    void checkLCDdata(uint8_t pad, bool velocityAvailable, bool aftertouchAvailable, bool xAvailable, bool yAvailable);
    void sendAftertouch(uint8_t pad);
    void sendX(uint8_t pad);
    void sendY(uint8_t pad);
    void sendNotes(uint8_t pad, uint8_t velocity, bool state);

    //note buffer
    void storeNotes(uint8_t pad);

    //pad press history buffer
    void updatePressHistory(uint8_t pad);
    void clearTouchHistoryPad(uint8_t pad);

    //internal checking functions
    void checkRemainingOctaveShift();
    void checkRemainingNoteShift();

    //last midi values
    uint8_t                 lastXMIDIvalue[NUMBER_OF_PADS],
                            lastYMIDIvalue[NUMBER_OF_PADS];

    uint8_t                 lastVelocityValue[NUMBER_OF_PADS],
                            lastAftertouchValue[NUMBER_OF_PADS],
                            lastMIDInoteState[NUMBER_OF_PADS];

    //last raw pressure value
    //needed to get correct aftertouch value
    int16_t                 lastPressureValue[NUMBER_OF_PADS];

    //store press states for all pads inside this variable
    uint16_t                padPressed;

    //parameters from eeprom
    uint8_t                 ccXPad[NUMBER_OF_PADS],
                            ccYPad[NUMBER_OF_PADS],
                            ccXminPad[NUMBER_OF_PADS],
                            ccXmaxPad[NUMBER_OF_PADS],
                            ccYminPad[NUMBER_OF_PADS],
                            ccYmaxPad[NUMBER_OF_PADS];

    //store info for all pads in single uint16_t - we know there are only 9 pads
    uint16_t                xSendEnabled,
                            ySendEnabled,
                            noteSendEnabled,
                            aftertouchSendEnabled;

    int8_t                  padCurveX[NUMBER_OF_PADS],
                            padCurveY[NUMBER_OF_PADS];

    int16_t                 padPressureLimitUpper[NUMBER_OF_PADS],
                            padXLimitLower[NUMBER_OF_PADS],
                            padXLimitUpper[NUMBER_OF_PADS],
                            padYLimitLower[NUMBER_OF_PADS],
                            padYLimitUpper[NUMBER_OF_PADS],
                            padAftertouchLimitLower[NUMBER_OF_PADS],
                            padAftertouchLimitUpper[NUMBER_OF_PADS];

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

    uint32_t                padDebounceTimer[NUMBER_OF_PADS],
                            xSendTimer[NUMBER_OF_PADS],
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
    int16_t                 minCalibrationValue,
                            maxCalibrationValue;
    padCoordinate_t         activeCalibration;

    //pressure info
    pressureSensitivity_t   pressureSensitivity;
    curve_t                 pressureCurve;
};

extern Pads pads;
