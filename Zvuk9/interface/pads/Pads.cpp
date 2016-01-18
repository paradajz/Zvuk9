#include "Pads.h"
#include "eeprom/EEPROMsettings.h"
#include <avr/cpufunc.h>

#define PAD_NOTE_BUFFER_SIZE    32

#define DEFAULT_XY_VALUE        -999

volatile int16_t adcValue = 0;
volatile bool adcConversionInProgress = false;

uint8_t adcPinReadOrder[] = {

    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[1], //pressure, second reading
    muxCommonPinsAnalogRead[1], //x coordinate
    muxCommonPinsAnalogRead[3]  //y coordinate

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
    //while (tx_buffer_tail == i) ; // wait until space in buffer
    pad_buffer[i] = pad;
    velocity_buffer[i] = velocity;
    note_state_buffer[i] = noteState;
    pad_note_timer_buffer[i] = newMillis();
    note_buffer_head = i;

    //uint16_t i = (uint16_t)(buffer->head + 1) % PAD_NOTE_BUFFER_SIZE;
//
    //if (i != buffer->tail) {
//
        //buffer->padBuffer[buffer->head] = pad;
        //buffer->velocityBuffer[buffer->head] = velocity;
        //buffer->noteStateBuffer[buffer->head] = noteState;
        //buffer->padNoteTimerBuffer[buffer->head] = newMillis();
        //buffer->head = i;
//
    //}

}

void Pads::enableADCinterrupt()    {

    cli();
    ADCSRA |= (1<<ADIE);
    sei();

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
    
    I mentioned earlier that the function’s actually working how it’s documented to work, just not how it’s usually used in examples. The map() docs state that “[t]he map() function uses integer math so will not generate fractions, when the math might indicate that it should do so. Fractional remainders are truncated, and are not rounded or averaged.”

    This completely makes sense – if you imagine a range of 1024 values between 0 and one, all of them will be less than 1 except the last value, and since it’s integer arithmetic, all the less-than-1 values are 0.

    The solution is fairly simple – increase the in_max and out_max args by one more than the actual maximum value (and then wrap the output in constrain(), which you ought to have done anyway). It’s fairly easy to work through why this works in your head, but here are the same examples I gave above with the increased maximums:

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


Pads::Pads()  {

    //default constructor
    sendLEDstateCallback        = NULL;
    sendPadPressCallback        = NULL;
    sendPadReleaseCallback      = NULL;
    sendLCDAfterTouchCallback   = NULL;
    sendLCDxyCallback           = NULL;
    sendClearPadDataCallback    = NULL;
    sendPadEditModeCallback     = NULL;

}

void Pads::init()   {

    initPadPins();
    initVariables();
    setMuxInput(activePad);
    newDelay(100);
    analogRead(adcPinReadOrder[0]); //dummy read to init ADC
    enableADCinterrupt();
    getPadConfig();

}

void Pads::initVariables()  {

    //only init variables with custom init value (other than 0)

    for (int i=0; i<NUMBER_OF_PADS; i++)        {

        lastXValue[i]                       = DEFAULT_XY_VALUE;
        lastYValue[i]                       = DEFAULT_XY_VALUE;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activePreset                            = getActivePreset();
    previousPad                             = -1;

}

void Pads::getPadConfig()   {

    //read pad configuration from EEPROM

    getProgramParameters();
    getPresetParameters();
    getPressureLimits();
    getXLimits();
    getYLimits();
    getAfterTouchUpperPressureLimits();

    midiChannel = eeprom_read_byte((uint8_t*)EEPROM_MIDI_CHANNEL);

}

void Pads::getProgramParameters()   {

    activeProgram = getActiveProgram();
    midiChannel = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_MIDI_CHANNEL_ID);

    #if MODE_SERIAL
        Serial.print(F("Active program: "));
        Serial.println(activeProgram+1);
    #endif

    getPadParameters();

}

void Pads::getPadParameters()   {

    splitCounter = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_XY_SPLIT_STATE_ID);

    if (splitCounter == 0)  {   //split off

        //apply global settings to pads

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            xSendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID);
            ySendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID);
            noteSendEnabled[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID);
            aftertouchSendEnabled[i]        = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID);
            ccXPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_X_ID);
            ccYPad[i]                       = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_CC_Y_ID);
            ccXminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MIN_ID);
            ccXmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_MAX_ID);
            ccYminPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MIN_ID);
            ccYmaxPad[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_MAX_ID);
            padCurveX[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_CURVE_ID);
            padCurveY[i]                    = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_CURVE_ID);

        }

    }   else if (splitCounter == 1) {   //split x/y

            //functions are still global
            xSendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_X_ENABLE_ID);
            ySendEnabled[i]                 = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_Y_ENABLE_ID);
            noteSendEnabled[i]              = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_NOTE_ENABLE_ID);
            aftertouchSendEnabled[i]        = configuration.readParameter(CONF_BLOCK_PROGRAM, programGlobalSettingsSection, GLOBAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID);

            //pads have individual settings
            for (int i=0; i<NUMBER_OF_PADS; i++)    {

                ccXPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_X_ID);
                ccYPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_Y_ID);
                ccXminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MIN_ID);
                ccXmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MAX_ID);
                ccYminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MIN_ID);
                ccYmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MAX_ID);
                padCurveX[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_CURVE_ID);
                padCurveY[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_CURVE_ID);

            }

        }   else {  //split xy + functions

            //pads have individual settings and functions
            for (int i=0; i<NUMBER_OF_PADS; i++)    {

                xSendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_ENABLE_ID);
                ySendEnabled[i]             = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_ENABLE_ID);
                noteSendEnabled[i]          = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_NOTE_ENABLE_ID);
                aftertouchSendEnabled[i]    = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_AFTERTOUCH_ENABLE_ID);
                ccXPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_X_ID);
                ccYPad[i]                   = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_CC_Y_ID);
                ccXminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MIN_ID);
                ccXmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_MAX_ID);
                ccYminPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MIN_ID);
                ccYmaxPad[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_MAX_ID);
                padCurveX[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_X_CURVE_ID);
                padCurveY[i]                = configuration.readParameter(CONF_BLOCK_PROGRAM, programLocalSettingsSection, LOCAL_PROGRAM_SETTING_Y_CURVE_ID);

            }

    }

    sendLEDstateCallback(LED_ON_OFF_SPLIT, getSplitStateLEDvalue());
    sendLEDstateCallback(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendState(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_NOTES, getNoteSendState(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_X, getXSendState(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);
    sendLEDstateCallback(LED_ON_OFF_Y, getYSendState(lastTouchedPad) ? ledIntensityFull : ledIntensityOff);

}

void Pads::getPresetParameters()    {

    #if MODE_SERIAL
        Serial.print(F("Active preset: "));
        Serial.println(activePreset);
    #endif

    //clear all pad notes before assigning new ones
    for (int i=0; i<NUMBER_OF_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    setPadNotes(activePreset);

    #if MODE_SERIAL
    if (activePreset < NUMBER_OF_PREDEFINED_SCALES)    {

        Serial.println("Default notes for current preset: ");
        int16_t address = getActivePresetAddress();
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            Serial.print("Pad ");
            Serial.print(i);
            Serial.print(": ");
            //Serial.println(pgm_read_byte(&(defConf[address])));
            address++;

        }

        Serial.println("Current notes for current preset: ");
        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            Serial.print("Pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padNote[i][0]);

        }

    }
    #endif

    localOctaveValue = DEFAULT_OCTAVE;

    for (int i=0; i<NOTES_PER_PAD; i++)
        if (padNote[0][i] != BLANK_NOTE)    {

            localOctaveValue = (padNote[0][i]) / MIDI_OCTAVE_RANGE;
            break;

        }

}

void Pads::setPadNotes(uint8_t preset)  {

    int16_t eepromAddress = getActivePresetAddress();

    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        if (preset < NUMBER_OF_PREDEFINED_SCALES)   {

            //predefined preset

            padNote[i][0] = eeprom_read_byte((uint8_t*)eepromAddress+i);
            //update LED state
            //sendLEDstateTonicCallback(getTonicFromNote(padNote[i][0]), ledIntensityDim);

            }   else {  //user preset

            for (int j=0; j<NOTES_PER_PAD; j++) {

                padNote[i][j] = eeprom_read_byte((uint8_t*)eepromAddress+(i*NOTES_PER_PAD)+j);
                //if (padNote[i][j] != BLANK_NOTE)
                    //sendLEDstateTonicCallback(getTonicFromNote(padNote[i][j]), ledIntensityDim);

            }

        }

    }

}

void Pads::getPressureLimits()  {

    int16_t eepromAddress = EEPROM_PAD_PRESSURE_LIMITS_START;

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padPressureLimitLower[i] = eeprom_read_word((uint16_t*)eepromAddress+i*2);
        padPressureLimitUpper[i] = eeprom_read_word((uint16_t*)eepromAddress+1+i*2);

        #if MODE_SERIAL
            Serial.print("Lower pressure limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padPressureLimitLower[i]);

            Serial.print("Upper pressure limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padPressureLimitUpper[i]);
            Serial.println();
        #endif

    }

}

void Pads::getXLimits()  {

    int16_t eepromAddress = EEPROM_PAD_X_LIMITS_START;

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padXLimitLower[i] = eeprom_read_word((uint16_t*)eepromAddress+i*2);
        padXLimitUpper[i] = eeprom_read_word((uint16_t*)eepromAddress+1+i*2);

        #if MODE_SERIAL
            Serial.print("Lower X limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padXLimitLower[i]);

            Serial.print("Upper X limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padXLimitUpper[i]);
            Serial.println();
        #endif

    }

}

void Pads::getYLimits()  {

    int16_t eepromAddress = EEPROM_PAD_Y_LIMITS_START;

    for (int i=0; i<NUMBER_OF_PADS; i++) {

        padYLimitLower[i] = eeprom_read_word((uint16_t*)eepromAddress+i*2);
        padYLimitUpper[i] = eeprom_read_word((uint16_t*)eepromAddress+1+i*2);

        #if MODE_SERIAL
            Serial.print("Lower Y limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padYLimitLower[i]);

            Serial.print("Upper Y limit for pad ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(padYLimitUpper[i]);
            Serial.println();
        #endif

    }

}

uint8_t Pads::getMIDIchannel() {

    return midiChannel;

}

uint8_t Pads::calibratePressure(uint8_t pad, int16_t pressure, pressureType type) {

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

uint8_t Pads::calibrateXY(uint8_t padNumber, int16_t xyValue, ccType type) {

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

//get


bool Pads::getCCXsendEnabled(uint8_t padNumber)    {

    return bitRead(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_X_CC);

}

bool Pads::getCCYsendEnabled(uint8_t padNumber)    {

    return bitRead(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_Y_CC);

}

bool Pads::getNoteSendEnabled(uint8_t padNumber)   {

    return bitRead(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_NOTE);

}

int16_t Pads::getEEPROMaddressFeatures(uint8_t padNumber)   {

    switch(splitCounter)    {

        case 0:
        case 1:
        //global settings
        return getActiveProgramAddress() + 2;

        case 2:
        //split
        return getActiveProgramAddress() + 19 + padNumber*9;

        default:
        return 0;

    }

}

splitState Pads::getSplitState()    {

    switch(splitCounter)    {

        case 0:
        return splitOff;

        case 1:
        return splitXY;

        case 2:
        return splitXYFunctions;

    }   return splitOff;

}

//set

void Pads::setCCXsendEnabled(uint8_t padNumber, uint8_t state)    {

    int16_t eepromAddress = getEEPROMaddressFeatures(padNumber);

    bitWrite(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_X_CC, state);
    eeprom_update_byte((uint8_t*)eepromAddress, padFeatures[padNumber]);

}

void Pads::setCCYsendEnabled(uint8_t padNumber, uint8_t state)    {

    int16_t eepromAddress = getEEPROMaddressFeatures(padNumber);

    bitWrite(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_Y_CC, state);
    eeprom_update_byte((uint8_t*)eepromAddress, padFeatures[padNumber]);

}

void Pads::setNoteSendEnabled(uint8_t padNumber, uint8_t state)   {

    int16_t eepromAddress = getEEPROMaddressFeatures(padNumber);

    bitWrite(padFeatures[padNumber], EEPROM_PAD_FEATURE_BIT_NOTE, state);
    eeprom_update_byte((uint8_t*)eepromAddress, padFeatures[padNumber]);

}

bool Pads::setLowerPressureLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_PRESSURE_LIMITS_START+(pad*4);

    if ((int)limit != padPressureLimitLower[pad])    {

        padPressureLimitLower[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

}

bool Pads::setUpperPressureLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_PRESSURE_LIMITS_START+(pad*4)+2;

    if ((int)limit != padPressureLimitUpper[pad])    {

        padPressureLimitUpper[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

}

bool Pads::setLowerXLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_X_LIMITS_START+(pad*4);

    if ((int)limit != padXLimitLower[pad])    {

        padXLimitLower[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

}

bool Pads::setUpperXLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_X_LIMITS_START+(pad*4)+2;

    if ((int)limit != padXLimitUpper[pad])    {

        padXLimitUpper[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

}

bool Pads::setLowerYLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_Y_LIMITS_START+(pad*4);

    if ((int)limit != padYLimitLower[pad])    {

        padYLimitLower[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

}

bool Pads::setUpperYLimit(uint8_t pad, uint16_t limit)   {

    int16_t eepromAddress = EEPROM_PAD_Y_LIMITS_START+(pad*4)+2;

    if ((int)limit != padYLimitUpper[pad])    {

        padYLimitUpper[pad] = limit;
        eeprom_write_word((uint16_t*)eepromAddress, limit);

    }

    return (limit == eeprom_read_word((uint16_t*)eepromAddress));

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
        sendLEDstateCallback(LED_ON_OFF_AFTERTOUCH, ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_NOTES, ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_X, ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_Y, ledIntensityOff);

        //turn on feature LEDs depending on enabled features for last touched pad
        sendLEDstateCallback(LED_ON_OFF_AFTERTOUCH, getAfterTouchSendState(pad) ? ledIntensityFull : ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_NOTES, getNoteSendState(pad) ? ledIntensityFull : ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_X, getXSendState(pad) ? ledIntensityFull : ledIntensityOff);
        sendLEDstateCallback(LED_ON_OFF_Y, getYSendState(pad) ? ledIntensityFull : ledIntensityOff);

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

            #ifdef KONTROLA_BREADBOARD
                xValue = getY();
            #else
                xValue = getX();
            #endif

        }

        if (xValue == -1) return false;

        //we have x
        if (yValue == -1) {

            #ifdef KONTROLA_BREADBOARD
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

void Pads::update()  {

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
        if (xyProcessed)    {

            checkXY();

        }

    }

    if (switchToNextPad)  {

        //if we got to this point, everything that can be read is read

        //check data to be sent

        if (padMovementDetected)   {

            updateLastTouchedPad();
            padMovementDetected = false;

        }

        if (!padEditMode)   checkMIDIdata();
        firstRun = true;
        setNextPad();

    }   if (!padEditMode)   checkNoteBuffer();  //send notes after some delay

    checkOctaveShift();

}

void Pads::updateLastTouchedPad()   {

    if (padID[activePad] != previousPad)
        setFunctionLEDs(padID[activePad]);

    if (padID[activePad] != lastTouchedPad) {

        if (padEditMode) sendPadEditModeCallback(padID[activePad]);

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

int16_t Pads::getMedianValueZXY(coordinateType type)  {

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
                Serial.println(midiX);
                Serial.print(F("X CC: "));
                Serial.println(ccXPad[pad]);
            #else
                #if XY_INVERT > 0
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
                Serial.println(midiY);
                Serial.print(F("Y CC: "));
                Serial.println(ccYPad[pad]);
            #else
                #if XY_INVERT > 0
                    midi.sendControlChange(midiChannel, ccYPad[pad], 127-midiY);
                #else
                    midi.sendControlChange(midiChannel, ccYPad[pad], midiY);
                #endif
                lastYMIDIvalue[pad] = midiY;
            #endif

        }

    }

    if (!getPadEditMode())      {

        #if XY_INVERT > 0
            sendLCDxyCallback(pad, 127-midiX, 127-midiY, xAvailable_, yAvailable_);
        #else
            sendLCDxyCallback(pad, midiX, midiY, xAvailable_, yAvailable_);
        #endif

    }

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

void Pads::notesOnOff()    {

    bool newNotesState;

    if (splitCounter != 2)   {   //feature splitting is off

        newNotesState = !bitRead(padFeatures[0], EEPROM_PAD_FEATURE_BIT_NOTE);

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setNoteSendEnabled(i, newNotesState);

        #if MODE_SERIAL
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newNotesState = !bitRead(padFeatures[lastTouchedPad], EEPROM_PAD_FEATURE_BIT_NOTE);

        setNoteSendEnabled(lastTouchedPad, newNotesState);

        #if MODE_SERIAL
            Serial.print(F("Notes "));
            newNotesState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

void Pads::xOnOff()    {

    bool newXState;

    if (splitCounter != 2)   {   //feature splitting is off

        newXState = !bitRead(padFeatures[0], EEPROM_PAD_FEATURE_BIT_X_CC);

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setCCXsendEnabled(i, newXState);

        #if MODE_SERIAL
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newXState = !bitRead(padFeatures[lastTouchedPad], EEPROM_PAD_FEATURE_BIT_X_CC);
        setCCXsendEnabled(lastTouchedPad, newXState);

        #if MODE_SERIAL
            Serial.print(F("X "));
            newXState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

void Pads::yOnOff()    {

    bool newYState;

    if (splitCounter != 2)   {   //feature splitting is off

        newYState = !bitRead(padFeatures[0], EEPROM_PAD_FEATURE_BIT_Y_CC);

        for (int i=0; i<NUMBER_OF_PADS; i++)
            setCCYsendEnabled(i, newYState);

        #if MODE_SERIAL
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.println(F(" for all pads"));
        #endif

    }

    else {  //feature splitting is on

        newYState = !bitRead(padFeatures[lastTouchedPad], EEPROM_PAD_FEATURE_BIT_Y_CC);
        setCCYsendEnabled(lastTouchedPad, newYState);

        #if MODE_SERIAL
            Serial.print("Y ");
            newYState ? Serial.print(F("on")) : Serial.print(F("off"));
            Serial.print(F(" for pad "));
            Serial.println(lastTouchedPad);
        #endif

    }

}

uint8_t Pads::getActiveProgram()    {

    return configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveProgramSection);

}

bool Pads::setActiveProgram(int8_t program)   {

    if (program < 0 || program > 9)    return false;

    eeprom_update_byte((uint8_t*)EEPROM_ACTIVE_PROGRAM, program);

    return (eeprom_read_byte((uint8_t*)EEPROM_ACTIVE_PROGRAM) == program);

}

bool Pads::changeActiveProgram(bool direction)  {

    if (direction) activeProgram++; else activeProgram--;
    if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
    else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);

    if (!setActiveProgram(activeProgram)) return false;
    getProgramParameters();

    return true;

}

uint8_t Pads::getActivePreset()    {

    return configuration.readParameter(CONF_BLOCK_PROGRAM, programLastActiveScaleSection, activeProgram);

}

bool Pads::setActivePreset(uint8_t preset)  {

    int16_t eepromAddress = getActiveProgramAddress();

    if (preset < 0 || preset > 16)    return false;

    eeprom_update_byte((uint8_t*)eepromAddress, preset);

    if (!(eeprom_read_byte((uint8_t*)eepromAddress) == preset)) return false;
    return true;

}

bool Pads::changeActivePreset_incDec(bool direction)   {

    if (direction) activePreset++; else activePreset--;
    if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
    else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

    if (!setActivePreset(activePreset)) return false;
    getPresetParameters();
    return true;

}

bool Pads::changeActivePreset_direct(uint8_t _preset)   {

    activePreset = _preset;
    if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
    else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

    if (!setActivePreset(activePreset)) return false;
    getPresetParameters();
    return true;

}

int16_t Pads::getActiveProgramAddress()  {

    return programAddress[getActiveProgram()];

}

int16_t Pads::getActivePresetAddress()  {

    //active preset address is first byte in program
    return presetAddress[getActivePreset()];

}

void Pads::splitFunctions() {

    splitCounter++;
    if (splitCounter == splitEnd)  splitCounter = splitOff;

    eeprom_update_byte((uint8_t*)getActiveProgramAddress() + 1, splitCounter);
    getPadParameters();

    #if MODE_SERIAL
        switch(splitCounter)    {

            case 0:
            Serial.println(F("Split off"));
            break;

            case 1:
            Serial.println(F("Split XY"));
            break;

            case 2:
            Serial.println(F("Split XY + functions"));
            break;

        }

    #endif

}

ledIntensity Pads::getSplitStateLEDvalue()    {

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

changeOutput Pads::shiftOctave(bool direction)  {

    bool notesChanged = true;
    changeOutput result = outputChanged;

    //check if note/notes are too low/high if shifted
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        if (!direction)  {  //shift down

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                    if (padNote[i][j] < MIDI_OCTAVE_RANGE)
                        notesChanged = false;

                if (!notesChanged) break;

            }

            }   else {  //shift up

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                    if ((padNote[i][j] + MIDI_OCTAVE_RANGE) > MAX_MIDI_VALUE)
                        notesChanged = false;

                if (!notesChanged) break;

            }

        }

    }

    if (!notesChanged)    {

        #if MODE_SERIAL
            Serial.print("Unable to do global shift: one or more pad notes are too ");
            direction ? Serial.println("high.") : Serial.println("low");
        #endif
        result = outOfRange;
        localOctaveValue = getOctaveFromNote(padNote[0][0]);

    }   else {

        if (direction) shiftAmount++;
        else shiftAmount--;

        localOctaveValue = getOctaveFromNote(padNote[0][0] + MIDI_OCTAVE_RANGE*shiftAmount);

    }

    return result;

}

changeOutput Pads::shiftNote(bool direction) {

    //shift scale one note up or down
    //tonic remains the same, it just gets shifted to other pad

    scaleType currentScaleType = getActiveScaleType();
    if (currentScaleType == userScale) return notAllowed;
    int16_t tempNoteArray[NUMBER_OF_PADS];

    switch(direction)   {

        case true:
        //up
        tempNoteArray[NUMBER_OF_PADS-1] = padNote[NUMBER_OF_PADS-currentScaleType][0] + MIDI_OCTAVE_RANGE;
        if (tempNoteArray[NUMBER_OF_PADS-1] > 127) return outOfRange;
        for (int i=0; i<NUMBER_OF_PADS-1; i++)    {

            tempNoteArray[i] = padNote[i+1][0];

        }
        shiftedNote++;
        break;

        case false:
        //down
        tempNoteArray[0] = padNote[currentScaleType-1][0] - MIDI_OCTAVE_RANGE;
        if (tempNoteArray[0] < 0) return outOfRange;
        for (int i=0; i<NUMBER_OF_PADS-1; i++)    {

            tempNoteArray[i+1] = padNote[i][0];

        }
        shiftedNote--;
        break;

    }

    if (abs(shiftedNote) == (uint8_t)currentScaleType)
        shiftedNote = 0;

    #if MODE_SERIAL
        Serial.print("Shifted note: ");
        Serial.println(shiftedNote);
    #endif

    for (int i=0; i<NUMBER_OF_PADS; i++)
        setNewPadNote(i, tempNoteArray[i]);

    return outputChanged;

}

uint8_t Pads::getActiveOctave()    {

    return localOctaveValue;

}

tonic Pads::getActiveTonic()    {

    scaleType currentScaleType = getActiveScaleType();

    if (getActivePreset() >= NUMBER_OF_PREDEFINED_SCALES)   {

        //currentScaleTonic is first found note on first pad on user presets

        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[0][i] != BLANK_NOTE)
                return getTonicFromNote(padNote[0][i]);

    }   else  {

        uint8_t tonicLocation = 0;

        if (shiftedNote >= 0)
            tonicLocation = (uint8_t)currentScaleType - shiftedNote;
        else tonicLocation = shiftedNote;

        //we have tonic now
        return getTonicFromNote(padNote[tonicLocation][0]);

    }   return tonicInvalid;

}

void Pads::setNewPadNote(uint8_t padNumber, uint8_t note)  {

    int16_t eepromAddress = getActivePresetAddress();
    uint8_t preset = getActivePreset();
    int8_t newNoteIndex = NOTES_PER_PAD;

    if (preset < NUMBER_OF_PREDEFINED_SCALES)  {

        eepromAddress += padNumber;
        padNote[padNumber][0] = note;

    }

    else {

        //find first available note index
        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[padNumber][i] == BLANK_NOTE) newNoteIndex = i-1;

            //in case that pad doesn't have any assigned notes, newNoteIndex will be -1
            //fix that to 0
            if (newNoteIndex < 0) newNoteIndex = 0;

            //if newNoteIndex hasn't changed, we can't assign
            //more notes to the pad
            if (newNoteIndex == NOTES_PER_PAD) return;

        eepromAddress += padNumber*NOTES_PER_PAD + newNoteIndex;
        padNote[padNumber][newNoteIndex] = note;

    }

    eeprom_update_byte((uint8_t*)eepromAddress, note);

    #if MODE_SERIAL
        Serial.print("Note assigned to pad ");
        Serial.print(padNumber);
        Serial.print(": ");
        Serial.println(note);
    #endif

}

void Pads::shiftPadNote(uint8_t padNumber, bool direction, uint8_t noteIndex, uint8_t range)   {

    //internal function
    //do not perform any checking as it's done in shiftOctave function

    int16_t eepromAddress = getActivePresetAddress();
    uint8_t preset = getActivePreset();

    if (preset < NUMBER_OF_PREDEFINED_SCALES)  eepromAddress += padNumber;
    else                                        eepromAddress += padNumber*NOTES_PER_PAD + noteIndex;

    if (direction)  padNote[padNumber][noteIndex] += range;
    else            padNote[padNumber][noteIndex] -= range;

    eeprom_update_byte((uint8_t*)eepromAddress, padNote[padNumber][noteIndex]);

    #if MODE_SERIAL
        Serial.print("Note assigned to pad ");
        Serial.print(padNumber);
        Serial.print(": ");
        Serial.println(padNote[padNumber][noteIndex]);
    #endif

}

changeOutput Pads::setTonic(tonic _tonic)  {

    changeOutput result = noChange;
    tonic currentScaleTonic = getActiveTonic();

    //determine distance between notes
    uint8_t changeDifference;

    bool changeAllowed = true;
    bool shiftDirection;

    if (currentScaleTonic == tonicInvalid) return result; //pad has no notes

    shiftDirection = (uint8_t)currentScaleTonic < (uint8_t)_tonic;

    changeDifference = abs((uint8_t)currentScaleTonic - (uint8_t)_tonic);
    if (!changeDifference) { result = noChange; return result; }
    changeAllowed = true;

    //check if all notes are within range before shifting
    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        if (!changeAllowed) break;

        for (int j=0; j<NOTES_PER_PAD; j++)  {

            if ((uint8_t)currentScaleTonic < (uint8_t)_tonic)      {

                if (padNote[i][j] != BLANK_NOTE)
                if ((padNote[i][j] + changeDifference) > MAX_MIDI_VALUE) changeAllowed = false;

            }

            else if ((uint8_t)currentScaleTonic > (uint8_t)_tonic) {

                if (padNote[i][j] != BLANK_NOTE)
                if ((padNote[i][j] - changeDifference) < MIN_MIDI_VALUE) changeAllowed = false;

            }

        }

    }

    //change notes
    if (changeAllowed)  {

        result = outputChanged;

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (padNote[i][j] != BLANK_NOTE)
                    shiftPadNote(i, shiftDirection, j, changeDifference);

            }

        }

    }

    else {

        #if MODE_SERIAL
        //Serial.print("Unable to change tonic: one or more pad notes are too ");
        //(currentScaleTonic < tonic) ? Serial.println("high.") : Serial.println("low");
        #endif

        result = outOfRange;

    }

    return result;

}

uint8_t Pads::getNumberOfAssignedNotes(uint8_t padNumber)   {

    if (getActivePreset() < NUMBER_OF_PREDEFINED_SCALES) return 1; //predefined presets only have one note

    int16_t eepromAddress = getActivePresetAddress() + MIDI_OCTAVE_RANGE*padNumber;
    uint8_t activatedNotesCounter = 0;

    for (int i=0; i<MIDI_OCTAVE_RANGE; i++) {

        if (eeprom_read_byte((uint8_t*)eepromAddress+i) != BLANK_NOTE)
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

        if (ccXvalueActivePad == ccXvaluePreviousPad)   {

            midi.sendControlChange(midiChannel, ccXvaluePreviousPad, lastXMIDIvalue[previousPad]);

        }   else if (ccYvalueActivePad == ccYvaluePreviousPad)  {

            midi.sendControlChange(midiChannel, ccYvaluePreviousPad, lastYMIDIvalue[previousPad]);

        }

            uint8_t noteArray[NOTES_PER_PAD],
                    noteCounter = 0;

            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[previousPad][i] != BLANK_NOTE)  {

                    noteArray[noteCounter] = padNote[previousPad][i];
                    noteCounter++;

                }

            }   sendPadPressCallback(previousPad, noteArray, noteCounter, lastVelocityValue[previousPad], ccXvaluePreviousPad, ccYvaluePreviousPad);

        sendLCDxyCallback(previousPad, lastXValue[previousPad], lastYValue[previousPad], ccXsendEnabled, ccYsendEnabled);
        setFunctionLEDs(previousPad);

    }   else if (!midiNoteOnOff) resetPadLCDdata = true;

    storePadNote(pad, midiVelocity, midiNoteOnOff);

    lastVelocityValue[pad] = midiVelocity;
    if (resetPadLCDdata && !getPadEditMode()) sendClearPadDataCallback(pad);
    velocityAvailable = false;

}

tonic Pads::getTonicFromNote(uint8_t note)    {

    if (note == BLANK_NOTE) return tonicInvalid;
    return (tonic)(note % MIDI_OCTAVE_RANGE);

}

bool Pads::getNoteSendState(uint8_t pad) {

    return bitRead(padFeatures[pad], EEPROM_PAD_FEATURE_BIT_NOTE);

}

bool Pads::getXSendState(uint8_t pad) {

    return bitRead(padFeatures[pad], EEPROM_PAD_FEATURE_BIT_X_CC);

}

bool Pads::getYSendState(uint8_t pad) {

    return bitRead(padFeatures[pad], EEPROM_PAD_FEATURE_BIT_Y_CC);

}

uint8_t Pads::getOctaveFromNote(uint8_t note)  {

    if (note == BLANK_NOTE) return DEFAULT_OCTAVE;
    return note / MIDI_OCTAVE_RANGE;

}

uint8_t Pads::getLastTouchedPad()   {

    return lastTouchedPad;

}

changeOutput Pads::changeCC(bool direction, ccType type, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
    uint8_t endPad = globalShift ? NUMBER_OF_PADS : lastTouchedPad+1;
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch (type) {

        case ccTypeX:
        if (direction)  compareResult = ccXPad[startPad] + steps > compareValue;
        else            compareResult = ccXPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXPad[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXPad[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCC(i, changedValue, ccTypeX, globalShift);

        }
        break;

        case ccTypeY:
        if (direction)  compareResult = ccYPad[startPad] + steps > compareValue;
        else            compareResult = ccYPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYPad[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYPad[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCC(i, changedValue, ccTypeY, globalShift);

        }
        break;

    }

    return result;

}

void Pads::setNewPadCC(uint8_t padNumber, uint8_t ccValue, ccType type, bool globalShift)  {

    //get last active program address
    int16_t eepromAddress = getActiveProgramAddress();

    if (globalShift) eepromAddress += 3;
    else             eepromAddress += 11 + padNumber*9;

    #if MODE_SERIAL
        Serial.print("New ");
    #endif

    switch(type)  {

        case ccTypeX:
        #if MODE_SERIAL
            Serial.print("X ");
        #endif
        ccXPad[padNumber] = ccValue;
        break;

        case ccTypeY:
        #if MODE_SERIAL
            Serial.print("Y ");
        #endif
        eepromAddress++;
        ccYPad[padNumber] = ccValue;
        break;

    }

    eeprom_update_byte((uint8_t*)eepromAddress, ccValue);

    #if MODE_SERIAL
        Serial.print("CC value for pad ");
        Serial.print(padNumber);
        Serial.print(": ");
        Serial.println(ccValue);
    #endif

}

changeOutput Pads::changeXYlimits(bool direction, ccLimitType ccType, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
    uint8_t endPad = globalShift ? NUMBER_OF_PADS : lastTouchedPad+1;
    uint8_t compareValue = 127;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch(ccType)  {

        case ccLimitTypeXmax:
        if (direction)  compareResult = ccXmaxPad[startPad] + steps > compareValue;
        else            compareResult = ccXmaxPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXmaxPad[lastTouchedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXmaxPad[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCClimit(i, changedValue, ccLimitTypeXmax, globalShift);

        }
        break;

        case ccLimitTypeXmin:
        if (direction)  compareResult = ccXminPad[startPad] + steps > compareValue;
        else            compareResult = ccXminPad[startPad] + steps < compareValue;

        if (!compareResult) changedValue = ccXminPad[lastTouchedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccXminPad[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCClimit(i, changedValue, ccLimitTypeXmin, globalShift);

        }
        break;

        case ccLimitTypeYmax:
        if (direction)  compareResult = ccYmaxPad[lastTouchedPad] + steps > compareValue;
        else            compareResult = ccYmaxPad[lastTouchedPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYmaxPad[lastTouchedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYmaxPad[lastTouchedPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCClimit(i, changedValue, ccLimitTypeYmax, globalShift);

        }
        break;

        case ccLimitTypeYmin:
        if (direction)  compareResult = ccYminPad[lastTouchedPad] + steps > compareValue;
        else            compareResult = ccYminPad[lastTouchedPad] + steps < compareValue;

        if (!compareResult) changedValue = ccYminPad[lastTouchedPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (ccYminPad[lastTouchedPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewPadCClimit(i, changedValue, ccLimitTypeYmin, globalShift);

        }
        break;

    }

    return result;

}

void Pads::setNewPadCClimit(uint8_t padNumber, uint8_t value, ccLimitType limitType, bool globalShift)    {

    //get last active program address
    int16_t eepromAddress = getActiveProgramAddress();

    if (globalShift) eepromAddress += 5;
    else             eepromAddress += 13 + padNumber*9;

    #if MODE_SERIAL
        Serial.print("New ");
    #endif

    switch(limitType)   {

        case ccLimitTypeXmin:
        #if MODE_SERIAL
        Serial.print("X min ");
        #endif
        ccXminPad[padNumber] = value;
        break;

        case ccLimitTypeXmax:
        eepromAddress++;
        #if MODE_SERIAL
            Serial.print("X max ");
        #endif
        ccXmaxPad[padNumber] = value;
        break;

        case ccLimitTypeYmin:
        eepromAddress += 2;
        #if MODE_SERIAL
        Serial.print("Y min ");
        #endif
        ccYminPad[padNumber] = value;
        break;

        case ccLimitTypeYmax:
        eepromAddress += 3;
        #if MODE_SERIAL
            Serial.print("Y max ");
        #endif
        ccYmaxPad[padNumber] = value;
        break;

    }

    eeprom_update_byte((uint8_t*)eepromAddress, value);

    #if MODE_SERIAL
        Serial.print("CC value for pad ");
        Serial.print(padNumber);
        Serial.print(": ");
        Serial.println(value);
    #endif

}

changeOutput Pads::changeCurve(bool direction, curveCoordinate coordinate, int8_t steps)  {

    bool globalShift = (splitCounter == 0);
    changeOutput result = outputChanged;
    uint8_t startPad = globalShift ? 0 : lastTouchedPad;
    uint8_t endPad = globalShift ? NUMBER_OF_PADS : lastTouchedPad+1;
    uint8_t compareValue = NUMBER_OF_SCALES;
    bool compareResult;
    uint8_t changedValue = 0;
    bool changeAllowed = true;

    if (!direction) {steps *= -1; compareValue = 0; }

    switch(coordinate)   {

        case curveCoordinateX:
        if (direction)  compareResult = padCurveX[startPad] + steps > compareValue;
        else            compareResult = padCurveX[startPad] + steps < compareValue;

        if (!compareResult) changedValue = padCurveX[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (padCurveX[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewCurve(i, changedValue, curveCoordinateX, globalShift);

        }
        break;

        case curveCoordinateY:
        if (direction)  compareResult = padCurveY[startPad] + steps > compareValue;
        else            compareResult = padCurveY[startPad] + steps < compareValue;

        if (!compareResult) changedValue = padCurveY[startPad]+steps;
        else {

            //result out of range
            //just assign compareValue if it's not already assigned
            if (padCurveY[startPad] != compareValue)
                changedValue = compareValue;
            else { changeAllowed = false; result = noChange; }

        }

        if (changeAllowed)  {

            for (int i=startPad; i<endPad; i++)
                setNewCurve(i, changedValue, curveCoordinateY, globalShift);

        }
        break;

        case curveCoordinateInvalid:
        return notAllowed;

    }

    return result;

}

void Pads::setNewCurve(uint8_t padNumber, uint8_t newCurveValue, curveCoordinate coordinate, bool globalShift) {

    //get last active program address
    int16_t eepromAddress = getActiveProgramAddress();

    if (globalShift) eepromAddress +=9;
    else             eepromAddress += 17 + padNumber*9;

    #if MODE_SERIAL
    Serial.print("New ");
    #endif

    switch(coordinate)   {

        case curveCoordinateX:
        #if MODE_SERIAL
            Serial.print("X curve ");
        #endif
        padCurveX[padNumber] = newCurveValue;
        break;

        case curveCoordinateY:
        eepromAddress++;
        #if MODE_SERIAL
            Serial.print("Y curve ");
        #endif
        padCurveY[padNumber] = newCurveValue;
        break;

        case curveCoordinateInvalid:
        return;

    }

    eeprom_update_byte((uint8_t*)eepromAddress, newCurveValue);

    #if MODE_SERIAL
        Serial.print("value for pad ");
        Serial.print(padNumber);
        Serial.print(": ");
        Serial.println(newCurveValue);
    #endif

}

curveType Pads::getPadCurve(curveCoordinate coordinate, uint8_t padNumber)    {

    switch(coordinate)   {

        case curveCoordinateX:
        return (curveType)padCurveX[padNumber];

        case curveCoordinateY:
        return (curveType)padCurveY[padNumber];

        default:
        return curveTypeInvalid;

    }

}

uint8_t Pads::getPadCClimitValue(ccType type, ccLimitType limitType, uint8_t padNumber)   {

    switch(type)  {

        case ccTypeX:
        if (limitType == ccLimitTypeXmax) return ccXmaxPad[padNumber];
        else return ccXminPad[padNumber];

        case ccTypeY:
        if (limitType == ccLimitTypeYmax) return ccYmaxPad[padNumber];
        else return ccYminPad[padNumber];

    }

    return 0;

}

uint8_t Pads::getPadCCvalue(ccType type, uint8_t padNumber)  {

    switch(type)  {

        case ccTypeX:
        return ccXPad[padNumber];

        case ccTypeY:
        return ccYPad[padNumber];

    }   return 0;

}

changeOutput Pads::changeMIDIchannel(bool direction)  {

    if (direction) midiChannel++;
    else           midiChannel--;

    if (midiChannel < 1)  midiChannel = 16;
    if (midiChannel > 16) midiChannel = 1;

    eeprom_update_byte((uint8_t*)EEPROM_MIDI_CHANNEL, midiChannel);

    return outputChanged;

}

uint8_t* Pads::getPadNotes(uint8_t padNumber)   {

    return padNote[padNumber];

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

        //proceed if noteState is off
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

        uint8_t noteArray[NOTES_PER_PAD],
                noteCounter = 0;

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

                    midi.sendNoteOff(midiChannel, padNote[pad][i], 0);

                }
                #endif

            }

            #if MODE_SERIAL
            if (noteState)  {

                Serial.print(F("Velocity: "));
                Serial.println(velocity);

            }
            #endif

            for (int i=0; i<NOTES_PER_PAD; i++) {

                if (padNote[pad][i] != BLANK_NOTE)  {

                    noteArray[noteCounter] = padNote[pad][i];
                    noteCounter++;

                }

            }

            if (!noteState)  sendPadReleaseCallback(pad, noteArray, noteCounter);

        }   if (noteState)  {

                uint8_t ccX = getPadCCvalue(ccTypeX, pad);
                uint8_t ccY = getPadCCvalue(ccTypeY, pad);

                sendPadPressCallback(pad, noteArray, noteCounter, velocity, ccX, ccY);

            }

        note_buffer_tail = i;

    }

}

void Pads::changeActiveOctave(bool direction)   {

    if (direction)  localOctaveValue++;
    else localOctaveValue--;

    if (localOctaveValue >= (MIDI_OCTAVE_RANGE-2)) localOctaveValue--;
    if (localOctaveValue < -2) localOctaveValue++;

}

void Pads::setPadEditMode(bool state) {

    padEditMode = state;

}

bool Pads::getPadEditMode() {

    return padEditMode;

}

changeOutput Pads::changePadNote(uint8_t tonic)    {

    uint8_t noteIndex = 0;
    uint8_t note = getActiveOctave()*MIDI_OCTAVE_RANGE + tonic;
    bool addOrRemove = true;    //add by default
    int16_t address;

    //check if calculated note is already assigned to pad
    for (int i=0; i<NOTES_PER_PAD; i++)
        if (padNote[lastTouchedPad][i] == note) { addOrRemove = false; noteIndex = i; break; }

    //if it isn't, add it
    if (addOrRemove)    {

        //get number of assigned notes to last touched pad
        for (int i=0; i<NOTES_PER_PAD; i++)
            if (padNote[lastTouchedPad][i] != BLANK_NOTE)
                noteIndex++;

        //pads cannot have more than NOTES_PER_PAD notes
        if (noteIndex == NOTES_PER_PAD) return outOfRange;

        //assign new pad note
        padNote[lastTouchedPad][noteIndex] = note;
        address = getActivePresetAddress() + (NOTES_PER_PAD*lastTouchedPad) + noteIndex;
        eeprom_update_byte((uint8_t*)address, note);

    }   else {

        address = getActivePresetAddress() + (NOTES_PER_PAD*lastTouchedPad);

        note = BLANK_NOTE; //else delete note (assign BLANK_NOTE)

        //assign new pad note
        padNote[lastTouchedPad][noteIndex] = note;

        //copy note array
        uint8_t tempNoteArray[NOTES_PER_PAD];

        for (int i=0; i<NOTES_PER_PAD; i++)
            tempNoteArray[i] = padNote[lastTouchedPad][i];

        //shift all notes so that BLANK_NOTE is at the end of array
        for (int i=noteIndex; i<(NOTES_PER_PAD-1); i++) {

            padNote[lastTouchedPad][i] = tempNoteArray[i+1];

        }   padNote[lastTouchedPad][NOTES_PER_PAD-1] = BLANK_NOTE;

        for (int i=0; i<NOTES_PER_PAD; i++)
            eeprom_update_byte((uint8_t*)address+i, padNote[lastTouchedPad][i]);

    }

    return outputChanged; 

}

void Pads::checkOctaveShift()   {

    if (!shiftAmount) return;

    if (checkPadsPressed()) return;

    static uint8_t localOctaveValueEmptyPad = DEFAULT_OCTAVE;
    bool direction = shiftAmount > 0;
    bool emptyPad = true;
    if (shiftAmount < 0) shiftAmount*=-1;

    for (int i=0; i<NUMBER_OF_PADS; i++)    {

        for (int j=0; j<NOTES_PER_PAD; j++) {

            //do not update blank notes
            if (padNote[i][j] != BLANK_NOTE)
                shiftPadNote(i, direction, j, MIDI_OCTAVE_RANGE*shiftAmount);

            }

        }

    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (padNote[0][i] != BLANK_NOTE) {

            //local octave value is octave that gets displayed
            //on LCD as changed octave
            localOctaveValue = getOctaveFromNote(padNote[0][0]);
            emptyPad = false;
            break;

        }

    }

    if (emptyPad)   {

        if (direction) localOctaveValueEmptyPad++; else localOctaveValueEmptyPad--;
        localOctaveValue = localOctaveValueEmptyPad;

    }

    shiftAmount = 0;

}

bool Pads::checkPadsPressed()   {

    //return true if any pad is pressed
    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (getPadPressed(i)) return true;

    return false;

}

scaleType Pads::getActiveScaleType() {

    uint8_t currentPreset = getActivePreset();

    switch(currentPreset)   {

        case scaleNaturalMinor:
        case scaleMelodicMinor:
        case scaleHarmonicMinor:
        case scaleMajor:
        case scaleHarmonicMajor:
        return sevenNoteScale;

        case scaleMinorPentatonic:
        case scaleMajorPentatonic:
        return fiveNoteScale;

        default:
        return userScale;

    }

}

bool Pads::noteActive(tonic _tonic) {

    //return true if received tonic is among active notes on some pad

    for (int i=0; i<NUMBER_OF_PADS; i++)
        for (int j=0; j<NOTES_PER_PAD; j++)
            if (padNote[i][j] != BLANK_NOTE)
                if (getTonicFromNote(padNote[i][j]) == _tonic) return true;

    return false;

}


Pads pads;