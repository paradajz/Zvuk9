#ifndef PADS_H
#define PADS_H

#include "Arduino.h"
#include <avr/eeprom.h>
#include "Debug.h"
#include "hardware\pins\HardwareIDs.h"
#include "PadXYscales.h"
#include "midi\MIDI_parameters.h"
#include "PadsCalibration.h"
#include "Types.h"
#include "hardware/timer/TimerObject.h"
#include "midi/MIDI.h"

#define NUMBER_OF_PADS                      9

//DO NOT CHANGE
#define NUMBER_OF_SAMPLES                   3
#define NUMBER_OF_MEDIAN_RUNS               5

//multiplexer pins
const uint8_t muxCommonPins[] = { MUX_COMMON_PIN_0, MUX_COMMON_PIN_1, MUX_COMMON_PIN_2, MUX_COMMON_PIN_3 };
const uint8_t muxCommonPinsAnalogRead[] = { MUX_COMMON_PIN_AR_0, MUX_COMMON_PIN_AR_1, MUX_COMMON_PIN_AR_2, MUX_COMMON_PIN_AR_3 };
const uint8_t muxSelectPins[] = { MUX_SELECT_PIN_0, MUX_SELECT_PIN_1, MUX_SELECT_PIN_2, MUX_SELECT_PIN_3 };

const uint8_t padID[] = { PAD_0, PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8 };


class Pads  {

    public:

    Pads();
    void init();
    void update();

    void setPadEditMode(bool state);
    bool getPadEditMode();
    changeOutput changePadNote(uint8_t tonic);

    //calibration
    bool setLowerPressureLimit(uint8_t pad, uint16_t limit);
    bool setUpperPressureLimit(uint8_t pad, uint16_t limit);
    bool setLowerXLimit(uint8_t pad, uint16_t limit);
    bool setUpperXLimit(uint8_t pad, uint16_t limit);
    bool setLowerYLimit(uint8_t pad, uint16_t limit);
    bool setUpperYLimit(uint8_t pad, uint16_t limit);
    bool setAfterTouchPressureRatio(uint8_t ratio);

    //pad features control
    void notesOnOff();
    void afterTouchOnOff();
    void xOnOff();
    void yOnOff();
    void splitFunctions();

    void setPadNotes(uint8_t preset);
    bool noteActive(tonic _tonic);

    //note control
    changeOutput shiftOctave(bool direction);
    changeOutput shiftNote(bool direction);
    changeOutput setTonic(tonic _tonic);
    void changeActiveOctave(bool direction);

    //callbacks
    void setHandlePadEditModeCallback(void (*fptr)(uint8_t pad));
    void setHandleLEDstateCallback(void (*fptr)(uint8_t ledNumber, ledIntensity state));
    void setHandlePadPressCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY));
    void setHandlePadReleaseCallback(void (*fptr)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes));
    void setHandleLCDAfterTouchCallback(void (*fptr)(uint8_t pressure));
    void setHandleLCDxyCallback(void (*fptr)(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable));
    void setHandleClearPadDataCallback(void(*fptr)(uint8_t pad));

    //setters
    bool changeActiveProgram(bool direction);
    bool changeActivePreset_incDec(bool direction);
    bool changeActivePreset_direct(uint8_t _preset);
    changeOutput changeCC(bool direction, ccType type, int8_t steps);
    changeOutput changeXYlimits(bool direction, ccLimitType ccType, int8_t steps);
    changeOutput changeCurve(bool direction, curveCoordinate coordinate, int8_t steps=1);
    changeOutput changeMIDIchannel(bool direction);

    //getters
    //features - single
    bool getNoteSendEnabled(uint8_t padNumber);
    bool getAfterTouchSendEnabled(uint8_t padNumber);
    bool getCCXsendEnabled(uint8_t padNumber);
    bool getCCYsendEnabled(uint8_t padNumber);

    //used when changing program to determine enabled functions
    bool getNoteSendState(uint8_t);
    bool getAfterTouchSendState(uint8_t);
    bool getXSendState(uint8_t);
    bool getYSendState(uint8_t);

    tonic getActiveTonic();
    uint8_t getActiveOctave();
    uint8_t getMIDIchannel();
    uint8_t *getPadNotes(uint8_t padNumber);

    //split
    splitState getSplitState();
    ledIntensity getSplitStateLEDvalue();

    //CC parameters
    uint8_t getPadCCvalue(ccType type, uint8_t padNumber);
    uint8_t getPadCClimitValue(ccType type, ccLimitType limitType, uint8_t padNumber);
    curveType getPadCurve(curveCoordinate curve, uint8_t padNumber);

    //notes
    tonic getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);

    //last touched pad - 0 default
    uint8_t getLastTouchedPad();

    //check if selected pad is still pressed
    bool getPadPressed(uint8_t padNumber);

    uint8_t getActivePreset();

    bool setActivePreset(uint8_t preset);

    uint8_t getActiveProgram();

    private:

    void checkOctaveShift();
    bool checkPadsPressed();

    //init
    void initPadPins();
    void initVariables();
    void setUpADCtimer();

    //EEPROM config read
    void getPadConfig();
    void getProgramParameters();
    void getPresetParameters();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAfterTouchUpperPressureLimits();

    //hardware control
    void switchMuxPins();
    void setMuxInput(uint8_t muxInput);
    void setupPressure();
    int16_t getPressure();
    void setupX();
    int16_t getX();
    void setupY();
    int getY();
    void resetPadReadOrder();
    void setAnalogInput(uint8_t pin);

    bool readXY();
    void setReadXY(bool state);

    bool nextPad();
    void setNextPad();

    bool processPressure();
    bool processXY();
    void checkAftertouch();

    int16_t getMedianValueZXY(coordinateType);

    //adc
    void setFastADC();
    void analogReadStart();
    void enableADCinterrupt();
    int16_t getAnalogValue();
    bool analogReadInProgress();
    uint8_t getADCpinCounter();
    void setADCpinCounter(uint8_t value);

    //calibration
    uint8_t calibratePressure(uint8_t pad, int16_t pressure, pressureType type);
    uint8_t calibrateXY(uint8_t padNumber, int16_t xyValue, ccType type);

    //pad data processing
    void addPressureSamples(uint16_t pressure);
    void addXYSamples(uint16_t xValue,  uint16_t yValue);
    bool pressureSampled();
    bool xySampled();

    //pad press
    void setPadPressed(uint8_t padNumber, bool padState);
    bool pressureStable(uint8_t padNumber, uint8_t pressDetected);

    //callbacks
    void (*sendPadEditModeCallback)(uint8_t pad);
    void (*sendLEDstateCallback)(uint8_t ledNumber, ledIntensity state);
    void (*sendPadPressCallback)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY);
    void (*sendPadReleaseCallback)(uint8_t pad, uint8_t *notes, uint8_t numberOfNotes);
    void (*sendLCDAfterTouchCallback)(uint8_t pressure);
    void (*sendLCDxyCallback)(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
    void (*sendClearPadDataCallback)(uint8_t pad);

    //getters
    int16_t getEEPROMaddressFeatures(uint8_t padNumber);

    //program
    int16_t getActiveProgramAddress();
    bool setActiveProgram(int8_t program);

    //preset
    int16_t getActivePresetAddress();
    scaleType getActiveScaleType();

    //notes
    void setNoteSendEnabled(uint8_t padNumber, uint8_t state);
    void setNewPadNote(uint8_t padNumber, uint8_t note);
    void shiftPadNote(uint8_t padNumber, bool direction, uint8_t noteIndex, uint8_t range);
    void clearPadNote(uint8_t padNumber);
    uint8_t getNumberOfAssignedNotes(uint8_t padNumber);

    //aftertouch
    bool getAfterTouchGestureActivated(uint8_t padNumber, uint8_t calibratedPressure);
    void resetAfterTouchCounters(uint8_t padNumber);
    void setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state);

    //CC
    void setCCXsendEnabled(uint8_t padNumber, uint8_t state);
    void setCCYsendEnabled(uint8_t padNumber, uint8_t state);
    void setNewPadCC(uint8_t padNumber, uint8_t ccValue, ccType type, bool globalShift);
    void setNewPadCClimit(uint8_t padNumber, uint8_t value, ccLimitType limitType, bool globalShift);
    void setNewCurve(uint8_t padNumber, uint8_t newCurveValue, curveCoordinate coordinate, bool globalShift);

    //MIDI send
    void storePadNotes();
    void sendPadAftertouch();
    void sendPadXY();

    void checkXY();
    void checkVelocity();

    void getPadParameters();
    void setFunctionLEDs(uint8_t pad);

    bool pressureMIDIdataAvailable();
    bool afterTouchMIDIdataAvailable();
    bool xyMIDIdataAvailable();
    void checkNoteBuffer();
    void updateLastTouchedPad();
    void checkMIDIdata();

    uint8_t midiVelocity;
    bool midiNoteOnOff;
    uint8_t midiX;
    uint8_t midiY;
    bool velocityAvailable;
    bool xyAvailable;
    bool xAvailable;
    bool yAvailable;
    bool afterTouchAvailable;
    uint8_t midiAfterTouch;
    int8_t shiftedNote;

    uint8_t     selectedMuxChannel;

    bool        padEditMode;

    uint8_t     lastPadState[NUMBER_OF_PADS],
                sampleCounter;

    uint16_t    padPressed;

    int8_t      midiChannel;

    int16_t     lastPressureValue[NUMBER_OF_PADS],
                lastAfterTouchValue[NUMBER_OF_PADS],
                initialPressure[NUMBER_OF_PADS],
                initialXvalue[NUMBER_OF_PADS],
                initialYvalue[NUMBER_OF_PADS],
                lastXValue[NUMBER_OF_PADS],
                lastYValue[NUMBER_OF_PADS];

    uint8_t     lastXMIDIvalue[NUMBER_OF_PADS],
                lastYMIDIvalue[NUMBER_OF_PADS];

    int8_t      ccXPad[NUMBER_OF_PADS],
                ccYPad[NUMBER_OF_PADS],
                ccXminPad[NUMBER_OF_PADS],
                ccXmaxPad[NUMBER_OF_PADS],
                ccYminPad[NUMBER_OF_PADS],
                ccYmaxPad[NUMBER_OF_PADS];

    uint8_t     splitCounter;
    uint8_t     padSplitState;
    int8_t      localOctaveValue;

    uint8_t     padNote[NUMBER_OF_PADS][NOTES_PER_PAD];

    uint8_t     lastTouchedPad;
    int8_t      activePreset,
                activeProgram;
    uint32_t    lastSwitchTime;

    int16_t     padPressureLimitLower[NUMBER_OF_PADS],
                padPressureLimitUpper[NUMBER_OF_PADS],
                padXLimitLower[NUMBER_OF_PADS],
                padXLimitUpper[NUMBER_OF_PADS],
                padYLimitLower[NUMBER_OF_PADS],
                padYLimitUpper[NUMBER_OF_PADS],
                padPressureLimitUpperAfterTouch[NUMBER_OF_PADS];

    bool        afterTouchActivated[NUMBER_OF_PADS];

    uint32_t    afterTouchGestureTimer[NUMBER_OF_PADS];
    uint8_t     afterTouchGestureCounter[NUMBER_OF_PADS],
                lastAfterTouchGestureDirection[NUMBER_OF_PADS];

    uint8_t     activePad;
    bool        switchToNextPad;
    uint32_t    afterTouchSendTimer[NUMBER_OF_PADS];
    uint32_t    xSendTimer[NUMBER_OF_PADS],
                ySendTimer[NUMBER_OF_PADS];

    bool        initialXYignored[NUMBER_OF_PADS];
    uint8_t     sampleCounterPressure,
                sampleCounterXY;

    int16_t     xValueSamples[NUMBER_OF_SAMPLES],
                yValueSamples[NUMBER_OF_SAMPLES],
                pressureValueSamples[NUMBER_OF_SAMPLES];

    int8_t      padCurveX[NUMBER_OF_PADS],
                padCurveY[NUMBER_OF_PADS];

    bool switchToXYread;

    uint8_t     medianRunCounterXY,
                medianRunCounterPressure;

    int16_t     xAvg,
                yAvg,
                pressureAvg;

    uint32_t    padDebounceTimer[NUMBER_OF_PADS];
    bool padDebounceTimerStarted[NUMBER_OF_PADS];
    uint32_t lastPadCheckTime;

    bool        firstXYValueDelayTimerStarted[NUMBER_OF_PADS],
                firstPressureValueDelayTimerStarted[NUMBER_OF_PADS];

    uint32_t    firstXYValueDelayTimer[NUMBER_OF_PADS],
                firstPressureValueDelayTimer[NUMBER_OF_PADS];

    int8_t      previousPad;
    bool padMovementDetected;
    uint8_t lastVelocityValue[NUMBER_OF_PADS];

    bool        octaveShiftScheduled;
    int8_t      shiftAmount;

    bool        xSendEnabled[NUMBER_OF_PADS],
                ySendEnabled[NUMBER_OF_PADS],
                noteSendEnabled[NUMBER_OF_PADS],
                aftertouchSendEnabled[NUMBER_OF_PADS];

};

extern Pads pads;
#endif
