#ifndef PADS_H
#define PADS_H

#include "Arduino.h"
#include <avr/eeprom.h>
#include "../../Debug.h"
#include "../../hardware/pins/Pins.h"
#include "PadXYscales.h"
#include "../../midi/MIDI_parameters.h"
#include "PadsCalibration.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../midi/MIDI.h"
#include "../../eeprom/EEPROMsettings.h"
#include "../lcd/LCD.h"
#include "../leds/LEDs.h"

#define NUMBER_OF_PADS                      9

//DO NOT CHANGE
#define NUMBER_OF_SAMPLES                   3
#define NUMBER_OF_MEDIAN_RUNS               5 //only for x/y, pressure is read 3 times only

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] = { MUX_COMMON_PIN_0_INDEX, MUX_COMMON_PIN_1_INDEX, MUX_COMMON_PIN_2_INDEX, MUX_COMMON_PIN_3_INDEX };
const uint8_t padID[] = { PAD_0, PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8 };

class Pads  {

    public:

    Pads();
    void init();
    void update(bool midiEnabled = true);
    changeOutput_t assignPadNote(uint8_t pad, uint8_t tonic);

    //program
    bool setActiveProgram(int8_t program);

    //pad edit
    void setupPadEditMode(uint8_t pad);
    void displayActivePadNotes(uint8_t pad);
    void setNoteLEDs();
    bool editModeActive() {return editModeActivated; }
    void setEditMode(bool state) {editModeActivated = state; };
    void exitPadEditMode();

    void notesOnOff();
    void aftertouchOnOff();
    void xOnOff();
    void yOnOff();

    uint8_t getPadNote(uint8_t pad, uint8_t note);

    changeOutput_t changePadNote(uint8_t tonic);

    //calibration
    bool setLowerPressureLimit(uint8_t pad, uint16_t limit);
    bool setUpperPressureLimit(uint8_t pad, uint16_t limit);
    bool setLowerXLimit(uint8_t pad, uint16_t limit);
    bool setUpperXLimit(uint8_t pad, uint16_t limit);
    bool setLowerYLimit(uint8_t pad, uint16_t limit);
    bool setUpperYLimit(uint8_t pad, uint16_t limit);

    void setSplit();

    bool noteActive(note_t note);

    //note control
    changeOutput_t shiftOctave(bool direction);
    changeOutput_t shiftNote(bool direction, bool internalChange = false);
    changeOutput_t setTonic(note_t note, bool internalChange = false);
    void changeActiveOctave(bool direction);

    //setters
    changeOutput_t changeCC(bool direction, ccType_t type, int8_t steps);
    changeOutput_t changeXYlimits(bool direction, ccLimitType_t ccType, int8_t steps);
    changeOutput_t changeCurve(bool direction, curveCoordinate_t coordinate, int8_t steps=1);
    changeOutput_t setMIDIchannel(uint8_t channel);

    //getters
    //features - single
    bool getNoteSendEnabled(uint8_t padNumber);
    bool getAfterTouchSendEnabled(uint8_t padNumber);
    bool getCCXsendEnabled(uint8_t padNumber);
    bool getCCYsendEnabled(uint8_t padNumber);

    note_t getActiveTonic();
    uint8_t getActiveOctave();
    uint8_t getMIDIchannel();

    //split
    splitState_t getSplitState();
    ledIntensity_t getSplitStateLEDvalue();

    //CC parameters
    uint8_t getPadCCvalue(ccType_t type, uint8_t padNumber);
    uint8_t getPadCClimitValue(ccType_t type, ccLimitType_t limitType, uint8_t padNumber);
    curveType_t getPadCurve(curveCoordinate_t curve, uint8_t padNumber);

    //notes
    note_t getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);

    //last touched pad - 0 default
    uint8_t getLastTouchedPad();

    //check if selected pad is still pressed
    bool getPadPressed(uint8_t padNumber);

    uint8_t getActivePreset();

    bool setActivePreset(uint8_t preset);

    uint8_t getActiveProgram();
    bool isUserScale(uint8_t scale);
    bool isPredefinedScale(uint8_t scale);

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
    void getPadLimits();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAfterTouchUpperPressureLimits();

    //hardware control
    void setMuxInput(uint8_t muxInput);
    void setupPressure();
    int16_t getPressure();
    void setupX();
    int16_t getX();
    void setupY();
    int getY();
    void setAnalogInput(uint8_t pin);

    void setNextPad();

    bool processPressure();
    bool processXY();
    void checkAftertouch();

    int16_t getMedianValueZXY(coordinateType_t);

    //adc
    void analogReadStart();
    void enableADCinterrupt();
    int16_t getAnalogValue();
    bool analogReadInProgress();

    //calibration
    uint8_t calibratePressure(uint8_t pad, int16_t pressure, pressureType_t type);
    uint8_t calibrateXY(uint8_t padNumber, int16_t xyValue, ccType_t type);

    //pad data processing
    void addPressureSamples(uint16_t pressure);
    void addXYSamples(uint16_t xValue,  uint16_t yValue);
    bool pressureSampled();
    bool xySampled();

    //pad press
    void setPadPressed(uint8_t padNumber, bool padState);
    bool pressureStable(uint8_t padNumber, uint8_t pressDetected);

    //sent only if midi send is enabled
    void handleNote(uint8_t pad, uint8_t velocity, bool state);
    void handleXY(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xAvailable, bool yAvailable);
    //preset
    scaleType_t getScaleType(uint8_t scale);

    //notes
    void setNoteSendEnabled(uint8_t padNumber, uint8_t state);
    uint8_t getNumberOfAssignedNotes(uint8_t padNumber);

    //aftertouch
    bool getAfterTouchGestureActivated(uint8_t padNumber, uint8_t calibratedPressure);
    void resetAfterTouchCounters(uint8_t padNumber);
    void setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state);

    //CC
    void setCCXsendEnabled(uint8_t padNumber, uint8_t state);
    void setCCYsendEnabled(uint8_t padNumber, uint8_t state);

    //MIDI send
    void storePadNotes();
    void sendPadAftertouch();
    void sendPadXY();

    void checkXY();
    void checkVelocity();

    void getPadParameters();
    void setFunctionLEDs(uint8_t pad);

    void checkNoteBuffer();
    void updateLastTouchedPad();
    void checkMIDIdata();

    void generateScale(scale_t scale);

    //current midi values
    uint8_t     midiVelocity;
    uint8_t     midiX;
    uint8_t     midiY;
    bool        midiNoteOnOff;
    uint8_t     midiAfterTouch;

    //last midi values
    uint8_t     lastXMIDIvalue[NUMBER_OF_PADS],
                lastYMIDIvalue[NUMBER_OF_PADS],
                lastVelocityValue[NUMBER_OF_PADS],
                lastAfterTouchValue[NUMBER_OF_PADS];

    //last raw values
    int16_t     lastXValue[NUMBER_OF_PADS],
                lastYValue[NUMBER_OF_PADS],
                lastPressureValue[NUMBER_OF_PADS];

    //median value samples get stored here (3 samples)
    int16_t     xValueSamples[NUMBER_OF_SAMPLES],
                yValueSamples[NUMBER_OF_SAMPLES],
                pressureValueSamples[NUMBER_OF_SAMPLES];

    bool        velocityAvailable;
    bool        xyAvailable;
    bool        xAvailable;
    bool        yAvailable;
    bool        afterTouchAvailable;
    bool        editModeActivated;

    //store press states for all pads inside this variable
    uint16_t    padPressed;

    //used for aftertouch gesture activation
    int16_t     initialPressure[NUMBER_OF_PADS],
                initialXvalue[NUMBER_OF_PADS],
                initialYvalue[NUMBER_OF_PADS];

    bool        afterTouchActivated[NUMBER_OF_PADS];

    uint32_t    afterTouchGestureTimer[NUMBER_OF_PADS],
                afterTouchSendTimer[NUMBER_OF_PADS];

    uint8_t     afterTouchGestureCounter[NUMBER_OF_PADS],
                lastAfterTouchGestureDirection[NUMBER_OF_PADS];

    //parameters from eeprom
    int8_t      ccXPad[NUMBER_OF_PADS],
                ccYPad[NUMBER_OF_PADS],
                ccXminPad[NUMBER_OF_PADS],
                ccXmaxPad[NUMBER_OF_PADS],
                ccYminPad[NUMBER_OF_PADS],
                ccYmaxPad[NUMBER_OF_PADS];

    bool        xSendEnabled[NUMBER_OF_PADS],
                ySendEnabled[NUMBER_OF_PADS],
                noteSendEnabled[NUMBER_OF_PADS],
                aftertouchSendEnabled[NUMBER_OF_PADS];

    int8_t      padCurveX[NUMBER_OF_PADS],
                padCurveY[NUMBER_OF_PADS];

    int16_t     padPressureLimitLower[NUMBER_OF_PADS],
                padPressureLimitUpper[NUMBER_OF_PADS],
                padXLimitLower[NUMBER_OF_PADS],
                padXLimitUpper[NUMBER_OF_PADS],
                padYLimitLower[NUMBER_OF_PADS],
                padYLimitUpper[NUMBER_OF_PADS],
                padPressureLimitUpperAfterTouch[NUMBER_OF_PADS];

    uint8_t     padNote[NUMBER_OF_PADS][NOTES_PER_PAD];

    int8_t      midiChannel;

    uint8_t     splitCounter;

    int8_t      activeOctave;

    int8_t      activePreset,
                activeProgram;

    int8_t      noteShiftAmount;

    //debouncing
    bool        padDebounceTimerStarted[NUMBER_OF_PADS],
                firstXYValueDelayTimerStarted[NUMBER_OF_PADS],
                firstPressureValueDelayTimerStarted[NUMBER_OF_PADS],
                initialXYignored[NUMBER_OF_PADS],
                padMovementDetected;

    uint32_t    padDebounceTimer[NUMBER_OF_PADS],
                firstXYValueDelayTimer[NUMBER_OF_PADS],
                firstPressureValueDelayTimer[NUMBER_OF_PADS],
                xSendTimer[NUMBER_OF_PADS],
                ySendTimer[NUMBER_OF_PADS];

    uint8_t     medianRunCounterXY,
                sampleCounterPressure,
                sampleCounterXY;

    //pad read control
    uint8_t     activePad;
    bool        switchToNextPad,
                switchToXYread;

    //previous/last touched pad detection
    uint8_t     lastTouchedPad;
    int8_t      previousPad;

    //used to shift octave once all pads are released
    int8_t      octaveShiftAmount;

};

extern Pads pads;
#endif
