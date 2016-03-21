#ifndef PADS_H
#define PADS_H

#include "Arduino.h"
#include <avr/eeprom.h>
#include "../../Debug.h"
#include "../../hardware/pins/Pins.h"
#include "XYscales.h"
#include "../../midi/MIDI_parameters.h"
#include "Calibration.h"
#include "../../Types.h"
#include "../../hardware/timer/TimerObject.h"
#include "../../midi/MIDI.h"
#include "../../eeprom/EEPROMsettings.h"
#include "../lcd/LCD.h"
#include "../leds/LEDs.h"
#include "../../hardware/adc/ADC.h"
#include "../lcd/menu/Menu.h"

#define NUMBER_OF_PADS                      9

//DO NOT CHANGE
#define NUMBER_OF_SAMPLES                   3
#define NUMBER_OF_MEDIAN_RUNS               5 //only for x/y, pressure is read 3 times only

#define DEFAULT_XY_VALUE        -999
#define PAD_NOTE_BUFFER_SIZE    32

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] = { MUX_COMMON_PIN_0_INDEX, MUX_COMMON_PIN_1_INDEX, MUX_COMMON_PIN_2_INDEX, MUX_COMMON_PIN_3_INDEX };
const uint8_t padID[] = { PAD_0, PAD_1, PAD_2, PAD_3, PAD_4, PAD_5, PAD_6, PAD_7, PAD_8 };

class Pads  {

    public:

    //init
    Pads();
    void init();
    void update(bool midiEnabled = true);

    //calibration
    bool setLowerPressureLimit(uint8_t pad, uint16_t limit);
    bool setUpperPressureLimit(uint8_t pad, uint16_t limit);
    bool setLowerXLimit(uint8_t pad, uint16_t limit);
    bool setUpperXLimit(uint8_t pad, uint16_t limit);
    bool setLowerYLimit(uint8_t pad, uint16_t limit);
    bool setUpperYLimit(uint8_t pad, uint16_t limit);

    //program/preset
    //getters
    uint8_t getActiveProgram();
    uint8_t getActivePreset();
    //setters
    bool setActiveProgram(int8_t program);
    bool setActivePreset(uint8_t preset);

    //pad edit mode
    void setEditMode(bool state);
    bool editModeActive();
    void setupPadEditMode(uint8_t pad);
    void exitPadEditMode();
    void displayActivePadNotes(uint8_t pad);
    changeOutput_t assignPadNote(uint8_t pad, note_t note);
    void changeActiveOctave(bool direction);

    //functions on/off
    //getters
    bool getNoteSendEnabled(uint8_t padNumber);
    bool getAfterTouchSendEnabled(uint8_t padNumber);
    bool getCCXsendEnabled(uint8_t padNumber);
    bool getCCYsendEnabled(uint8_t padNumber);
    splitState_t getSplitState();
    //setters
    void notesOnOff();
    void aftertouchOnOff();
    void xOnOff();
    void yOnOff();
    void updateSplit();

    //notes
    //getters
    uint8_t getActiveOctave();
    uint8_t getPadNote(uint8_t pad, uint8_t note);
    note_t getActiveTonic();
    bool noteActive(note_t note);
    note_t getTonicFromNote(uint8_t note);
    uint8_t getOctaveFromNote(uint8_t note);
    //setters
    changeOutput_t shiftOctave(bool direction);
    changeOutput_t shiftNote(bool direction, bool internalChange = false);
    changeOutput_t setTonic(note_t note, bool internalChange = false);

    //CC
    //getters
    uint8_t getCCvalue(ccType_t type, uint8_t padNumber);
    uint8_t getCClimitValue(ccType_t type, ccLimitType_t limitType, uint8_t padNumber);
    curveType_t getCCcurve(curveCoordinate_t curve, uint8_t padNumber);
    //setters
    changeOutput_t changeCC(bool direction, ccType_t type, int8_t steps);
    changeOutput_t changeCClimits(bool direction, ccLimitType_t ccType, int8_t steps);
    changeOutput_t changeCCcurve(bool direction, curveCoordinate_t coordinate, int8_t steps=1);

    //midi channel
    uint8_t getMIDIchannel();
    bool setMIDIchannel(uint8_t channel);

    //pad info
    uint8_t getPadPressHistoryIndex(padHistoryID_t id);
    bool isPadPressed(uint8_t padNumber);

    //scale info
    bool isUserScale(uint8_t scale);
    bool isPredefinedScale(uint8_t scale);

    private:

    //init
    void initPadPins();
    void initHardware();

    //EEPROM config read
    void getConfiguration();
    void getProgramParameters();
    void getPresetParameters();
    void getPadLimits();
    void getPressureLimits();
    void getXLimits();
    void getYLimits();
    void getAfterTouchUpperPressureLimits();
    void getPadParameters();

    //hardware control
    void setMuxInput(uint8_t muxInput);
    void setNextPad();
    void setupPressure();
    int16_t getPressure();
    void setupX();
    int16_t getX();
    void setupY();
    int16_t getY();

    //midi scaling
    uint8_t scalePressure(uint8_t pad, int16_t pressure, pressureType_t type);
    uint8_t scaleXY(uint8_t padNumber, int16_t xyValue, ccType_t type);

    //data sampling/debouncing
    void addPressureSamples(uint16_t pressure);
    void addXYSamples(uint16_t xValue,  uint16_t yValue);
    bool pressureSampled();
    bool xySampled();
    bool pressureStable(uint8_t padNumber, uint8_t pressDetected);
    int16_t getMedianValueXYZ(coordinateType_t coordinate);

    //data processing
    bool processPressure();
    bool processXY();
    void checkOctaveShift();

    //data availability checks
    void checkAftertouch();
    void checkXY();
    void checkVelocity();

    //pad press updating/info
    void setPadPressed(uint8_t padNumber, bool padState);
    void updateLastTouchedPad();
    bool allPadsReleased();

    //lcd/led handling on midi event
    void handleNote(uint8_t pad, uint8_t velocity, bool state);
    void handleXY(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xAvailable, bool yAvailable);

    //scale
    scaleType_t getScaleType(uint8_t scale);
    void generateScale(scale_t scale);

    //features on/off
    void setNoteSendEnabled(uint8_t padNumber, uint8_t state);
    void setCCXsendEnabled(uint8_t padNumber, uint8_t state);
    void setCCYsendEnabled(uint8_t padNumber, uint8_t state);
    void setAfterTouchSendEnabled(uint8_t padNumber, uint8_t state);
    void setFunctionLEDs(uint8_t padNumber);

    //aftertouch
    bool getAfterTouchGestureActivated(uint8_t padNumber, uint8_t calibratedPressure);
    void resetAfterTouchCounters(uint8_t padNumber);

    //MIDI send
    void checkMIDIdata();
    void sendAftertouch(uint8_t pad);
    void sendXY(uint8_t pad);
    void sendNotes(uint8_t pad, uint8_t velocity, bool state);

    //note buffer
    void storeNotes(uint8_t pad);

    //pad press history buffer
    void updatePressHistory(uint8_t pad);

    //current midi values
    uint8_t     midiVelocity;
    uint8_t     midiX;
    uint8_t     midiY;
    bool        midiNoteOn;
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

    //indicate if there are new values to be sent
    bool        velocityAvailable;
    bool        xyAvailable;
    bool        xAvailable;
    bool        yAvailable;
    bool        afterTouchAvailable;

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
                firstPressureValueDelayTimerStarted[NUMBER_OF_PADS],
                padMovementDetected;

    uint32_t    padDebounceTimer[NUMBER_OF_PADS],
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

    //used to shift octave once all pads are released
    int8_t      octaveShiftAmount;

    //pad press history buffer
    uint8_t padPressHistory_buffer[NUMBER_OF_PADS];
    uint8_t padPressHistory_counter;

    //note buffer
    uint8_t pad_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t velocity_buffer[PAD_NOTE_BUFFER_SIZE];
    uint32_t pad_note_timer_buffer[PAD_NOTE_BUFFER_SIZE];
    uint8_t note_buffer_head;
    uint8_t note_buffer_tail;

};

extern Pads pads;
#endif
