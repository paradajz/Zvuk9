#include "Pads.h"
#include "../lcd/menu/Menu.h"
#include <avr/cpufunc.h>

#define PAD_NOTE_BUFFER_SIZE    32

#define DEFAULT_XY_VALUE        -999

volatile int16_t adcValue = 0;
volatile bool adcConversionInProgress = false;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t adcPinReadOrder[] = {

    muxCommonPinsAnalogRead[2], //pressure, first reading, Y+
    muxCommonPinsAnalogRead[0], //pressure, second reading, X+
    muxCommonPinsAnalogRead[2], //y coordinate, X+
    muxCommonPinsAnalogRead[0]  //x coordinate, Y+

};

static uint8_t pad_buffer[PAD_NOTE_BUFFER_SIZE];
static uint8_t velocity_buffer[PAD_NOTE_BUFFER_SIZE];
static bool note_state_buffer[PAD_NOTE_BUFFER_SIZE];
static uint32_t pad_note_timer_buffer[PAD_NOTE_BUFFER_SIZE];
static uint8_t note_buffer_head = 0;
static uint8_t note_buffer_tail = 0;

const uint8_t debounceCompare = 0b11111100;

volatile uint8_t adcPinCounter = 0;
const uint8_t adcPinCounterMaxValue = 4;

void storePadNote(uint8_t pad, uint8_t velocity, bool noteState)  {

    uint8_t i = note_buffer_head + 1;
    if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;
    pad_buffer[i] = pad;
    velocity_buffer[i] = velocity;
    note_state_buffer[i] = noteState;
    pad_note_timer_buffer[i] = newMillis();
    note_buffer_head = i;

}

void Pads::enableADCinterrupt()    {

    ADCSRA |= (1<<ADIE);

}

void Pads::analogReadStart() {

    //set flag and start new analog conversion
    adcConversionInProgress = true;

    //single conversion mode
    ADCSRA |= (1<<ADSC);

}

void Pads::setAnalogInput(uint8_t pin)    {

    //read next analogue input
    ADMUX = (ADMUX & 0xF0) | (adcPinReadOrder[pin] & 0x0F);
    _NOP(); _NOP(); _NOP();

}

// Interrupt service routine for the ADC completion
ISR(ADC_vect)   {

    adcConversionInProgress = false;
    adcValue = ADC;

}

bool Pads::analogReadInProgress()    {

    cli();
    bool temp = adcConversionInProgress;
    sei();
    return temp;

}

int16_t Pads::getAnalogValue() {

    cli();
    int16_t adcValueCopy = adcValue;
    sei();

    return adcValueCopy;

}

inline uint8_t mapInternal(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max) {

    /*
    
    I mentioned earlier that the function�s actually working how it�s documented to work, just not how it�s usually used in examples. The map() docs state that �[t]he map() function uses integer math so will not generate fractions, when the math might indicate that it should do so. Fractional remainders are truncated, and are not rounded or averaged.�

    This completely makes sense � if you imagine a range of 1024 values between 0 and one, all of them will be less than 1 except the last value, and since it�s integer arithmetic, all the less-than-1 values are 0.

    The solution is fairly simple � increase the in_max and out_max args by one more than the actual maximum value (and then wrap the output in constrain(), which you ought to have done anyway). It�s fairly easy to work through why this works in your head, but here are the same examples I gave above with the increased maximums:

    map(0..1023, 0, 1024, 0, 2);
    0   512
    1   512
    map(0..1023, 0, 1024, 0, 16);
    0   64
    1   64
    2   64
    3   64
    4   64
    5   64
    6   64
    7   64
    8   64
    9   64
    10   64
    11   64
    12   64
    13   64
    14   64
    15   64
    
    */
    

    if ((in_min == out_min) && (in_max == out_max)) return x;

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pads::Pads()  {

    //default constructor

}

void Pads::init()   {

    initPadPins();
    initVariables();
    getPadConfig();
    setMuxInput(activePad);
    analogRead(adcPinReadOrder[0]); //dummy read to init ADC
    enableADCinterrupt();

}

void Pads::initVariables()  {

    //only init variables with custom init value (other than 0)

    for (int i=0; i<NUMBER_OF_PADS; i++)        {

        lastXValue[i] = DEFAULT_XY_VALUE;
        lastYValue[i] = DEFAULT_XY_VALUE;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activeProgram = -1;
    activePreset = -1;
    previousPad = -1;
    shiftAmount = 0;
    shiftedNote = 0;

}

uint8_t Pads::calibratePressure(uint8_t pad, int16_t pressure, pressureType_t type) {

    switch(type)  {

        case pressureAfterTouch:
        return map(constrain(pressure, padPressureLimitLower[pad], padPressureLimitUpperAfterTouch[pad]), padPressureLimitLower[pad], padPressureLimitUpperAfterTouch[pad], 0, 127);
        break;

        case pressureVelocity:
        return map(constrain(pressure, padPressureLimitLower[pad], padPressureLimitUpper[pad]), padPressureLimitLower[pad], padPressureLimitUpper[pad], 0, 127);
        break;

    }

    return 0;

}

uint8_t Pads::calibrateXY(uint8_t padNumber, int16_t xyValue, ccType_t type) {

    switch (type)   {

        case ccTypeX:
        return map(constrain(xyValue, padXLimitLower[padNumber], padXLimitUpper[padNumber]), padXLimitLower[padNumber], padXLimitUpper[padNumber], ccXminPad[padNumber], ccXmaxPad[padNumber]);

        case ccTypeY:
        return map(constrain(xyValue, padYLimitLower[padNumber], padYLimitUpper[padNumber]), padYLimitLower[padNumber], padYLimitUpper[padNumber], ccYminPad[padNumber], ccYmaxPad[padNumber]);

    }   return 0;

}

bool Pads::getPadPressed(uint8_t padNumber) {

    return bitRead(padPressed, padNumber);

}

void Pads::setPadPressed(uint8_t padNumber, bool padState) {

    bitWrite(padPressed, padNumber, padState);

}

bool Pads::pressureStable(uint8_t pad, uint8_t pressDetected)  {

    if (pressDetected) {

        if (!firstPressureValueDelayTimerStarted[pad])  {

            firstPressureValueDelayTimerStarted[pad] = true;
            padDebounceTimerStarted[pad] = false;
            firstPressureValueDelayTimer[pad] = newMillis();
            return false;

        }   return (newMillis() - firstPressureValueDelayTimer[pad] > PAD_PRESS_DEBOUNCE_TIME);


    } else {

        if (!padDebounceTimerStarted[pad])  {

            padDebounceTimerStarted[pad] = true;
            firstPressureValueDelayTimerStarted[pad] = false;
            padDebounceTimer[pad] = newMillis();
            return false;

        }   return (newMillis() - padDebounceTimer[pad] > PAD_RELEASE_DEBOUNCE_TIME);

    }

}

void Pads::addPressureSamples(uint16_t value) {

    pressureValueSamples[sampleCounterPressure] = value;
    sampleCounterPressure++;

}

bool Pads::pressureSampled()   {

    return (sampleCounterPressure == NUMBER_OF_SAMPLES);

}

bool Pads::processPressure() {

    int16_t pressure;

    pressure = getPressure();

    if (pressure == -1) return false;

    //we have pressure
    addPressureSamples(pressure);

    if (!pressureSampled()) return false;
    else {

        //reset pressure sample counter
        sampleCounterPressure = 0;
        return true;

    }

}

void Pads::checkVelocity()  {

    uint8_t pad = padID[activePad];

    //we've taken 3 pressure samples so far, get median value
    int16_t medianValue = getMedianValueZXY(coordinateZ);
    //calibrate pressure based on median value (0-1023 -> 0-127)
    uint8_t calibratedPressure = calibratePressure(pad, medianValue, pressureVelocity);

    bool pressDetected = (calibratedPressure > 0);

    if (pressureStable(pad, pressDetected))    {

        //pad reading is stable

        switch (pressDetected)    {

            case true:
            if (!bitRead(padPressed, pad)) {  //pad isn't already pressed

                //sensor is really pressed
                bitWrite(padPressed, pad, true);  //set pad pressed
                initialPressure[pad] = calibratedPressure;
                midiVelocity = calibratedPressure;
                midiNoteOnOff = true;
                velocityAvailable = true;
                padMovementDetected = true;

            }
            break;

            case false:
            if (bitRead(padPressed, pad))  {  //pad is already pressed

                midiVelocity = calibratedPressure;
                midiNoteOnOff = false;
                velocityAvailable = true;
                padMovementDetected = true;
                lastXValue[pad] = DEFAULT_XY_VALUE;
                lastYValue[pad] = DEFAULT_XY_VALUE;
                firstXYValueDelayTimerStarted[pad] = false;
                bitWrite(padPressed, pad, false);  //set pad not pressed
                //reset all aftertouch gestures after pad is released
                resetAfterTouchCounters(pad);
                switchToXYread = false;

            }
            break;

        }

        //send aftertouch only while sensor is pressed
        if (bitRead(padPressed, pad))  {

            lastPressureValue[pad] = medianValue;
            switchToXYread = true;

        }

    }

    if (!switchToXYread) switchToNextPad = true;

}

void Pads::setFunctionLEDs(uint8_t pad)   {

    if (splitCounter == 2)  {

        //split features
        //turn off function LEDs first
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_X, ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_Y, ledIntensityOff);

        //turn on feature LEDs depending on enabled features for last touched pad
        leds.setLEDstate(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendEnabled(pad) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_NOTES, getNoteSendEnabled(pad) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_X, getCCXsendEnabled(pad) ? ledIntensityFull : ledIntensityOff);
        leds.setLEDstate(LED_ON_OFF_Y, getCCYsendEnabled(pad) ? ledIntensityFull : ledIntensityOff);

    }

}

bool Pads::readXY() {

    return switchToXYread;

}

void Pads::setReadXY(bool state)    {

    switchToXYread = state;

}

bool Pads::processXY()  {

    //read x/y three times, get median value, then read x/y again until NUMBER_OF_MEDIAN_RUNS
    //get avg x/y value

    uint8_t pad = padID[activePad];

    static int16_t xValue = -1, yValue = -1;

    if (!firstXYValueDelayTimerStarted[pad]) {

        firstXYValueDelayTimerStarted[pad] = true;
        firstXYValueDelayTimer[pad] = newMillis();
        return false;

    } else {

        if (!(newMillis() - firstXYValueDelayTimer[pad] > PAD_X_Y_DEBOUNCE_TIME)) return false;

        //X/Y values should be more stable now

        if (xValue == -1) {

            #if XY_FLIP_AXIS > 0
                xValue = getY();
            #else
                xValue = getX();
            #endif

        }

        if (xValue == -1) return false;

        //we have x
        if (yValue == -1) {

            #if XY_FLIP_AXIS > 0
                yValue = getX();
            #else
                yValue = getY();
            #endif

        }
        if (!((xValue != -1) && (yValue != -1))) return false;    //we don't have y yet

        //if we got to this point, we have x and y coordinates
        addXYSamples(xValue, yValue);

        if (!xySampled()) return false;
        else {

            xValue = -1;
            yValue = -1;
            sampleCounterXY = 0;
            return true;

        }

    }

}

void Pads::setNextPad()    {

    switchToNextPad = false;
    activePad++;
    if (activePad == NUMBER_OF_PADS) activePad = 0;

}

void Pads::update(bool midiEnabled)  {

    static bool firstRun = true;    //true when switching to another pad

    if (firstRun) { setMuxInput(activePad); firstRun = false; } //only activate mux input once per pad

    if (!switchToXYread)    {

        bool pressureProcessed = processPressure();
        if (pressureProcessed)  {

            //all needed pressure samples are obtained
            checkVelocity();
            checkAftertouch();

        }

    } else {

        bool xyProcessed = processXY();
        if (xyProcessed)
            checkXY();

    }

    if (switchToNextPad)  {

        //if we got to this point, everything that can be read is read

        //check data to be sent

        if (padMovementDetected)   {

            updateLastTouchedPad();
            padMovementDetected = false;

        }

        if (!editModeActivated && !menu.menuDisplayed())   checkMIDIdata();
        firstRun = true;
        setNextPad();

    }   if (!editModeActivated && !menu.menuDisplayed())   checkNoteBuffer();  //send notes after some delay

    checkOctaveShift();

}

void Pads::updateLastTouchedPad()   {

    if (padID[activePad] != previousPad)
        setFunctionLEDs(padID[activePad]);

    if (padID[activePad] != lastTouchedPad) {

        if (editModeActivated)
            setupPadEditMode(padID[activePad]);

        if (getPadPressed(lastTouchedPad)) previousPad = lastTouchedPad;
            lastTouchedPad = padID[activePad];

    }

    if (previousPad != -1)  {

        if (!getPadPressed(previousPad) && !getPadPressed(lastTouchedPad))
        previousPad = -1;

    }

}

void Pads::addXYSamples(uint16_t xValue, uint16_t yValue)    {

    xValueSamples[sampleCounterXY] = xValue;
    yValueSamples[sampleCounterXY] = yValue;

    sampleCounterXY++;

}

bool Pads::xySampled() {

    return (sampleCounterXY == NUMBER_OF_SAMPLES);

}

int16_t Pads::getMedianValueZXY(coordinateType_t type)  {

    int16_t medianValue = 0;

    switch(type)  {

        case coordinateX:
        if ((xValueSamples[0] <= xValueSamples[1]) && (xValueSamples[0] <= xValueSamples[2]))
        {
            medianValue = (xValueSamples[1] <= xValueSamples[2]) ? xValueSamples[1] : xValueSamples[2];
        }
        else if ((xValueSamples[1] <= xValueSamples[0]) && (xValueSamples[1] <= xValueSamples[2]))
        {
            medianValue = (xValueSamples[0] <= xValueSamples[2]) ? xValueSamples[0] : xValueSamples[2];
        }
        else
        {
            medianValue = (xValueSamples[0] <= xValueSamples[1]) ? xValueSamples[0] : xValueSamples[1];
        }
        break;

        case coordinateY:
        if ((yValueSamples[0] <= yValueSamples[1]) && (yValueSamples[0] <= yValueSamples[2]))
        {
            medianValue = (yValueSamples[1] <= yValueSamples[2]) ? yValueSamples[1] : yValueSamples[2];
        }
        else if ((yValueSamples[1] <= yValueSamples[0]) && (yValueSamples[1] <= yValueSamples[2]))
        {
            medianValue = (yValueSamples[0] <= yValueSamples[2]) ? yValueSamples[0] : yValueSamples[2];
        }
        else
        {
            medianValue = (yValueSamples[0] <= yValueSamples[1]) ? yValueSamples[0] : yValueSamples[1];
        }
        break;

        case coordinateZ:
        if ((pressureValueSamples[0] <= pressureValueSamples[1]) && (pressureValueSamples[0] <= pressureValueSamples[2]))
        {
            medianValue = (pressureValueSamples[1] <= pressureValueSamples[2]) ? pressureValueSamples[1] : pressureValueSamples[2];
        }
        else if ((pressureValueSamples[1] <= pressureValueSamples[0]) && (pressureValueSamples[1] <= pressureValueSamples[2]))
        {
            medianValue = (pressureValueSamples[0] <= pressureValueSamples[2]) ? pressureValueSamples[0] : pressureValueSamples[2];
        }
        else
        {
            medianValue = (pressureValueSamples[0] <= pressureValueSamples[1]) ? pressureValueSamples[0] : pressureValueSamples[1];
        }
        break;

    }   return medianValue;

}

void Pads::checkXY()  {

    //store median value in these variables NUMBER_OF_MEDIAN_RUNS times, then get avg value
    static int16_t xAvg = 0;
    static int16_t yAvg = 0;

    xAvailable = false;
    yAvailable = false;

    uint8_t pad = padID[activePad];

    int16_t xValue = calibrateXY(pad, getMedianValueZXY(coordinateX), ccTypeX);
    int16_t yValue = calibrateXY(pad, getMedianValueZXY(coordinateY), ccTypeY);

    xAvg += xValue;
    yAvg += yValue;

    medianRunCounterXY++;
    if (medianRunCounterXY != NUMBER_OF_MEDIAN_RUNS) return;

    xAvg = xAvg / NUMBER_OF_MEDIAN_RUNS;
    yAvg = yAvg / NUMBER_OF_MEDIAN_RUNS;

    medianRunCounterXY = 0;

    xValue = xAvg;
    yValue = yAvg;

    xAvg = 0;
    yAvg = 0;

    bool xChanged = false;
    bool yChanged = false;

    if ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(xValue - lastXValue[pad]) > XY_SEND_TIMEOUT_STEP) xChanged = true;

    }   else if ((xValue != lastXValue[pad]) && ((newMillis() - xSendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
            xChanged = true;


    if ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT)  {

        if (abs(yValue - lastYValue[pad]) > XY_SEND_TIMEOUT_STEP) yChanged = true;

    }   else if ((yValue != lastYValue[pad]) && ((newMillis() - ySendTimer[pad]) > XY_SEND_TIMEOUT_IGNORE))
            yChanged = true;

    if (xChanged)   {

        lastXValue[pad] = xValue;
        xSendTimer[pad] = newMillis();

        if (padCurveX[pad] != 0)  xValue = xyScale[padCurveX[pad]-1][xValue];

        xAvailable = true;

    }

    if (yChanged)   {

        lastYValue[pad] = yValue;
        ySendTimer[pad] = newMillis();

        if (padCurveY[pad] != 0)  yValue = xyScale[padCurveY[pad]-1][yValue];

        yAvailable = true;

    }

    if (xChanged || yChanged)   {

        padDebounceTimer[pad] = newMillis();
        xyAvailable = true;
        padMovementDetected = true;
        midiX = xValue;
        midiY = yValue;

    }

    switchToXYread = false;
    switchToNextPad = true;

}

void Pads::sendPadXY()  {

    uint8_t pad = padID[activePad];

    bool xAvailable_ = getCCXsendEnabled(pad);
    bool yAvailable_ = getCCYsendEnabled(pad);

    if (xAvailable_) {

        if (xAvailable)   {

            #if MODE_SERIAL
                Serial.print(F("X for pad "));
                Serial.print(pad);
                Serial.print(F(": "));
                #if XY_FLIP_VALUES > 0
                    Serial.println(127-midiX);
                #else
                    Serial.println(midiX);
                #endif
                Serial.print(F("X CC: "));
                Serial.println(ccXPad[pad]);
            #else
                #if XY_FLIP_VALUES > 0
                    midi.sendControlChange(midiChannel, ccXPad[pad], 127-midiX);
                #else
                    midi.sendControlChange(midiChannel, ccXPad[pad], midiX);
                #endif
                lastXMIDIvalue[pad] = midiX;
            #endif

        }

    }

    if (yAvailable_) {

        if (yAvailable)   {

            #if MODE_SERIAL
                Serial.print(F("Y for pad "));
                Serial.print(pad);
                Serial.print(F(": "));
                #if XY_FLIP_VALUES > 0
                    Serial.println(midiY);
                #else
                    Serial.println(127-midiY);
                #endif
                Serial.print(F("Y CC: "));
                Serial.println(ccYPad[pad]);
            #else
                #if XY_FLIP_VALUES > 0
                    midi.sendControlChange(midiChannel, ccYPad[pad], midiY);
                #else
                    midi.sendControlChange(midiChannel, ccYPad[pad], 127-midiY);
                #endif
                lastYMIDIvalue[pad] = midiY;
            #endif

        }

    }

    #if XY_FLIP_VALUES > 0
        handleXY(pad, 127-midiX, midiY, xAvailable_, yAvailable_);
    #else
        handleXY(pad, midiX, 127-midiY, xAvailable_, yAvailable_);
    #endif

    //record first sent x/y values
    //if they change enough, reset aftertouch gesture counter
    if ((initialXvalue[pad] == -999) || (initialYvalue[pad] == -999))   {

        initialXvalue[pad] = midiX;
        initialYvalue[pad] = midiY;

    }   else {

        if (
            (abs(initialXvalue[pad] - midiX) > XY_CHANGE_AFTERTOUCH_RESET) ||
            (abs(initialYvalue[pad] - midiY) > XY_CHANGE_AFTERTOUCH_RESET)
         )  if (!afterTouchActivated[pad]) resetAfterTouchCounters(pad);

    }

    xyAvailable = false;

}

ledIntensity_t Pads::getSplitStateLEDvalue()    {

    switch (splitCounter)   {

        case 0:
        return ledIntensityOff;
        break;

        case 1:
        return ledIntensityDim;
        break;

        case 2:
        return ledIntensityFull;
        break;

    }   return ledIntensityOff;

}

uint8_t Pads::getNumberOfAssignedNotes(uint8_t padNumber)   {

    if (isPredefinedScale(activePreset)) return 1; //predefined presets only have one note

    uint8_t activatedNotesCounter = 0;
    uint8_t noteID = (activePreset - NUMBER_OF_PREDEFINED_SCALES)*(NUMBER_OF_PADS*NOTES_PER_PAD);

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (configuration.readParameter(CONF_BLOCK_USER_SCALE, padNotesSection, noteID+i+(NOTES_PER_PAD*padNumber)) != BLANK_NOTE)
            activatedNotesCounter++;

    }   return activatedNotesCounter;

}

void Pads::storePadNotes()  {

    uint8_t pad = padID[activePad];
    bool resetPadLCDdata = false;

    if ((previousPad != -1) && getPadPressed(previousPad) && !midiNoteOnOff)  {

        resetPadLCDdata = false;
        bool ccXsendEnabled = getCCXsendEnabled(previousPad);
        bool ccYsendEnabled = getCCYsendEnabled(previousPad);
        uint8_t ccXvaluePreviousPad = getPadCCvalue(ccTypeX, previousPad);
        uint8_t ccYvaluePreviousPad = getPadCCvalue(ccTypeY, previousPad);
        uint8_t ccXvalueActivePad = getPadCCvalue(ccTypeX, pad);
        uint8_t ccYvalueActivePad = getPadCCvalue(ccTypeY, pad);

        if ((ccXvalueActivePad == ccXvaluePreviousPad) && ccXsendEnabled)   {

            midi.sendControlChange(midiChannel, ccXvaluePreviousPad, lastXMIDIvalue[previousPad]);

        }   else if ((ccYvalueActivePad == ccYvaluePreviousPad) && ccYsendEnabled) {

            midi.sendControlChange(midiChannel, ccYvaluePreviousPad, lastYMIDIvalue[previousPad]);

        }

            if (getNoteSendEnabled(previousPad))
                handleNote(previousPad, lastVelocityValue[previousPad], true);

        handleXY(previousPad, lastXValue[previousPad], lastYValue[previousPad], ccXsendEnabled, ccYsendEnabled);
        setFunctionLEDs(previousPad);

    }   else if (!midiNoteOnOff) resetPadLCDdata = true;

    storePadNote(pad, midiVelocity, midiNoteOnOff);

    lastVelocityValue[pad] = midiVelocity;
    if (resetPadLCDdata)    {

        lcDisplay.clearPadData();

    }
    velocityAvailable = false;

}

note_t Pads::getTonicFromNote(uint8_t note)    {

    if (note == BLANK_NOTE) return MIDI_NOTES;
    return (note_t)(note % MIDI_NOTES);

}

uint8_t Pads::getOctaveFromNote(uint8_t note)  {

    if (note == BLANK_NOTE) return MIDI_OCTAVE_RANGE;
    return note / MIDI_OCTAVE_RANGE;

}

uint8_t Pads::getLastTouchedPad()   {

    return lastTouchedPad;

}

bool Pads::pressureMIDIdataAvailable()  {

    return velocityAvailable;

}

bool Pads::xyMIDIdataAvailable()    {

    return xyAvailable;

}

void Pads::checkMIDIdata()   {

    //send X/Y immediately
    if (xyMIDIdataAvailable())
        sendPadXY();

    //if notes are available, store them in buffer first
    if (pressureMIDIdataAvailable())
        storePadNotes();

    //send aftertouch immediately
    if (afterTouchMIDIdataAvailable())
        sendPadAftertouch();

}

void Pads::checkNoteBuffer()    {

    //send all notes from pads after PAD_NOTE_SEND_DELAY
    while (note_buffer_head != note_buffer_tail)    {

        uint8_t i = note_buffer_tail + 1;
        if (i >= PAD_NOTE_BUFFER_SIZE) i = 0;

        //check buffer until it's empty
        uint8_t pad = pad_buffer[i];
        uint8_t velocity = velocity_buffer[i];
        bool noteSendEnabled = getNoteSendEnabled(pad);
        bool noteState = note_state_buffer[i];
        uint32_t noteTime = pad_note_timer_buffer[i];

        //if note is on, and there's been less than PAD_NOTE_SEND_DELAY, exit function as it checks single pad at the time
        if (((newMillis() - noteTime) < PAD_NOTE_SEND_DELAY) && noteState) return;

        #if MODE_SERIAL
        if (noteSendEnabled)    {

            if (noteState)  {

                Serial.print(F("Pad "));
                Serial.print(pad);
                Serial.println(F(" pressed. Notes: "));

            }   else {

                Serial.print(F("\nPad "));
                Serial.print(pad);
                Serial.println(F(" off!"));
                Serial.println();

            }

        }
        #endif

        if (noteSendEnabled)    {

            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[pad][i] == BLANK_NOTE) continue;

                #if MODE_SERIAL
                    if (noteState) Serial.println(padNote[pad][i]);
                #else
                    if (noteState)
                        midi.sendNoteOn(midiChannel, padNote[pad][i], velocity);

                else {

                    if (afterTouchActivated[pad])   {

                        //send aftertouch 0 when pad is released
                        midi.sendAfterTouch(midiChannel, padNote[pad][i], 0);
                        afterTouchActivated[pad] = false;
                        afterTouchAvailable = false;

                    }

                    bool sendOff = true;
                    //some special considerations here
                    for (int j=0; j<NUMBER_OF_PADS; j++)    {

                        //don't check current pad
                        if (j == pad) continue;

                        //don't check released pads
                        if (!getPadPressed(j)) continue;

                        //only send note off if the same note isn't active on some other pad already
                        if (padNote[j][i] == padNote[pad][i])    {

                            sendOff = false;
                            break; //no need to check further

                        }

                    }

                    if (sendOff) midi.sendNoteOff(midiChannel, padNote[pad][i], 0);

                }
                #endif

            }

            #if MODE_SERIAL
            if (noteState)  {

                Serial.print(F("Velocity: "));
                Serial.println(velocity);

            }
            #endif

            noteState ? handleNote(pad, velocity, true) : handleNote(pad, velocity, false);

        }

        note_buffer_tail = i;

    }

}

void Pads::handleNote(uint8_t pad, uint8_t velocity, bool state)  {

    uint8_t noteArray[NOTES_PER_PAD],
            noteCounter = 0;

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[pad][i] != BLANK_NOTE)  {

            noteArray[noteCounter] = padNote[pad][i];
            noteCounter++;

        }

    }

    switch(state)   {

        case true:
        //note on
        uint8_t tonicArray[NOTES_PER_PAD],
                octaveArray[NOTES_PER_PAD];

        for (int i=0; i<noteCounter; i++) {

            tonicArray[i] = (uint8_t)getTonicFromNote(noteArray[i]);
            leds.setNoteLEDstate((note_t)tonicArray[i], ledIntensityFull);
            octaveArray[i] = normalizeOctave(getOctaveFromNote(noteArray[i]));

        }

        lcDisplay.updateNote(pad, tonicArray, octaveArray, noteCounter, velocity);
        break;

        case false:
        //note off
        //we need to set LEDs back to dim states for released pad, but only if
        //some other pad with same active note isn't already pressed

        bool noteActive;

        for (int z=0; z<noteCounter; z++) {

            //iterate over every note on released pad

            noteActive = false;

            for (int i=0; i<NUMBER_OF_PADS; i++)    {

                if (!getPadPressed(i)) continue; //skip released pad
                if (i == pad) continue; //skip current pad

                for (int j=0; j<NOTES_PER_PAD; j++) {

                    if (getTonicFromNote(padNote[i][j]) == getTonicFromNote(noteArray[z])) {

                        noteActive = true;

                    }

                }

            }   if (!noteActive) leds.setNoteLEDstate(getTonicFromNote((note_t)noteArray[z]), ledIntensityDim);

        }
        break;

    }

}

void Pads::handleXY(uint8_t pad, uint8_t xPosition, uint8_t yPosition, bool xAvailable, bool yAvailable)   {

    if (xAvailable || yAvailable)
        lcDisplay.setXYData(pad, xPosition, yPosition, xAvailable, yAvailable);

    //always display ccx/ccy
    lcDisplay.setCCData(pad, ccXPad[pad], ccYPad[pad]);

}

bool Pads::checkPadsPressed()   {

    //return true if any pad is pressed
    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (getPadPressed(i)) return true;

    return false;

}

bool Pads::noteActive(note_t note) {

    //return true if received tonic is among active notes on some pad

    for (int i=0; i<NUMBER_OF_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            if (padNote[i][j] != BLANK_NOTE)
                if (getTonicFromNote(padNote[i][j]) == note) return true;

    return false;

}

Pads pads;
